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
    class prefix_scope;

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
    indent_scope() = default;

    /// \brief Constructor
    explicit indent_scope(ostream& os, int indent)
        : m_os(&os), m_scoped(indent) {
        m_os->impl.add_indent(m_scoped);
    }

    /// \brief Destructor
    ~indent_scope() {
        if (m_os) m_os->impl.add_indent(-m_scoped);
    }

    indent_scope(indent_scope&& o) noexcept {
        std::swap(m_os, o.m_os);
        std::swap(m_scoped, o.m_scoped);
    }
    indent_scope& operator=(indent_scope&& o) {
        std::swap(m_os, o.m_os);
        std::swap(m_scoped, o.m_scoped);
        return *this;
    }

    indent_scope(indent_scope const&) = delete;
    indent_scope& operator=(indent_scope const&) = delete;

private:
    ostream* m_os     = nullptr;
    int      m_scoped = 0;
};

//==============================================================================
// CLASS ostream::prefix_scope
//==============================================================================

class ostream::prefix_scope {
public:
    prefix_scope() = default;

    /// \brief Constructor
    explicit prefix_scope(ostream& os, std::string_view prefix)
        : m_os(&os), m_scoped(prefix) {
        m_os->impl.swap_prefix(m_scoped);
    }

    /// \brief Destructor
    ~prefix_scope() {
        if (m_os) m_os->impl.swap_prefix(m_scoped);
    }

    prefix_scope(prefix_scope&& o) noexcept {
        std::swap(m_os, o.m_os);
        std::swap(m_scoped, o.m_scoped);
    }
    prefix_scope& operator=(prefix_scope&& o) noexcept {
        std::swap(m_os, o.m_os);
        std::swap(m_scoped, o.m_scoped);
        return *this;
    }

    prefix_scope(prefix_scope const&) = delete;
    prefix_scope& operator=(prefix_scope const&) = delete;

private:
    ostream*         m_os = nullptr;
    std::string_view m_scoped;
};

//------------------------------------------------------------------------------
//
inline ostream::indent_scope ostream::make_indent_scope(int indent) & {
    return indent_scope{*this, indent};
}

} // namespace cbl

#endif
