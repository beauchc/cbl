#include "iaccessor.h"
#include "iaccessor_factory.h"
#include "iapply.h"

#include <gtest/gtest.h>

//==============================================================================
// TESTS
//==============================================================================

//------------------------------------------------------------------------------
//
TEST(iaccessor, tests) {
    constexpr float vff = 42.5f;
    constexpr int   vii = 74;

    auto acc1 = IAccessorFactory::make_accessor(vff);
    auto acc2 = IAccessorFactory::make_accessor(vii);

    constexpr auto vfi = static_cast<int>(vff);
    constexpr auto vif = static_cast<float>(vii);

    EXPECT_EQ(vfi + vii, (*acc1->get<int>() + *acc2->get<int>()));
    EXPECT_EQ(vff + vif, (*acc1->get<float>() + *acc2->get<float>()));
    EXPECT_EQ(vfi + vif, (*acc1->get<int>() + *acc2->get<float>()));
    EXPECT_EQ(vff + vii, (*acc1->get<float>() + *acc2->get<int>()));
}

//------------------------------------------------------------------------------
//
TEST(iapply, tests) {
    constexpr float vff = 42.5f;
    constexpr int   vii = 74;

    std::array inputs //
        {IAccessorFactory::make_accessor(vff),
         IAccessorFactory::make_accessor(vii)};

    float o1 = 0, o2 = 0;
    auto  callbacks = make_callbacks( //
        [&o1](auto a, auto b) { o1 = a + b; },
        [&o2](auto a, auto b) { o2 = a * b; });

    constexpr auto vfi = static_cast<int>(vff);
    constexpr auto vif = static_cast<float>(vii);

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
