#ifndef CBL_SMALL_VECTOR_H
#define CBL_SMALL_VECTOR_H

#include <array>

namespace cbl {

//==============================================================================
// CLASS SmallVector<T,N>
//==============================================================================

template <typename T, unsigned N>
class SmallVector {
public:
    static_assert(std::is_trivially_destructible_v<T>);

    SmallVector() = default;

    template <typename... Args>
    void emplace_back(Args&&... args) {
        new (&m_vec[m_size++]) T(std::forward<Args>(args)...);
    }

    void push_back(T val) { m_vec[m_size++] = val; }
    void pop_back() { --m_size; }

    T const& operator[](unsigned i) const { return m_vec[i]; }
    T&       operator[](unsigned i) { return m_vec[i]; }

    T const& front() const { return m_vec[0]; }
    T&       front() { return m_vec[0]; }

    T const& back() const { return m_vec[m_size - 1]; }
    T&       back() { return m_vec[m_size - 1]; }

    T const* begin() const { return &m_vec[0]; }
    T*       begin() { return &m_vec[0]; }

    T const* end() const { return begin() + size(); }
    T*       end() { return begin() + size(); }

    unsigned size() const { return m_size; }

private:
    std::array<T, N> m_vec{};
    unsigned         m_size = 0;
};

} // namespace cbl

#endif
