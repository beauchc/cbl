#ifndef CBL_IO_INTERNAL_TRACING_H
#define CBL_IO_INTERNAL_TRACING_H

#include <cbl/io/ostream.h>

#define CBL_INTERNAL_TR(OS, ...) cbl::TR_IMPL::trace{OS, __LINE__}(__VA_ARGS__)
#define CBL_INTERNAL_TRC(code) code // code block

#define CBL_INTERNAL_TRS(...) cbl::TR_IMPL::sep(__VA_ARGS__) // separator
#define CBL_INTERNAL_TRV(EXPR) #EXPR, " = ", (EXPR)          // var/expr

#define CBL_INTERNAL_TRI(...) \
    cbl::TR_IMPL::tri cbl_TR_guard##__LINE__{__LINE__, __VA_ARGS__};

//==============================================================================
// NAMESPACE cbl::TR_IMPL
//==============================================================================

namespace cbl::TR_IMPL {

//------------------------------------------------------------------------------
//
auto sep(unsigned size = 70) { return cbl::iomanip::line{size, '='}; }

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

// struct tri {
//     constexpr static int s_indent = 4;
//
//     tri(int line, char const* name, int indent)
//         : m_line(line), m_indent(indent) {
//         tr(std::cerr, line, name, (name[0] ? " {" : "{"));
//         tr_indent += indent;
//     }
//     tri(int line, char const* name) : tri(line, name, s_indent) {}
//     tri(int line, int indent) : tri(line, "", indent) {}
//     tri(int line) : tri(line, "", s_indent) {}
//
//     ~tri() {
//         tr_indent -= m_indent;
//         tr(std::cerr, m_line, '}');
//     }
//     int m_line, m_indent;
// };

} // namespace cbl::TR_IMPL

#endif
