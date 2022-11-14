#ifndef CBL_SPAN_H
#define CBL_SPAN_H

namespace cbl {

//==============================================================================
// CLASS Span<T>
//==============================================================================

template <typename T>
class Span {
public:
    Span(T* begin, unsigned size) : m_begin(begin), m_size(size) {}

    T const& operator[](unsigned i) const { return m_begin[i]; }
    T&       operator[](unsigned i) { return m_begin[i]; }

    T const& front() const { return m_begin[0]; }
    T&       front() { return m_begin[0]; }

    T const& back() const { return m_begin[m_size - 1]; }
    T&       back() { return m_begin[m_size - 1]; }

    T const* begin() const { return m_begin; }
    T*       begin() { return m_begin; }

    T const* end() const { return begin() + size(); }
    T*       end() { return begin() + size(); }

    unsigned size() const { return m_size; }

private:
    T*       m_begin;
    unsigned m_size;
};

} // namespace cbl

#endif
