#ifndef CBL_IO_INTERNAL_TRACING_H
#define CBL_IO_INTERNAL_TRACING_H

#include <cbl/io/ostream.h>

#define CBL_INTERNAL_TR(OS, ...) cbl::TR_IMPL::trace{OS, __LINE__}(__VA_ARGS__)

#define CBL_INTERNAL_TRS(...) cbl::TR_IMPL::sep(__VA_ARGS__) // separator
#define CBL_INTERNAL_TRV(EXPR) #EXPR, " = ", (EXPR)          // var/expr

#define CBL_INTERNAL_TRC(code) code // code block
#define CBL_INTERNAL_TRI(OS, ...) \
    cbl::TR_IMPL::tri cbl_TR_guard##__LINE__(OS, __LINE__, __VA_ARGS__)

//==============================================================================
// NAMESPACE cbl::TR_IMPL
//==============================================================================

namespace cbl::TR_IMPL {

//------------------------------------------------------------------------------
//
auto sep(unsigned size = 70) { return cbl::iomanip::line{size, '='}; }

//------------------------------------------------------------------------------
//
struct trace {
    trace(cbl::ostream& os, int line) : os(os), line(line) {}

    template <typename... Args>
    void operator()(Args&&... args) const {
        os << line << " : ";
        ((os << args), ...);
        os << cbl::endl;
    }

    cbl::ostream& os;
    int line;
};

//------------------------------------------------------------------------------
//
struct tri {
    constexpr static int s_default = 4;

    tri(cbl::ostream& os, int line, char const* name, int indent)
        : m_line(line) {
        trace{os, line}(name, (name[0] ? " {" : "{"));
        m_scope = os.make_indent_scope(indent);
    }
    tri(cbl::ostream& os, int line, char const* name)
        : tri(os, line, name, s_default) {}
    tri(cbl::ostream& os, int line, int indent) : tri(os, line, "", indent) {}
    tri(cbl::ostream& os, int line) : tri(os, line, "", s_default) {}

    ~tri() {
        cbl::ostream& os = m_scope.os();
        m_scope = cbl::ostream::indent_scope();
        trace{os, m_line}('}');
    }

    cbl::ostream::indent_scope m_scope;
    int m_line;
};

} // namespace cbl::TR_IMPL

#endif
