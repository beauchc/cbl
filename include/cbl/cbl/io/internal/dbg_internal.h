#ifndef CBL_TR_DBG_INTERNAL_H
#define CBL_TR_DBG_INTERNAL_H

#include <cbl/tr/tr.h>

#define CBL_INTERNAL_TR(OS, ...) cbl::TR_IMPL::trace{OS, __LINE__}(__VA_ARGS__)
#define CBL_INTERNAL_TRC(code) code // code block

#define CBL_INTERNAL_TRS(...) (cbl::TR_IMPL::sep{__VA_ARGS__})   // separator
#define CBL_INTERNAL_TRV(EXPR) cbl::TR_IMPL::expr{#EXPR}, (EXPR) // var/expr

#define CBL_INTERNAL_TRI(...) \
    cbl::TR_IMPL::tri cbl_TR_guard##__LINE__{__LINE__, __VA_ARGS__};

//==============================================================================
// NAMESPACE cbl::TR_IMPL
//==============================================================================

namespace cbl::TR_IMPL {

//------------------------------------------------------------------------------
//
struct expr {
    char const* name;
};
struct sep {
    int size = 70;
};

template <typename Os>
struct trace {
    trace(Os& os, int line) : os(os), line(line) {}

    void write(expr arg) const { os << arg.name << " = "; }
    void write(sep arg) const {
        for (int i = 0; i < arg.size; ++i) os << '=';
    }
    template <typename Arg>
    void write(Arg&& arg) const {
        os << tr(std::forward<Arg>(arg));
    }
    template <typename Arg, typename... Args>
    void write(Arg&& arg, Args&&... args) const {
        write(std::forward<Arg>(arg));
        write(std::forward<Args>(args)...);
    }
    template <typename... Args>
    void operator()(Args&&... args) const {
        os << line << " : ";
        write(std::forward<Args>(args)...);
        os << '\n';
    }

    Os& os;
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
