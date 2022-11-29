#include <cbl/tr/tr.h>

#include <array>
#include <vector>

#include <gtest/gtest.h>

//------------------------------------------------------------------------------
//
using namespace std::string_literals;
using namespace std::string_view_literals;

template <typename T>
constexpr bool is_iterable_v = cbl::tr_internal::is_iterable<T>::value;

template <typename T>
std::string str(T&& t) {
    using namespace cbl::tr_ns;
    std::stringstream ss;
    ss << tr(std::forward<T>(t));
    return ss.str();
}
template <typename T>
std::string str_nice(int indent, T&& t) {
    using namespace cbl::tr_ns;
    std::stringstream ss;
    ss << tr_nice(std::forward<T>(t), indent);
    return ss.str();
}

template <typename T>
class trT : public testing::Test {};
using trT_types = ::testing::Types<int, std::array<int, 3>>;
TYPED_TEST_CASE(trT, trT_types);

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
TYPED_TEST(trT, simple) {
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
TEST(tr, nice_array) {
    using namespace std::string_literals;

    std::array<int, 3>        a1{1, 2, 3};
    std::array<int, 3>&       a2 = a1;
    std::array<int, 3> const& a3 = a1;

    EXPECT_EQ("{\n1,\n2,\n3\n}"s, str_nice(0, a2));                  // &
    EXPECT_EQ("{\n 1,\n 2,\n 3\n}"s, str_nice(1, a3));               // const&
    EXPECT_EQ("{\n  1,\n  2,\n  3\n}"s, str_nice(2, std::move(a1))); // &&
}
