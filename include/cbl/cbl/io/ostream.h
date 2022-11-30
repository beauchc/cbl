#ifndef CBL_IO_OSTREAM_H
#define CBL_IO_OSTREAM_H

#include <cbl/io/internal/ostream_internal.h>

#include <ostream>

namespace cbl {

class ostream;
inline auto endl = internal::endl_t{};

//==============================================================================
// CLASS ostream
//==============================================================================

class ostream {
public:
    class indent_scope;

    explicit ostream(std::ostream& os) : impl{os} {}

    template <typename T>
    ostream& operator<<(T const& t) {
        impl.toStream(t);
        return *this;
    }

    indent_scope make_indent_scope(int indent) &;
    indent_scope make_indent_scope(int indent) && = delete;

private:
    internal::ostream_impl impl;
};

//==============================================================================
// CLASS ostream::indent_scope
//==============================================================================

class ostream::indent_scope {
public:
    /// \brief Constructor
    explicit indent_scope(ostream& os, int indent)
        : m_os(&os), m_indent(indent) {
        m_os->impl.add_indent(m_indent);
    }

    /// \brief Move constructor
    indent_scope(indent_scope&& o) noexcept
        : m_os(o.m_os), m_indent(o.m_indent) {
        o.m_os = nullptr;
    }

    /// \brief Move assign operator
    indent_scope& operator=(indent_scope&& o) noexcept {
        m_os     = o.m_os;
        m_indent = o.m_indent;
        o.m_os   = nullptr;
        return *this;
    }

    /// \brief indent_scope is not copiable
    indent_scope(indent_scope const&) = delete;

    /// \brief indent_scope is not copy assignable
    indent_scope& operator=(indent_scope const&) = delete;

    /// \brief Destructor
    ~indent_scope() {
        if (m_os) m_os->impl.add_indent(-m_indent);
    }

private:
    ostream* m_os;
    int      m_indent;
};

//------------------------------------------------------------------------------
//
inline ostream::indent_scope ostream::make_indent_scope(int indent) & {
    return indent_scope{*this, indent};
}

} // namespace cbl

#endif
