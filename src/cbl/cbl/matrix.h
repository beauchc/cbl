#ifndef CBL_MATRIX_H
#define CBL_MATRIX_H

#include <array>
#include <bitset>
#include <iostream>
#include <vector>

namespace cbl {

//==============================================================================
// CLASS Matrix<T, N, M>
//==============================================================================

template <typename T, size_t HEIGHT = 0, size_t WIDTH = 0>
class Matrix {
public:
    Matrix() = default;

    decltype(auto) operator()(size_t i, size_t j) const { return m_mat[i][j]; }
    decltype(auto) operator()(size_t i, size_t j) { return m_mat[i][j]; }

    constexpr size_t height() const { return HEIGHT; }
    constexpr size_t width() const { return WIDTH; }

private:
    using RowT = std::conditional_t //
        <std::is_same_v<T, bool>, std::bitset<WIDTH>, std::array<T, WIDTH>>;

    template <typename U, size_t J, size_t K>
    friend std::ostream& operator<<(std::ostream& os, Matrix<U, J, K> const& m);

    std::array<RowT, HEIGHT> m_mat;
};

//------------------------------------------------------------------------------
//
template <typename T>
class Matrix<T, 0, 0> {
public:
    Matrix() = default;
    explicit Matrix(unsigned height, unsigned width)
        : m_mat(height * width), m_height(height), m_width(width) {}

    decltype(auto) operator()(size_t i, size_t j) const { return get(i, j); }
    decltype(auto) operator()(size_t i, size_t j) { return get(i, j); }

    decltype(auto) get(size_t i, size_t j) { return m_mat[idx(i, j)]; }
    decltype(auto) get(size_t i, size_t j) const { return m_mat[idx(i, j)]; }

    size_t height() const { return m_height; }
    size_t width() const { return m_width; }

private:
    auto idx(size_t i, size_t j) const { return i * m_height + j; }

    std::vector<T> m_mat;
    size_t         m_height = 0, m_width = 0;
};

//------------------------------------------------------------------------------
//
template <typename T, size_t HEIGHT, size_t WIDTH>
std::ostream& operator<<(std::ostream& os, Matrix<T, HEIGHT, WIDTH> const& m) {
    os << "{\n";
    for (size_t i = 0; i < m.height(); ++i) {
        os << "    ";
        for (size_t j = 0; j < m.width(); ++j) os << ' ' << m(i, j);
        os << '\n';
    }
    return os << "}";
}

} // namespace cbl

#endif
