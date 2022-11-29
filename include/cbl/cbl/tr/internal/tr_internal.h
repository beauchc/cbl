#ifndef CBL_TR_TR_INTERNAL_H
#define CBL_TR_TR_INTERNAL_H

#include <iterator>
#include <type_traits>

#define CBL_SFINAE_TEST(NAME, RHS, CONST)                    \
    template <typename U>                                    \
    class NAME {                                             \
        template <typename T>                                \
        static decltype(std::declval<T CONST&>().RHS) base(  \
            decltype(std::declval<T CONST&>().RHS));         \
        template <typename T>                                \
        static std::nullptr_t base(...);                     \
                                                             \
    public:                                                  \
        using base_type = decltype(base<U>(nullptr));        \
        static constexpr bool value =                        \
            !std::is_same<std::nullptr_t, base_type>::value; \
    };

namespace cbl::tr_internal {

struct endl {};
struct comma {};
struct newline {};
struct indent {
    explicit operator bool() const { return indent >= 0; }
    int      indent;
};

CBL_SFINAE_TEST(is_iterable, begin(), const);

template <typename T>
using get_t = std::conditional_t<std::is_lvalue_reference_v<T>, T const&, T>;

template <typename T, typename R>
using enable_if_iterable = std::enable_if_t<is_iterable<T>::value, R>;

//------------------------------------------------------------------------------
//
template <typename T>
struct scalar {
    T m_scalar;

    template <typename Os>
    void to_stream(Os& os) const;
};

template <typename T, typename R = scalar<get_t<T>>>
using enable_if_scalar = std::enable_if_t<!is_iterable<T>::value, R>;

//------------------------------------------------------------------------------
//
template <typename T>
struct iterable {
    T   m_iterable;
    int m_indent;

    template <typename Os>
    void to_stream(Os& os) const;
};

template <typename T, typename R = iterable<get_t<T>>>
using enable_if_iterable = std::enable_if_t<is_iterable<T>::value, R>;

//------------------------------------------------------------------------------
//
template <typename T>
enable_if_scalar<T> tr1(T&& t) {
    return {std::forward<T>(t)};
}
template <typename T>
enable_if_iterable<T> tr1(T&& t) {
    return {std::forward<T>(t), -1};
}
template <typename T>
enable_if_scalar<T> tr1(int, T&& t) {
    return {std::forward<T>(t)};
}
template <typename T>
enable_if_iterable<T> tr1(int indent, T&& t) {
    return {std::forward<T>(t), indent};
}
} // namespace cbl::tr_internal

//==============================================================================
// Os& OPERATOR<<(Os&, T)
//==============================================================================

template <typename Os>
Os& operator<<(Os& os, cbl::tr_internal::comma) {
    os << ',';
    return os;
}

template <typename Os>
Os& operator<<(Os& os, cbl::tr_internal::newline) {
    os << '\n';
    return os;
}

template <typename Os>
Os& operator<<(Os& os, cbl::tr_internal::indent indent) {
    for (int i = 0; i < indent.indent; ++i) os << ' ';
    return os;
}

template <typename Os>
Os& operator<<(Os& os, cbl::tr_internal::endl) {
    os << cbl::tr_internal::newline{};
    os.flush();
    return os;
}

template <typename Os, typename T>
Os& operator<<(Os& os, cbl::tr_internal::scalar<T> const& scalar) {
    scalar.to_stream(os);
    return os;
}

template <typename Os, typename T>
Os& operator<<(Os& os, cbl::tr_internal::iterable<T> const& iterable) {
    iterable.to_stream(os);
    return os;
}

namespace cbl::tr_internal {
//------------------------------------------------------------------------------
//
template <typename T>
template <typename Os>
void scalar<T>::to_stream(Os& os) const {
    os << m_scalar;
}

//------------------------------------------------------------------------------
//
template <typename T>
template <typename Os>
void iterable<T>::to_stream(Os& os) const {
    const indent indent{m_indent};

    auto it  = m_iterable.begin();
    auto end = m_iterable.end();

    os << '{';
    if (it != end) {
        if (indent) os << newline{} << indent;
        os << tr1(m_indent, *it);
        ++it;
    }
    for (; it != end; ++it) {
        os << comma{};
        if (indent) os << newline{} << indent;
        if (!indent) os << ' ';
        os << tr1(m_indent, *it);
    }
    if (indent) os << newline{};
    os << '}';
}
} // namespace cbl::tr_internal

#endif
