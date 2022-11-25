#include "iaccessor.h"

#include "iaccessor_factory.h"

#include <gtest/gtest.h>

//==============================================================================
// TESTS
//==============================================================================

TEST(iaccessor, tests) {
    auto acc1 = IAccessorFactory::make_accessor(42.5f);
    auto acc2 = IAccessorFactory::make_accessor(74);

    EXPECT_EQ((42 + 74), (*acc1->get<int>() + *acc2->get<int>()));
    EXPECT_EQ((42.5f + 74.f), (*acc1->get<float>() + *acc2->get<float>()));
}
