#ifndef CBL_IO_INTERNAL_OSTREAM_H
#define CBL_IO_INTERNAL_OSTREAM_H

#include <cbl/io/iomanip.h>

#include "sfinae.h"

#include <ostream>
#include <type_traits>

#include <iostream> // remove

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

    using pretty     = iomanip::pretty;
    using set_indent = iomanip::set_indent;

    CBL_SFINAE_TEST(is_iterable_sfinae, begin(), const);

    template <typename T>
    inline static constexpr bool is_string_v =
        std::is_convertible_v<T, std::string_view>;

    template <typename T>
    inline static constexpr bool is_iterable_v =
        !is_string_v<T> && is_iterable_sfinae<T>::value;

    template <typename T, typename R = void>
    using enable_if_iterable = std::enable_if_t<is_iterable_v<T>, R>;

    template <typename T, typename R = void>
    using enable_if_scalar = std::enable_if_t<!is_iterable_v<T>, R>;
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

    void add_indent(int delta) {
        indent += delta;
        assert(indent >= 0);
    }
    void swap_prefix(std::string_view& other) { std::swap(other, prefix); }

private:
    template <typename T>
    void write(T const& t) {
        wr(t);
        m_pretty.m_depth = 0;
    }
    void write(iomanip::pretty const& pretty) { m_pretty = pretty; }

    void write(iomanip::set_indent const& si) { indent = si.m_indent; }

    void write(iomanip::set_prefix const& sp) { prefix = sp.m_prefix; }

    void wr(newline const&) {
        os << '\n';
        need_indent = true;
    }

    void wr(endl_t const&) {
        os << std::endl;
        need_indent = true;
    }

    void wr(iomanip::line const& l) {
        pre_wr();
        os << std::setw(l.m_size) << std::setfill(l.m_c) << l.m_c;
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
        if (!need_indent) return;
        os << prefix;
        if (indent) os << std::setw(indent) << std::setfill(' ') << ' ';
        need_indent = false;
    }
    void push_pretty() {
        add_indent(m_pretty.m_indent);
        --m_pretty.m_depth;
    }
    void pop_pretty() {
        add_indent(-m_pretty.m_indent);
        ++m_pretty.m_depth;
    }

    std::ostream& os;

    std::string_view prefix {};

    bool need_indent = true;
    int  indent      = 0;

    pretty m_pretty{0, 0};
};

} // namespace cbl::internal

#endif
