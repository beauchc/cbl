#include <cbl/matrix.h>

#include <gtest/gtest.h>

//------------------------------------------------------------------------------
//
namespace {
struct MyInt {
    explicit MyInt() noexcept = default;
    explicit MyInt(int i) : m_int(std::make_unique<int>(i)) {}

    MyInt(MyInt&& o) noexcept = default;
    MyInt& operator=(MyInt&& o) noexcept = default;

    MyInt(MyInt const& o) = delete;
    MyInt& operator=(MyInt const& o) = delete;

    std::unique_ptr<int> m_int;
};
bool operator==(MyInt const& lhs, MyInt const& rhs) {
    if (!lhs.m_int) return !rhs.m_int;
    if (!rhs.m_int) return false;
    return *lhs.m_int == *rhs.m_int;
}
} // namespace

//------------------------------------------------------------------------------
//
namespace {
template <typename T>
struct is_dynamic : public std::integral_constant<bool, false> {};
template <typename T>
struct is_dynamic<cbl::Matrix<T>> : public std::integral_constant<bool, true> {
};

template <typename T>
size_t size_of(T const&) {
    return sizeof(T);
}
template <typename T>
size_t size_of(cbl::Matrix<T> const& m) {
    return 0;
}

} // namespace

//------------------------------------------------------------------------------
//
namespace {
template <class T>
class matrix : public testing::Test {};
using types = ::testing::Types< //
    cbl::Matrix<bool, 2, 3>,
    cbl::Matrix<double, 2, 3>,
    cbl::Matrix<MyInt, 2, 3>,
    cbl::Matrix<bool>,
    cbl::Matrix<double>,
    cbl::Matrix<MyInt>
>;
TYPED_TEST_CASE(matrix, types);
} // namespace

//------------------------------------------------------------------------------
//
TYPED_TEST(matrix, tests) {
    using Mat = TypeParam;
    using T   = typename Mat::value_type;

    Mat mat;
    if constexpr (is_dynamic<Mat>::value) {
        mat = Mat(2, 3);
    }
    ASSERT_EQ(2, mat.height());
    ASSERT_EQ(3, mat.width());
    ASSERT_LE(size_of(mat), (2 * 3 * sizeof(T)));

    mat(0, 0) = T(0);
    mat(0, 1) = T(1);
    mat(0, 2) = T(2);
    mat(1, 0) = T(3);
    mat(1, 1) = T(4);
    mat(1, 2) = T(5);

    ASSERT_EQ(T(0), mat(0, 0));
    ASSERT_EQ(T(1), mat(0, 1));
    ASSERT_EQ(T(2), mat(0, 2));
    ASSERT_EQ(T(3), mat(1, 0));
    ASSERT_EQ(T(4), mat(1, 1));
    ASSERT_EQ(T(5), mat(1, 2));

    // const
    auto const& cmat = mat;
    ASSERT_EQ(2, cmat.height());
    ASSERT_EQ(3, cmat.width());

    ASSERT_EQ(T(0), cmat(0, 0));
    ASSERT_EQ(T(1), cmat(0, 1));
    ASSERT_EQ(T(2), cmat(0, 2));
    ASSERT_EQ(T(3), cmat(1, 0));
    ASSERT_EQ(T(4), cmat(1, 1));
    ASSERT_EQ(T(5), cmat(1, 2));
}
