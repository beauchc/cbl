#include "iapply.h"

#include "iaccessor_factory.h"

#include <gtest/gtest.h>

//==============================================================================
// TESTS
//==============================================================================

TEST(iapply, tests) {
    float vff = 42.5f;
    int   vii = 74;
    int   vfi = static_cast<int>(vff);
    int   vif = static_cast<float>(vii);

    float o1 = 0, o2 = 0;

    std::array<IAccessorPtr, 2> inputs = //
        {IAccessorFactory::make_accessor(vff),
         IAccessorFactory::make_accessor(vii)};

    auto callbacks = std::make_tuple( //
        [&o1](auto a, auto b) { o1 = a + b; },
        [&o2](auto a, auto b) { o2 = a * b; });

    o1 = o2 = 0;
    apply<Signature<int, int>>(callbacks, inputs);
    EXPECT_EQ(vfi + vii, o1);
    EXPECT_EQ(vfi * vii, o2);

    o1 = o2 = 0;
    apply<Signature<float, float>>(callbacks, inputs);
    EXPECT_EQ(vff + vif, o1);
    EXPECT_EQ(vff * vif, o2);

    o1 = o2 = 0;
    apply<Signature<int, float>>(callbacks, inputs);
    EXPECT_EQ(vfi + vif, o1);
    EXPECT_EQ(vfi * vif, o2);

    o1 = o2 = 0;
    apply<Signature<float, int>>(callbacks, inputs);
    EXPECT_EQ(vff + vii, o1);
    EXPECT_EQ(vff * vii, o2);
}
