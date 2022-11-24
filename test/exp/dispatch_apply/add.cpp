#include "add.h"

//------------------------------------------------------------------------------
//
namespace {
using size_type = Add::size_type;

template<typename T, typename U>
T to(U const& u) {
    return static_cast<T>(u);
}
template <typename T, typename U, typename V>
auto add1(T const& a, U const& b, V& c) {
    c = to<V>(a + to<T>(b));
}
template <typename T, typename V>
auto add1(T const& a, T const& b, V& c) {
    c = to<V>(a + b);
}

//------------------------------------------------------------------------------
//
template <typename T, typename U, typename V>
void add_impl(T const* a, U const* b, V* c, size_type n);
template <typename T, typename U, typename V>
void add_impl(T const& a, U const* b, V* c, size_type n);
template <typename T, typename U, typename V>
void add_impl(T const* a, U const& b, V* c, size_type n);

template <typename T, typename V>
void add_impl(T const& a, T const* b, V* c, size_type n) {
    for (size_type i = 0; i < n; ++i) add1(a, b[i], c[i]);
}
template <typename T, typename V>
void add_impl(T const* a, T const& b, V* c, size_type n) {
    for (size_type i = 0; i < n; ++i) add1(a[i], b, c[i]);
}

template <typename T, typename U, typename V>
void add_impl(T const* a, U const* b, V* c, size_type n) {
    for (size_type i = 0; i < n; ++i) add1(a[i], b[i], c[i]);
}
template <typename T, typename U, typename V>
void add_impl(T const& a, U const* b, V* c, size_type n) {
    add_impl(to<T>(a), b, c, n);
}
template <typename T, typename U, typename V>
void add_impl(T const* a, U const& b, V* c, size_type n) {
    add_impl(a, to<T>(b), c, n);
}

} // namespace

//------------------------------------------------------------------------------
//
#define ADD_DEFINE_OPS(T, U, V)                                             \
    void Add::operator()(T const* a, U const* b, V* c, size_type n) const { \
        add_impl(a, b, c, n);                                               \
    }                                                                       \
    void Add::operator()(T const& a, U const* b, V* c, size_type n) const { \
        add_impl(a, b, c, n);                                               \
    }                                                                       \
    void Add::operator()(T const* a, U const& b, V* c, size_type n) const { \
        add_impl(a, b, c, n);                                               \
    }
ADD_FOREACH_TYPE(ADD_DEFINE_OPS);
#undef ADD_DECL_OPS
