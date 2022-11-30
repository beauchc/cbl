#include <cbl/io/tracing.h>

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
#define TR_OS os

#define EXPECT_TR_EQ(EXP, ...)                                   \
    {                                                            \
        std::stringstream ss;                                    \
        cbl::ostream      os{ss};                                \
        TR(__VA_ARGS__);                                         \
        EXPECT_EQ(STRINGIZE(__LINE__) ": " EXP "\n"s, ss.str()); \
    }

//==============================================================================
// TESTS
//==============================================================================

//------------------------------------------------------------------------------
//
TEST(tracing, TR) {
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
TEST(tracing, TRV) {
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
TEST(tracing, TRS) {
    EXPECT_TR_EQ(
        "==================================="
        "===================================",
        TRS());
    EXPECT_TR_EQ("==", TRS(2));
    EXPECT_TR_EQ("====", TRS(4));
}

//------------------------------------------------------------------------------
//
TEST(tracing, TRC) {
    TRC(int i = 42);
    EXPECT_EQ(42, i);
}

//------------------------------------------------------------------------------
//
inline auto TRI = R"(101: start
102: A
104: bbbb {
105:     B
107:     {
108:      C
107:     }
110:     D
112:     eeee {
113:       E
112:     }
115:     F
104: }
117: G
)"s;

TEST(ostream, TRI) {
    std::stringstream ss;
    cbl::ostream os{ss};

    TR("start");
    TR('A');
    {
        TRC(TRI("bbbb"));
        TR('B');
        {
            TRC(TRI(1));
            TR('C');
        }
        TR('D');
        {
            TRC(TRI("eeee", 2));
            TR('E');
        }
        TR('F');
    }
    TR('G');
    EXPECT_EQ(TRI, ss.str());
}
