#ifndef CBL_IO_INTERNAL_TRACING_H
#define CBL_IO_INTERNAL_TRACING_H

#include <cbl/io/ostream.h>

#include <iostream>

#define CBL_INTERNAL_TR_STRINGIZE2(x) #x
#define CBL_INTERNAL_TR_STRINGIZE(x) CBL_INTERNAL_TR_STRINGIZE2(x)

#define CBL_INTERNAL_TR_PREFIX CBL_INTERNAL_TR_STRINGIZE(__LINE__) ": "

#define CBL_INTERNAL_TR(OS, ...) \
    cbl::TR_IMPL::trace{OS, CBL_INTERNAL_TR_PREFIX}(__VA_ARGS__)

#define CBL_INTERNAL_TRS(...) cbl::TR_IMPL::sep(__VA_ARGS__) // separator
#define CBL_INTERNAL_TRV(EXPR) #EXPR, " = ", (EXPR)          // var/expr

#define CBL_INTERNAL_TRC(code) code // code block
#define CBL_INTERNAL_TRI(OS, ...) \
    cbl::TR_IMPL::tri cbl_TR_##__LINE__(OS, CBL_INTERNAL_TR_PREFIX, __VA_ARGS__)

//==============================================================================
// NAMESPACE cbl::TR_IMPL
//==============================================================================

namespace cbl::TR_IMPL {

//------------------------------------------------------------------------------
//
auto sep(int size = 70) { return cbl::iomanip::line{size, '='}; }

//------------------------------------------------------------------------------
//
struct trace {
    trace(cbl::ostream& os, std::string_view prefix)
        : os(os), scope(os, prefix) {}

    template <typename... Args>
    void operator()(Args&&... args) {
        ((os << args), ...);
        os << cbl::endl;
    }

    cbl::ostream& os;
    cbl::ostream::prefix_scope scope;
};

//------------------------------------------------------------------------------
//
struct tri {
    constexpr static int s_default = 4;

    tri(cbl::ostream& os, std::string_view prefix, char const* name, int indent)
        : os(os), prefix(prefix) {
        trace(os, prefix)(name, (name[0] ? " {" : "{"));
        m_scope = os.make_indent_scope(indent);
    }
    tri(cbl::ostream& os, std::string_view prefix, char const* name)
        : tri(os, prefix, name, s_default) {}
    tri(cbl::ostream& os, std::string_view prefix, int indent)
        : tri(os, prefix, "", indent) {}
    tri(cbl::ostream& os, std::string_view prefix)
        : tri(os, prefix, "", s_default) {}

    ~tri() {
        m_scope = cbl::ostream::indent_scope();
        trace(os, prefix)('}');
    }
    cbl::ostream& os;
    std::string_view prefix;
    cbl::ostream::indent_scope m_scope;
};

} // namespace cbl::TR_IMPL

#endif
