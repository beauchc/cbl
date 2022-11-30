#ifndef CBL_IOMANIP_H
#define CBL_IOMANIP_H

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
    explicit pretty(int indent, int depth = -1)
        : m_indent(static_cast<unsigned>(indent)),
          m_depth(static_cast<unsigned>(depth)) {
        assert(indent >= 0);
        assert(depth >= -1);
    }
    explicit operator bool() const { return m_depth; }
    unsigned m_indent;
    unsigned m_depth;
};
} // namespace cbl::iomanip

#endif
