#ifndef CBL_MATRIX_H
#define CBL_MATRIX_H

#include <cbl/span.h>

#include <array>
#include <vector>

namespace cbl {

//==============================================================================
// CLASS Matrix<T, HEIGHT, WIDTH>
//==============================================================================

template <typename T, size_t HEIGHT = 0, size_t WIDTH = 0>
class Matrix {
public:
    using value_type = T;
    using size_type  = size_t;

    class row_type {
        std::array<T, WIDTH> m_row;

    public:
        T const& operator[](size_type i) const { return m_row[i]; }
        T&       operator[](size_type i) { return m_row[i]; }
    };

    constexpr Matrix() = default;

    constexpr Matrix(Matrix&& o) noexcept = default;
    constexpr Matrix(Matrix const& o)     = default;

    constexpr Matrix& operator=(Matrix&& o) noexcept = default;
    constexpr Matrix& operator=(Matrix const& o) = default;

    constexpr row_type const& operator[](size_type i) const { return m_mat[i]; }
    constexpr row_type&       operator[](size_type i) { return m_mat[i]; }

    constexpr T const& operator()(size_type i, size_type j) const {
        return (*this)[i][j];
    }
    constexpr T& operator()(size_type i, size_type j) { return (*this)[i][j]; }

    constexpr size_type height() const { return HEIGHT; }
    constexpr size_type width() const { return WIDTH; }

    constexpr T const*  data() const { return &m_mat[0][0]; }
    constexpr size_type size() const { return HEIGHT * WIDTH; }

private:
    std::array<row_type, HEIGHT> m_mat{};
    static_assert(sizeof(m_mat) == sizeof(T) * HEIGHT * WIDTH);
};

//------------------------------------------------------------------------------
//
template <typename T>
class Matrix<T, 0, 0> {
public:
    using value_type = T;
    using size_type  = size_t;

    Matrix() = default;
    Matrix(size_t height, size_t width)
        : m_mat(height * width), // LCOV_EXCL_BR_LINE
          m_height(height),
          m_width(width) {}

    Matrix(Matrix&& o) noexcept = default;
    Matrix(Matrix const& o)     = default;

    Matrix& operator=(Matrix&& o) noexcept = default;
    Matrix& operator=(Matrix const& o) = default;

    cbl::Span<T const> operator[](size_type i) const {
        assert(i < m_height);
        return {data() + (i * m_width), m_width};
    }
    cbl::Span<T> operator[](size_type i) {
        assert(i < m_height);
        return {data() + (i * m_width), m_width};
    }
    size_t height() const { return m_height; }
    size_t width() const { return m_width; }

    size_type size() const { return m_width * m_height; }

private:
    using E = std::conditional_t<std::is_same_v<T, bool>, char, T>;
    static_assert(sizeof(char) == sizeof(bool));

    T const* data() const { return m_mat.data(); }
    T*       data() { return m_mat.data(); }

    std::vector<E> m_mat;
    size_t         m_height = 0, m_width = 0;
};

template <>
bool const* Matrix<bool, 0, 0>::data() const {
    return reinterpret_cast<bool const*>(m_mat.data());
}
template <>
bool* Matrix<bool, 0, 0>::data() {
    return reinterpret_cast<bool*>(m_mat.data());
}

} // namespace cbl

//------------------------------------------------------------------------------
//
// LCOV_EXCL_BR_START
template <typename Os, typename T, size_t H, size_t W>
Os& operator<<(Os& os, cbl::Matrix<T, H, W> const& m) {
    os << "Matrix{";
    if (m.height()) {
        os << '{';
        if (m.width()) os << m[0][0];
        for (size_t j = 1; j < m.width(); ++j) os << ", " << m[0][j];
        os << '}';
    }
    for (size_t i = 1; i < m.height(); ++i) {
        os << ", {";
        if (m.width()) os << m[i][0];
        for (size_t j = 1; j < m.width(); ++j) os << ", " << m[i][j];
        os << "}";
    }
    os << "}";
    return os;
}
// LCOV_EXCL_BR_STOP

#endif
