#include <any>
#include <tuple>
#include <utility>
#include <variant>

#include <gtest/gtest.h>

//------------------------------------------------------------------------------

template <typename Fn>
struct Callable;

template <typename Ret, typename... Args>
struct Callable<Ret (*)(Args...)> {
    using return_type = Ret;
    static constexpr auto num_args = sizeof...(Args);

    using Fn = Ret (*)(Args...);
    Ret operator()(Args... args) { return m_fn(std::forward<Args>(args)...); }
    Fn  m_fn;
};

template <typename... Fns>
struct Overload;

template <>
struct Overload<> {};

template <typename Fn, typename... Fns>
struct Overload<Fn, Fns...> : public Callable<Fn>, Overload<Fns...> {
    Overload(Fn fn, Fns... fns)
        : Callable<Fn>{std::forward<Fn>(fn)},
          Overload<Fns...>(std::forward<Fns>(fns)...) {}
};

template <typename... Fns>
auto make_overload(Fns&&... fns) {
    return Overload<Fns...>(std::forward<Fns>(fns)...);
}

//------------------------------------------------------------------------------

template<typename Fn>
struct GetFnRet;

template <typename Ret, typename... Args>
struct GetFnRet<Ret (*)(Args...)> {
    using type = Ret;
};

template<int idx, typename Fn>
struct GetFnArg;

template<typename Ret, typename Arg, typename... Args>
struct GetFnArg<0, Ret(*)(Arg, Args...)> {
    using type = Arg;
};

template<int idx, typename Ret, typename Arg, typename... Args>
struct GetFnArg<idx, Ret(*)(Arg, Args...)> {
    using type = typename GetFnArg<idx-1, Ret(*)(Args...)>::type;
};

template <int idx, typename... Overload>
struct ArgTypes {};

template<typename Overload>
struct TransposedOverload;

template<typename Ov, typename... Ovs>
struct TransposedOverload<Overload<Ov, Ovs...>> {

};

//------------------------------------------------------------------------------

int foo(int arg, int v) { return arg + v; }
int foo(float arg, int v) { return static_cast<int>(arg) + v; }
auto foo_overload() {
    return make_overload(static_cast<int (*)(int, int)>(&foo),
                         static_cast<int (*)(float, int)>(&foo));
}

std::pair<bool, int> poly_foo(std::any any, int v) {
    if (auto arg = std::any_cast<int>(&any)) return {true, foo(*arg, v)};
    if (auto arg = std::any_cast<float>(&any)) return {true, foo(*arg, v)};
    return {false, {}};
}

TEST(test, test) {
    std::variant<int, float> bla = 1;

    bool is_int = std::visit(
        [](auto t) { return std::is_same_v<decltype(t), int>; }, bla);
    EXPECT_TRUE(is_int);
}
