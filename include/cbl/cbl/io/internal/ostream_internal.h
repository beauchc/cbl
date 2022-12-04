#ifndef CBL_IO_INTERNAL_OSTREAM_H
#define CBL_IO_INTERNAL_OSTREAM_H

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
// STRUCT ostream_details
//==============================================================================

struct ostream_details {
    struct newline {};

    using pretty     = iomanip::pretty;
    using set_indent = iomanip::set_indent;

    template <typename... Args>
    static constexpr size_t tuple_size(std::tuple<Args...> const&);
    template <typename F, typename S>
    static constexpr size_t tuple_size(std::pair<F, S> const&);

    template <typename T>
    static constexpr typename T::mapped_type mapped_type(T const&);

    CBL_SFINAE_TEST(is_iterable_sfinae, std::begin, const);
    CBL_SFINAE_TEST(is_tuple_sfinae, tuple_size, const);
    CBL_SFINAE_TEST(is_map_sfinae, mapped_type, const);

    template <typename T>
    inline static constexpr bool is_string_v =
        std::is_convertible_v<T, std::string_view>;

    template <typename T>
    inline static constexpr bool is_map_v = is_map_sfinae<T>::value;

    template <typename T>
    inline static constexpr bool is_iterable_v =
        !is_string_v<T> && is_iterable_sfinae<T>::value;

    template <typename T>
    inline static constexpr bool is_tuple_v =
        !is_iterable_v<T> && is_tuple_sfinae<T>::value;

    template <typename T>
    inline static constexpr bool is_scalar_v =
        !is_iterable_v<T> && !is_tuple_v<T> && !is_map_v<T>;

    template <typename T, typename R = void>
    using enable_if_iterable = std::enable_if_t<is_iterable_v<T>, R>;

    template <typename T, typename R = void>
    using enable_if_tuple = std::enable_if_t<is_tuple_v<T>, R>;

    template <typename T, typename R = void>
    using enable_if_map = std::enable_if_t<is_map_v<T>, R>;

    template <typename T, typename R = void>
    using enable_if_scalar = std::enable_if_t<is_scalar_v<T>, R>;
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

    template <typename... Args>
    void wr(Args const&... args) {
        (wr(args), ...);
    }

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
    enable_if_tuple<T> wr(T const& t) {
        pre_wr();
        print_tuple(t, std::make_index_sequence<std::tuple_size_v<T>>());
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
            wr_it<T>(it++);
        }
        for (; it != end; ++it) {
            os << ',';
            pretty ? wr(newline{}) : wr(' ');
            wr_it<T>(it);
        }
        if (pretty) wr(newline{});

        if (pretty) pop_pretty();
        wr('}');
    }

    template <typename T, typename It>
    std::enable_if_t<is_map_v<T>> wr_it(It it) {
        wr('[', it->first, "] = ", it->second);
    }
    template <typename T, typename It>
    std::enable_if_t<!is_map_v<T>> wr_it(It it) {
        wr(*it);
    }

    template <typename Tuple, std::size_t... Is>
    void print_tuple(const Tuple& t, std::index_sequence<Is...>) {
        wr('(');
        ((wr((Is == 0 ? "" : ", "), std::get<Is>(t))), ...);
        wr(')');
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

//==============================================================================
// CLASS ostream_scope
//==============================================================================

template <typename Derived, typename Scoped>
class ostream_scope {
public:
    ostream_scope() = default;

    /// \brief Constructor
    explicit ostream_scope(ostream& os, Scoped scoped)
        : m_os(&os), m_scoped(std::move(scoped)) {
        static_cast<Derived*>(this)->push_scope();
    }

    ostream_scope(ostream_scope&& o) noexcept {
        std::swap(m_os, o.m_os);
        std::swap(m_scoped, o.m_scoped);
    }
    ostream_scope& operator=(ostream_scope&& o) {
        std::swap(m_os, o.m_os);
        std::swap(m_scoped, o.m_scoped);
        return *this;
    }

    ostream_scope(ostream_scope const&) = delete;
    ostream_scope& operator=(ostream_scope const&) = delete;

    ~ostream_scope() {
        if (m_os) static_cast<Derived*>(this)->pop_scope();
    }

protected:
    ostream* m_os = nullptr;
    Scoped   m_scoped{};
};

} // namespace cbl::internal

#endif
