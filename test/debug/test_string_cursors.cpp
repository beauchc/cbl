#include <cbl/debug/string_cursors.h>

#include <gtest/gtest.h>

#include <sstream>

using namespace std::string_literals;
using namespace std::string_view_literals;

//------------------------------------------------------------------------------
//
template<typename... Args>
std::string print_cursors(Args&&... args) {
    std::stringstream ss;
    ss << cbl::debug::make_string_cursors(std::forward<Args>(args)...);
    return ss.str();
}

//------------------------------------------------------------------------------
//
TEST(string_cursors, named) {
    auto s = "this is a test"sv;
    auto p = [s](unsigned i) { return &s[i]; };

    auto actual = print_cursors( //
        s, '0', p(0), '1', p(1), '8', p(8), 'E', p(s.size()), 'A', p(8), 'B',
        p(8), '6', p(6));

    auto expected =
        "\n"                 //
        "'this is a test'\n" //
        " 01    6 8     E\n" //
        "         A\n"       //
        "         B"s;
    EXPECT_EQ(expected, actual);
}

//------------------------------------------------------------------------------
//
TEST(string_cursors, autonamed) {
    auto s = "this is a test"sv;
    auto p = [s](unsigned i) { return &s[i]; };

    auto actual = print_cursors( //
        s, p(0), p(1), p(8), p(s.size()), p(8), p(8), p(6));

    auto expected =
        "\n"                 //
        "'this is a test'\n" //
        " AB    G C     D\n" //
        "         E\n"       //
        "         F"s;
    EXPECT_EQ(expected, actual);
}
