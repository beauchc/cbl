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
    template <typename Scoped>
    class scope;
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
// CLASS ostream::scope<Scoped>
//==============================================================================

template<typename Scoped>
class ostream::scope {
public:
    scope() = default;
    ~scope() = default;

    /// \brief Constructor
    explicit scope(ostream& os, Scoped scoped)
        : m_os(&os), m_scoped(std::move(scoped)) {}

    scope(scope&& o) noexcept {
        std::swap(m_os, o.m_os);
        std::swap(m_scoped, o.m_scoped);
    }
    scope& operator=(scope&& o) {
        std::swap(m_os, o.m_os);
        std::swap(m_scoped, o.m_scoped);
        return *this;
    }

    scope(scope const&) = delete;
    scope& operator=(scope const&) = delete;

protected:
    ostream* m_os = nullptr;
    Scoped   m_scoped{};
};

//==============================================================================
// CLASS ostream::indent_scope
//==============================================================================

class ostream::indent_scope : public scope<int> {
public:
    explicit indent_scope(ostream& os, int indent) : scope<int>(os, indent) {
        m_os->impl.add_indent(m_scoped);
    }
    ~indent_scope() {
        if (m_os) m_os->impl.add_indent(-m_scoped);
    }

    indent_scope() = default;
    indent_scope(indent_scope&& o) noexcept = default;
    indent_scope& operator=(indent_scope&& o) = default;
    indent_scope(indent_scope const&) = delete;
    indent_scope& operator=(indent_scope const&) = delete;
};

//==============================================================================
// CLASS ostream::prefix_scope
//==============================================================================

class ostream::prefix_scope : public scope<std::string_view>{
public:
    explicit prefix_scope(ostream& os, std::string_view prefix)
        : scope<std::string_view>(os, prefix) {
        m_os->impl.swap_prefix(m_scoped);
    }
    ~prefix_scope() {
        if (m_os) m_os->impl.swap_prefix(m_scoped);
    }

    prefix_scope() = default;
    prefix_scope(prefix_scope&& o) noexcept  = default;
    prefix_scope& operator=(prefix_scope&& o) noexcept =default;
    prefix_scope(prefix_scope const&) = delete;
    prefix_scope& operator=(prefix_scope const&) = delete;
};

//------------------------------------------------------------------------------
//
inline ostream::indent_scope ostream::make_indent_scope(int indent) & {
    return indent_scope{*this, indent};
}

} // namespace cbl

#endif
