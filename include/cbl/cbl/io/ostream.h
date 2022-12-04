#ifndef CBL_IO_OSTREAM_H
#define CBL_IO_OSTREAM_H

#include <cbl/io/internal/ostream_internal.h>

#include <ostream>

namespace cbl {

class StreamableBase;

class ostream;
inline auto endl = internal::endl_t{};

//==============================================================================
// CLASS ostream
//==============================================================================

class ostream {
    template <typename D, typename S>
    using scope = internal::ostream_scope<D, S>;

public:
    class indent_scope;
    class prefix_scope;

    explicit ostream(std::ostream& os) : impl{os} {}

    template <typename T>
    std::enable_if_t<!std::is_base_of_v<StreamableBase, T>, ostream&>
    operator<<(T const& t) {
        impl.toStream(t);
        return *this;
    }

    indent_scope make_indent_scope(int indent) &;
    indent_scope make_indent_scope(int indent) && = delete;

    prefix_scope make_prefix_scope(std::string_view prefix) &;
    prefix_scope make_prefix_scope(std::string_view prefix) && = delete;

private:
    internal::ostream_impl impl;
};

//==============================================================================
// CLASS ostream::indent_scope
//==============================================================================

class ostream::indent_scope : public scope<indent_scope, int> {
public:
    using scope<indent_scope, int>::scope;

private:
    friend scope<indent_scope, int>;
    void push_scope() { m_os->impl.add_indent(m_scoped); }
    void pop_scope() { m_os->impl.add_indent(-m_scoped); }
};

//==============================================================================
// CLASS ostream::prefix_scope
//==============================================================================

class ostream::prefix_scope : public scope<prefix_scope, std::string_view> {
public:
    using scope<prefix_scope, std::string_view>::scope;

private:
    friend scope<prefix_scope, std::string_view>;
    void push_scope() { m_os->impl.swap_prefix(m_scoped); }
    void pop_scope() { m_os->impl.swap_prefix(m_scoped); }
};

//------------------------------------------------------------------------------
//
inline ostream::indent_scope ostream::make_indent_scope(int indent) & {
    return indent_scope{*this, indent};
}
inline ostream::prefix_scope ostream::make_prefix_scope(
    std::string_view prefix) & {
    return prefix_scope{*this, prefix};
}

} // namespace cbl

#endif
