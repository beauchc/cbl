#ifndef ADD_H
#define ADD_H

// clang-format off
#define ADD_FOREACH_TYPE_(MACRO) \
    MACRO(   int,   int,   int ) \
    MACRO(   int,   int, float ) \
    MACRO(   int, float,   int ) \
    MACRO(   int, float, float ) \
    MACRO( float,   int,   int ) \
    MACRO( float,   int, float ) \
    MACRO( float, float,   int ) \
    MACRO( float, float, float )
// clang-format on

#define ADD_FOREACH_TYPE(MACRO) \
    ADD_FOREACH_TYPE_(MACRO)    \
    static_assert(true)

struct Add {
    using size_type = unsigned long;
#define ADD_DECL_OPS(T, U, V)                                         \
    void operator()(T const* a, U const* b, V* c, size_type n) const; \
    void operator()(T const& a, U const* b, V* c, size_type n) const; \
    void operator()(T const* a, U const& b, V* c, size_type n) const;
    ADD_FOREACH_TYPE(ADD_DECL_OPS);
#undef ADD_DECL_OPS
};

#endif
