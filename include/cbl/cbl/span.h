#ifndef CBL_SPAN_H
#define CBL_SPAN_H

#include <cassert>
#include <cstddef>

namespace cbl {

//==============================================================================
// CLASS Span<T>
//==============================================================================

template <typename T>
class Span {
public:
    constexpr Span() = default;
    constexpr Span(T* begin, T* end) : m_bgn(begin), m_end(end) {
        assert(m_bgn <= m_end);
    }
    constexpr Span(T* begin, size_t size) : Span(begin, begin + size) {}

    constexpr T const& operator[](size_t i) const { return deref(m_bgn + i); }
    constexpr T&       operator[](size_t i) /* */ { return deref(m_bgn + i); }

    constexpr T const& front() const { return deref(m_bgn); }
    constexpr T&       front() /* */ { return deref(m_bgn); }

    constexpr T const& back() const { return deref(m_end - 1); }
    constexpr T&       back() /* */ { return deref(m_end - 1); }

    constexpr T const* cbegin() const { return m_bgn; }
    constexpr T const* begin() const { return m_bgn; }
    constexpr T*       begin() /* */ { return m_bgn; }

    constexpr T const* cend() const { return m_end; }
    constexpr T const* end() const { return m_end; }
    constexpr T*       end() /* */ { return m_end; }

    constexpr size_t size() const { return m_end - m_bgn; }
    constexpr bool   empty() const { return m_bgn == m_end; }

    // clang-format off
    constexpr void advance_begin(int n) { m_bgn += n; assert(m_bgn <= m_end); }
    constexpr void advance_end(int n)   { m_end += n; assert(m_bgn <= m_end); }
    // clang-format on

private:
    template <typename U>
    constexpr U& deref(U* p) const {
        assert(m_bgn <= p && p < m_end);
        return *p;
    }

    T* m_bgn = nullptr;
    T* m_end = nullptr;
};

//==============================================================================
// FUNCTION make_span
//==============================================================================

template <typename T>
Span<T> make_span(T* data, unsigned size) {
    return Span<T>(data, size);
}
template <typename T>
Span<T> make_span(T* begin, T* end) {
    return Span<T>(begin, end);
}

} // namespace cbl

#endif
