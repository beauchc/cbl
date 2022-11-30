#include <cbl/io/ostream.h>

#include <array>

#include <gtest/gtest.h>

#include "helpers.h"

//------------------------------------------------------------------------------
//
using namespace std::string_literals;
using namespace std::string_view_literals;
using namespace test_io_helpers;

//------------------------------------------------------------------------------
//
template <typename T>
class ostream : public testing::Test {};
using ostream_types = ::testing::Types<int, std::array<int, 3>>;
TYPED_TEST_CASE(ostream, ostream_types);

//==============================================================================
// TESTS
//==============================================================================

//------------------------------------------------------------------------------
//
TEST(tr, traits) {
    static_assert(is_iterable_v<std::array<int, 4>>);
    static_assert(is_iterable_v<std::array<int, 4>&>);
    static_assert(is_iterable_v<std::array<int, 4>&&>);
    static_assert(is_iterable_v<std::array<int, 4> const&>);
    static_assert(!is_iterable_v<int>);
    static_assert(!is_iterable_v<int&>);
    static_assert(!is_iterable_v<int&&>);
    static_assert(!is_iterable_v<int const&>);
}

//------------------------------------------------------------------------------
//
template <typename T>
struct helper {};
template <>
struct helper<int> {
    using type = int;
    static auto simple_expected() { return "42"s; };
    static auto simple_value() { return type(42); }
};
template <>
struct helper<std::array<int, 3>> {
    using type = std::array<int, 3>;
    static auto simple_expected() { return "{1, 2, 3}"s; };
    static auto simple_value() { return type{1, 2, 3}; }
};

//------------------------------------------------------------------------------
//
TYPED_TEST(ostream, simple) {
    using T = TypeParam;

    T        a1 = helper<T>::simple_value();
    T&       a2 = a1;
    T const& a3 = a1;

    auto expected = helper<T>::simple_expected();
    EXPECT_EQ(expected, str(a2));            // &
    EXPECT_EQ(expected, str(a3));            // const&
    EXPECT_EQ(expected, str(std::move(a1))); // &&
}

//------------------------------------------------------------------------------
//
inline auto indent_scope_expected = R"(
A
  B
  CDE
    FG
  H
      IJ
  K
L
)"s;

TEST(ostream, indent_scope) {
    std::stringstream ss;
    cbl::ostream os{ss};

    os << cbl::endl;
    os << 'A' << cbl::endl;
    {
        auto g1 = os.make_indent_scope(2);
        os << 'B' << cbl::endl;
        os << 'C';
        os << 'D';
        {
            auto g2 = os.make_indent_scope(2);
            os << 'E' << cbl::endl;
            os << 'F';
        }
        os << 'G' << cbl::endl;
        os << 'H' << cbl::endl;
        {
            auto g2 = os.make_indent_scope(4);
            os << 'I';
            os << 'J' << cbl::endl;
        }
        os << 'K' << cbl::endl;
    }
    os << 'L' << cbl::endl;

    EXPECT_EQ(indent_scope_expected, ss.str());
}
