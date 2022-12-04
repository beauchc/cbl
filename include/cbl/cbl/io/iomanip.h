#ifndef CBL_IO_IOMANIP_H
#define CBL_IO_IOMANIP_H

#include <iomanip>
#include <cassert>

namespace cbl::iomanip {

//==============================================================================
// STRUCT set_indent
//==============================================================================

struct set_indent {
    explicit set_indent(int indent) : m_indent(static_cast<unsigned>(indent)) {
        assert(indent >= 0);
    }
    explicit operator bool() const { return m_indent; }
    unsigned m_indent;
};

//==============================================================================
// STRUCT pretty
//==============================================================================

struct pretty {
    explicit pretty(int indent, int depth)
        : m_indent(static_cast<unsigned>(indent)),
          m_depth(static_cast<unsigned>(depth)) {
        assert(indent >= 0);
        assert(depth >= -1);
    }
    explicit pretty(int indent) : pretty(indent, -1) {}
    explicit pretty() : pretty(4, -1) {}

    explicit operator bool() const { return m_depth; }
    unsigned m_indent;
    unsigned m_depth;
};

//==============================================================================
// STRUCT line
//==============================================================================

struct line {
    explicit line(int size, char c) : m_size(size), m_c(c) { assert(size > 0); }
    unsigned m_size;
    char     m_c;
};

//==============================================================================
// STRUCT set_prefix
//==============================================================================

struct set_prefix {
    explicit set_prefix(std::string_view prefix) : m_prefix{prefix} {}
    std::string_view m_prefix;
};

} // namespace cbl::iomanip

#endif
