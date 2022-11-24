#ifndef CBL_MATRIX_H
#define CBL_MATRIX_H

#include <array>
#include <bitset>
#include <vector>

namespace cbl {

//==============================================================================
// CLASS Matrix<T, N, M>
//==============================================================================

template <typename T, size_t HEIGHT = 0, size_t WIDTH = 0>
class Matrix {
public:
    using value_type = T;
    using size_type  = size_t;

    constexpr Matrix() = default;

    constexpr Matrix(Matrix&& o) noexcept = default;
    constexpr Matrix(Matrix const& o)     = default;

    constexpr Matrix& operator=(Matrix&& o) noexcept = default;
    constexpr Matrix& operator=(Matrix const& o) = default;

    constexpr decltype(auto) operator()(size_type i, size_type j) const {
        return m_mat[i * WIDTH + j];
    }
    constexpr decltype(auto) operator()(size_type i, size_type j) {
        return m_mat[i * WIDTH + j];
    }

    constexpr size_type height() const { return HEIGHT; }
    constexpr size_type width() const { return WIDTH; }

private:
    static constexpr bool use_bitset = //
        std::is_same_v<T, bool> &&     //
        (sizeof(std::bitset<HEIGHT * WIDTH>) <
         sizeof(std::array<T, HEIGHT * WIDTH>));

    using underlying_type = std::conditional_t< //
        use_bitset,
        std::bitset<HEIGHT * WIDTH>,
        std::array<T, HEIGHT * WIDTH>>;

    underlying_type m_mat;
};

//------------------------------------------------------------------------------
//
template <typename T>
class Matrix<T, 0, 0> {
public:
    using value_type = T;

    Matrix() = default;
    Matrix(size_t height, size_t width)
        : m_mat(height * width), m_height(height), m_width(width) {}

    Matrix(Matrix&& o) noexcept = default;
    Matrix(Matrix const& o)     = default;

    Matrix& operator=(Matrix&& o) noexcept = default;
    Matrix& operator=(Matrix const& o) = default;

    decltype(auto) operator()(size_t i, size_t j) const {
        return m_mat[i * m_width + j];
    }
    decltype(auto) operator()(size_t i, size_t j) {
        return m_mat[i * m_width + j];
    }
    size_t height() const { return m_height; }
    size_t width() const { return m_width; }

private:
    std::vector<T> m_mat;
    size_t         m_height = 0, m_width = 0;
};

//------------------------------------------------------------------------------
//
template <typename Os, typename T, size_t HEIGHT, size_t WIDTH>
Os& operator<<(Os& os, Matrix<T, HEIGHT, WIDTH> const& m) {
    os << "Matrix{\n";
    for (size_t i = 0; i < m.height(); ++i) {
        os << "    ";
        for (size_t j = 0; j < m.width(); ++j) os << ' ' << m(i, j);
        os << '\n';
    }
    return os << "}";
}

} // namespace cbl

#endif
