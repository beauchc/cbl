#ifndef CBL_IO_INTERNAL_TRACING_H
#define CBL_IO_INTERNAL_TRACING_H

#include <cbl/io/ostream.h>

#include <iostream>

#define CBL_INTERNAL_TR_STRINGIZE2(x) #x
#define CBL_INTERNAL_TR_STRINGIZE(x) CBL_INTERNAL_TR_STRINGIZE2(x)

#define CBL_INTERNAL_TR_PREFIX \
    cbl::TR_IMPL::get_prefix("0000" CBL_INTERNAL_TR_STRINGIZE(__LINE__) ": ")

#define CBL_INTERNAL_TR(OS, ...) \
    cbl::TR_IMPL::trace{OS, CBL_INTERNAL_TR_PREFIX}(__VA_ARGS__)

#define CBL_INTERNAL_TRS(...) cbl::TR_IMPL::sep(__VA_ARGS__) // separator
#define CBL_INTERNAL_TRV(EXPR) #EXPR " = ", (EXPR)           // var/expr
#define CBL_INTERNAL_TRP(EXPR) \
#EXPR " = ", cbl::iomanip::pretty(4, 1), EXPR // pretty

#define CBL_INTERNAL_TRC(code) code // code block
#define CBL_INTERNAL_TRI(OS, ...) \
    cbl::TR_IMPL::tri cbl_TR_##__LINE__(OS, CBL_INTERNAL_TR_PREFIX, __VA_ARGS__)

//==============================================================================
// NAMESPACE cbl::TR_IMPL
//==============================================================================

namespace cbl::TR_IMPL {

std::string_view get_prefix(std::string_view prefix) {
    return prefix.substr(prefix.size() - 7);
}

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
    template<typename... Args>
    tri(cbl::ostream& os, std::string_view prefix, Args&&... args)
        : os(os), prefix(prefix) {
        trace(os, prefix)(std::forward<Args>(args)..., " {");
        m_scope = os.make_indent_scope(4);
    }
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
