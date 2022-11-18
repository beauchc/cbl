#ifndef CBL_DEBUG_TRACE_H
#define CBL_DEBUG_TRACE_H

#include "debug_trace_toggle.h"

#include <iostream>
#include <vector>

// clang-format off
#ifndef TR
#error  TR should be defined in debug_trace_toggle.h
#endif
#undef  TR
#define TR(...) cbl::tr::tr(std::cerr, __LINE__, __VA_ARGS__)

#ifndef TRV
#error  TRV should be defined in debug_trace_toggle.h
#endif
#undef  TRV
#define TRV(v) TR(#v " = ", v);

#ifndef TRS
#error  TRS should be defined in debug_trace_toggle.h
#endif
#undef  TRS
#define TRS TR("========== ", __LINE__, " ==========")

#ifndef TR_BLOCK
#error  TR_BLOCK should be defined in debug_trace_toggle.h
#endif
#undef  TR_BLOCK
#define TR_BLOCK(code) { code } static_assert(true)

#ifndef TR_INDENT
#error  TR_INDENT should be defined in debug_trace_toggle.h
#endif
#undef  TR_INDENT
#define TR_INDENT(...) \
    cbl::tr::TrGuard cbl_tr_guard##__LINE__{__LINE__, __VA_ARGS__};
// clang-format on

namespace cbl::tr {

int tr_indent = 0;

template <typename T>
constexpr bool is_string_v =
    std::is_same_v<T, std::string> || std::is_same_v<T, std::string_view>;

template <typename Arg, typename... Args>
void tr_impl(std::ostream& os, Arg const& arg, Args const&... args);

template <typename Arg>
void tr_impl(std::ostream& os, Arg const& arg) {
    if constexpr (is_string_v<Arg>) os << '"';
    os << arg;
    if constexpr (is_string_v<Arg>) os << '"';
}

template <typename T>
void tr_impl(std::ostream& os, std::vector<T> const& v) {
    os << '{';
    if (!v.empty()) {
        tr_impl(os, v.front());
        for (auto it = v.begin() + 1; it != v.end(); ++it)
            tr_impl(os, ", ", *it);
    }
    os << '}';
}
template <typename Arg, typename... Args>
void tr_impl(std::ostream& os, Arg const& arg, Args const&... args) {
    tr_impl(os, arg);
    tr_impl(os, args...);
}

template <typename... Args>
void tr(std::ostream& os, int line, Args const&... args) {
    os << line << ": ";
    for (int i = 0; i < tr_indent; ++i) os << ' ';
    tr_impl(os, args...);
    os << std::endl;
}

struct TrGuard {
    constexpr static int s_indent = 3;

    TrGuard(int line, char const* name, int indent)
        : m_line(line), m_indent(indent) {
        tr(std::cerr, line, name, (name[0] ? " {" : "{"));
        tr_indent += indent;
    }
    TrGuard(int line, char const* name) : TrGuard(line, name, s_indent) {}
    TrGuard(int line, int indent) : TrGuard(line, "", indent) {}
    TrGuard(int line) : TrGuard(line, "", s_indent) {}

    ~TrGuard() {
        tr_indent -= m_indent;
        tr(std::cerr, m_line, '}');
    }
    int m_line, m_indent;
};

} // namespace cbl::details

#endif
