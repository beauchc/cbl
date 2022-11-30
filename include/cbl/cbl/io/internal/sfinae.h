#ifndef CBL_IO_INTERNAL_SFINAE_H
#define CBL_IO_INTERNAL_SFINAE_H

#define CBL_SFINAE_TEST(NAME, RHS, CONST)                                    \
    template <typename U>                                                    \
    class NAME {                                                             \
        template <typename T>                                                \
        static std::true_type base(decltype(std::declval<T CONST&>().RHS)*); \
        template <typename T>                                                \
        static std::false_type base(...);                                    \
                                                                             \
    public:                                                                  \
        using type                  = decltype(base<U>(nullptr));            \
        static constexpr bool value = type::value;                           \
    }
#endif
