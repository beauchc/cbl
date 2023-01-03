#include "any.h"

#include <gtest/gtest.h>

//==============================================================================
// TESTS
//==============================================================================

//------------------------------------------------------------------------------
//
TEST(any, tests) {
    Any any(42);
    EXPECT_EQ(nullptr, any.as<float>());
    ASSERT_NE(nullptr, any.as<int>());
    EXPECT_EQ(42, *any.as<int>());
}

//------------------------------------------------------------------------------
//
struct Visitor {
    using payload_types = PayloadTypes<int, float>;
    int operator()(int v) const { return v; }
    int operator()(float v) const { return static_cast<int>(v); }
    int operator()(Any const&) const { return -1; }
};

TEST(any, visit) {
    Any any1(42);
    Any any2(1043.f);
    Any any3('A');

    auto v1 = visit(Visitor(), any1);
    auto v2 = visit(Visitor(), any2);
    auto v3 = visit(Visitor(), any3);
    EXPECT_EQ(42, v1);
    EXPECT_EQ(1043, v2);
    EXPECT_EQ(-1, v3);
}
