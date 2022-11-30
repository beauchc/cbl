#ifndef CBL_IO_OSTREAM_INTERNAL_H
#define CBL_IO_OSTREAM_INTERNAL_H

#include <cbl/io/iomanip.h>

#include "sfinae.h"

#include <ostream>
#include <type_traits>

//==============================================================================
// FORWARD DECLARATIONS
//==============================================================================

namespace cbl {
class ostream;
}

//==============================================================================
// NAMESPACE cbl::internal
//==============================================================================

namespace cbl::internal {

struct endl_t {};

//==============================================================================
// CLASS ostream_impl
//==============================================================================

struct ostream_details {
    struct newline {};

    struct indent {
        explicit indent(unsigned indent) : m_indent(indent), m_reset(indent) {}

        explicit operator bool() const { return m_indent > 0; }

        void operator=(unsigned indent) {
            if (m_reset == m_indent) m_indent = indent;
            m_reset = indent;
        }
        void operator+=(unsigned indent) {
            if (m_reset == m_indent) m_indent += indent;
            m_reset += indent;
        }
        void operator-=(unsigned indent) {
            assert(m_reset >= indent);
            if (m_reset == m_indent) m_indent -= indent;
            m_reset -= indent;
        }

        void reset() { m_indent = m_reset; }

        unsigned m_indent;
        unsigned m_reset;
    };

    using pretty     = iomanip::pretty;
    using set_indent = iomanip::set_indent;

    CBL_SFINAE_TEST(is_iterable, begin(), const);

    template <typename T, typename R = void>
    using enable_if_iterable = std::enable_if_t<is_iterable<T>::value, R>;

    template <typename T, typename R = void>
    using enable_if_scalar = std::enable_if_t<!is_iterable<T>::value, R>;
};

//==============================================================================
// CLASS ostream_impl
//==============================================================================

class ostream_impl : private ostream_details {
public:
    explicit ostream_impl(std::ostream& os_) : os{os_} {}

    template <typename... Args>
    void toStream(Args const&... args) {
        (write(args), ...);
    }

    void add_indent(int delta) { m_indent += delta; }

private:
    template <typename T>
    void write(T const& t) {
        wr(t);
        m_pretty.m_depth = 0;
    }
    void write(iomanip::pretty const& pretty) { m_pretty = pretty; }

    void write(iomanip::set_indent const& si) { m_indent = si.m_indent; }

    void write(iomanip::line const& l) {
        os << std::setw(l.m_size) << std::setfill(l.m_c) << l.m_c;
    }

    void wr(newline const&) {
        os << '\n';
        m_indent.reset();
    }

    void wr(endl_t const&) {
        os << std::endl;
        m_indent.reset();
    }

    template <typename T>
    enable_if_scalar<T> wr(T const& t) {
        pre_wr();
        os << t;
    }

    template <typename T>
    enable_if_iterable<T> wr(T const& t) {
        const bool pretty = static_cast<bool>(m_pretty);

        auto it  = t.begin();
        auto end = t.end();

        wr('{');
        if (pretty) push_pretty();

        if (it != end) {
            if (pretty) wr(newline{});
            wr(*it);
            ++it;
        }
        for (; it != end; ++it) {
            os << ',';
            pretty ? wr(newline{}) : wr(' ');
            wr(*it);
        }
        if (pretty) wr(newline{});

        if (pretty) pop_pretty();
        wr('}');
    }

    void pre_wr() {
        if (m_indent) {
            write(iomanip::line{m_indent.m_indent, ' '});
            m_indent.m_indent = 0;
        }
    }

    void push_pretty() {
        m_indent += m_pretty.m_indent;
        --m_pretty.m_depth;
    }

    void pop_pretty() {
        m_indent -= m_pretty.m_indent;
        ++m_pretty.m_depth;
    }

    std::ostream& os;

    indent m_indent{0};
    pretty m_pretty{0, 0};
};

} // namespace cbl::internal

#endif
