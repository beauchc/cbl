#include "dispatch_apply.h"

#include <gtest/gtest.h>

//==============================================================================
// TESTS
//==============================================================================

template <typename S, typename Out, typename F, typename In1, typename In2>
auto test_apply(F&& f, In1& i1, In2& i2) {
    size_t grain = 3;
    Out    out;
    apply<S>(std::forward<F>(f), grain, Iterator{&i1}, Iterator{&i2},
             Iterator{&out});
    return out;
}

template <typename int_or_float, typename Ints_or_Flts>
void test() {
    using T    = int_or_float;
    using Outs = Ints_or_Flts;

    auto in1 = Ints{+10, +20, +30, +40};
    auto in2 = Flts{1.f, 2.f, 3.f, 4.f};

    auto add = [](auto a, auto b, auto& c) { c = a + b; };

    ASSERT_EQ((Outs{20, 40, 60, 80}),
              (test_apply<Signature<int, int, T&>, Outs>(add, in1, in1)));
    ASSERT_EQ((Outs{2, 4, 6, 8}),
              (test_apply<Signature<int, int, T&>, Outs>(add, in2, in2)));
    ASSERT_EQ((Outs{11, 22, 33, 44}),
              (test_apply<Signature<int, int, T&>, Outs>(add, in1, in2)));
    ASSERT_EQ((Outs{11, 22, 33, 44}),
              (test_apply<Signature<int, int, T&>, Outs>(add, in2, in1)));

    ASSERT_EQ((Outs{20, 40, 60, 80}),
              (test_apply<Signature<float, float, T&>, Outs>(add, in1, in1)));
    ASSERT_EQ((Outs{2, 4, 6, 8}),
              (test_apply<Signature<float, float, T&>, Outs>(add, in2, in2)));
    ASSERT_EQ((Outs{11, 22, 33, 44}),
              (test_apply<Signature<float, float, T&>, Outs>(add, in1, in2)));
    ASSERT_EQ((Outs{11, 22, 33, 44}),
              (test_apply<Signature<float, float, T&>, Outs>(add, in2, in1)));

    ASSERT_EQ((Outs{20, 40, 60, 80}),
              (test_apply<Signature<float, int, T&>, Outs>(add, in1, in1)));
    ASSERT_EQ((Outs{2, 4, 6, 8}),
              (test_apply<Signature<float, int, T&>, Outs>(add, in2, in2)));
    ASSERT_EQ((Outs{11, 22, 33, 44}),
              (test_apply<Signature<float, int, T&>, Outs>(add, in1, in2)));
    ASSERT_EQ((Outs{11, 22, 33, 44}),
              (test_apply<Signature<float, int, T&>, Outs>(add, in2, in1)));

    ASSERT_EQ((Outs{20, 40, 60, 80}),
              (test_apply<Signature<int, float, T&>, Outs>(add, in1, in1)));
    ASSERT_EQ((Outs{2, 4, 6, 8}),
              (test_apply<Signature<int, float, T&>, Outs>(add, in2, in2)));
    ASSERT_EQ((Outs{11, 22, 33, 44}),
              (test_apply<Signature<int, float, T&>, Outs>(add, in1, in2)));
    ASSERT_EQ((Outs{11, 22, 33, 44}),
              (test_apply<Signature<int, float, T&>, Outs>(add, in2, in1)));
}

//------------------------------------------------------------------------------
//
TEST(Test, dispatch_apply) {
    test<int, Ints>();
    test<int, Flts>();
    test<float, Ints>();
    test<float, Flts>();
}
