#ifndef CBL_IO_INTERNAL_SFINAE_H
#define CBL_IO_INTERNAL_SFINAE_H

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

#endif
