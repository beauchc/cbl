#include <cbl/tr/dbg.h>

#include <array>
#include <vector>

#include <gtest/gtest.h>

//------------------------------------------------------------------------------
//
using namespace std::string_literals;
using namespace std::string_view_literals;

//------------------------------------------------------------------------------
//
#define STRINGIZE2(x) #x
#define STRINGIZE(x) STRINGIZE2(x)

#undef TR_OS
#define TR_OS ss

#define EXPECT_TR_EQ(EXP, ...)                                    \
    {                                                             \
        std::stringstream ss;                                     \
        TR(__VA_ARGS__);                                          \
        EXPECT_EQ(STRINGIZE(__LINE__) " : " EXP "\n"s, ss.str()); \
    }

//==============================================================================
// TESTS
//==============================================================================

//------------------------------------------------------------------------------
//
TEST(tr_dbg, simple) {
    std::array<int, 3>        a1 = {1, 2, 3};
    std::array<int, 3>&       a2 = a1;
    std::array<int, 3> const& a3 = a1;

#define A123 "{1, 2, 3}"
    EXPECT_TR_EQ(A123, a2);
    EXPECT_TR_EQ(A123, a3);
    EXPECT_TR_EQ(A123, std::move(a1));
#undef A123
}

//------------------------------------------------------------------------------
//
TEST(tr_dbg, trv) {
    std::array<int, 3>        a1 = {1, 2, 3};
    std::array<int, 3>&       a2 = a1;
    std::array<int, 3> const& a3 = a1;

#define A123 "{1, 2, 3}"
    EXPECT_TR_EQ("a2 = " A123, TRV(a2));
    EXPECT_TR_EQ("a3 = " A123, TRV(a3));
    EXPECT_TR_EQ("std::move(a1) = " A123, TRV(std::move(a1)));
#undef A123
}

//------------------------------------------------------------------------------
//
TEST(tr_dbg, trs) {
    EXPECT_TR_EQ(
        "==================================="
        "===================================",
        TRS());
    EXPECT_TR_EQ("==", TRS(2));
    EXPECT_TR_EQ("====", TRS(4));
}

//------------------------------------------------------------------------------
//
TEST(tr_dbg, trc) {
    TRC(int i = 42);
    EXPECT_EQ(42, i);
}

//------------------------------------------------------------------------------
//
TEST(tr_dbg, tri) {
    /// \todo Test TRI
}
