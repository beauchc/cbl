#include <cbl/io/iomanip.h>

#include <gtest/gtest.h>

#include "helpers.h"

using namespace std::string_literals;
using namespace test_io_helpers;

//==============================================================================
// TEST iomanip::pretty
//==============================================================================

//------------------------------------------------------------------------------
//
inline auto pretty_array_0_0 = "{{1, 2}, {3, 4}, {5, 6}}"s;

//------------------------------------------------------------------------------
//
inline auto pretty_array_0_1 = R"({
{1, 2},
{3, 4},
{5, 6}
})"s;
inline auto pretty_array_2_1 = R"({
  {1, 2},
  {3, 4},
  {5, 6}
})"s;
inline auto pretty_array_4_1 = R"({
    {1, 2},
    {3, 4},
    {5, 6}
})"s;

//------------------------------------------------------------------------------
//
inline auto pretty_array_0_INF = R"({
{
1,
2
},
{
3,
4
},
{
5,
6
}
})"s;
inline auto pretty_array_2_INF = R"({
  {
    1,
    2
  },
  {
    3,
    4
  },
  {
    5,
    6
  }
})"s;
inline auto pretty_array_4_INF = R"({
    {
        1,
        2
    },
    {
        3,
        4
    },
    {
        5,
        6
    }
})"s;

//------------------------------------------------------------------------------
//
TEST(iomanip, pretty_array) {
    using namespace cbl::iomanip;

    using A = std::array<int, 2>;
    std::array<A, 3>        a1{A{1, 2}, A{3, 4}, A{5, 6}};
    std::array<A, 3>&       a2 = a1;
    std::array<A, 3> const& a3 = a1;

    EXPECT_EQ(pretty_array_0_0, str(pretty(0, 0), a2)); // &
    EXPECT_EQ(pretty_array_0_0, str(pretty(2, 0), a2)); // &
    EXPECT_EQ(pretty_array_0_0, str(pretty(4, 0), a2)); // &

    EXPECT_EQ(pretty_array_0_1, str(pretty(0, 1), a3)); // const &
    EXPECT_EQ(pretty_array_2_1, str(pretty(2, 1), a3)); // const &
    EXPECT_EQ(pretty_array_4_1, str(pretty(4, 1), a3)); // const &

    EXPECT_EQ(pretty_array_0_INF, str(pretty(0), a2));            // &
    EXPECT_EQ(pretty_array_2_INF, str(pretty(2), a3));            // const &
    EXPECT_EQ(pretty_array_4_INF, str(pretty(4), std::move(a1))); // &&
}

//------------------------------------------------------------------------------
//
inline auto pretty_map_0_0 = "{[2] = two, [4] = four, [9] = nine}"s;

//------------------------------------------------------------------------------
//
inline auto pretty_map_0_1 = R"({
[2] = two,
[4] = four,
[9] = nine
})"s;
inline auto pretty_map_2_1 = R"({
  [2] = two,
  [4] = four,
  [9] = nine
})"s;
inline auto pretty_map_4_1 = R"({
    [2] = two,
    [4] = four,
    [9] = nine
})"s;

TEST(iomanip, pretty_map) {
    using namespace cbl::iomanip;

    std::map<int, std::string> a1;
    a1[4] = "four";
    a1[9] = "nine";
    a1[2] = "two";

    std::map<int, std::string>&       a2 = a1;
    std::map<int, std::string> const& a3 = a1;

    EXPECT_EQ(pretty_map_0_0, str(pretty(0, 0), a2)); // &
    EXPECT_EQ(pretty_map_0_0, str(pretty(2, 0), a2)); // &
    EXPECT_EQ(pretty_map_0_0, str(pretty(4, 0), a2)); // &

    EXPECT_EQ(pretty_map_0_1, str(pretty(0, 1), a2));            // &
    EXPECT_EQ(pretty_map_2_1, str(pretty(2, 1), a3));            // const &
    EXPECT_EQ(pretty_map_4_1, str(pretty(4, 1), std::move(a1))); // &&
}

//==============================================================================
// TEST set_indent
//==============================================================================

TEST(iomanip, set_indent) {
    using namespace cbl::iomanip;

    std::stringstream ss;
    cbl::ostream      os{ss};

    os << set_indent{1} << 'A' << set_indent{2} << 'B' << cbl::endl;
    os << set_indent{3} << 'C' << set_indent{4} << 'D' << cbl::endl;
    os << set_indent{5} << 'E' << set_indent{0} << 'F' << cbl::endl;
    os << 'G' << cbl::endl;

    auto expected =
        " AB\n"
        "   CD\n"
        "     EF\n"
        "G\n"s;
    EXPECT_EQ(expected, ss.str());
}
