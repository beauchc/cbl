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
#define TRS(N) TR(cbl::tr::Separator(N))

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

//==============================================================================
// FORWARD DECLARATIONS
//==============================================================================

namespace cbl {
template <typename T>
class Span;
}

//==============================================================================
// NAMESPACE cbl::trmanip
//==============================================================================

namespace cbl::tr {

template <typename Range>
struct PrettyRange {
    Range const& r;
    int          indent;
};
template <typename Range>
auto pretty(Range const& r, int indent = 4) {
    return PrettyRange<Range>{r, indent};
}

int tr_indent = 0;

//------------------------------------------------------------------------------
//
struct Indent {
    explicit Indent(int n) : n(n) {}
    int n;
};
struct NewLine {
    explicit NewLine(int n) : n(n) {}
    int n;
};
struct Separator {
    explicit Separator(int n) : n(n) {}
    int n;
};

//------------------------------------------------------------------------------
//
template <typename Arg>
void tr_impl(std::ostream& os, Arg const& arg);
template <typename Arg, typename... Args>
void tr_impl(std::ostream& os, Arg const& arg, Args const&... args);

//------------------------------------------------------------------------------
//
template <bool Pretty, typename T>
void tr_range_impl(std::ostream& os, T const& v, int indent = 0) {
    os << '{';
    if constexpr (Pretty) tr_impl(os, NewLine(tr_indent + indent + 4));
    if (!v.empty()) {
        tr_impl(os, v.front());
        for (auto it = v.begin() + 1; it != v.end(); ++it) {
            tr_impl(os, ", ");
            if constexpr (Pretty) tr_impl(os, NewLine(tr_indent + indent + 4));
            tr_impl(os, *it);
        }
    }
    if constexpr (Pretty) tr_impl(os, NewLine(tr_indent + 4));
    os << '}';
}

//------------------------------------------------------------------------------
//
template <typename Arg>
void tr_impl(std::ostream& os, Arg const& arg) {
    constexpr bool is_string = //
        std::is_same_v<Arg, std::string> ||
        std::is_same_v<Arg, std::string_view>;

    if constexpr (is_string) os << '"';
    os << arg;
    if constexpr (is_string) os << '"';
}
template <>
void tr_impl(std::ostream& os, Indent const& indent) {
    for (int i = 0; i < indent.n; ++i) os << ' ';
}
template <>
void tr_impl(std::ostream& os, NewLine const& nl) {
    os << '\n';
    tr_impl(os, Indent(nl.n));
}
template <>
void tr_impl(std::ostream& os, Separator const& sep) {
     for (int i = 0; i < sep.n; ++i) os << '=';
}

template <typename T1, typename T2>
void tr_impl(std::ostream& os, std::pair<T1, T2> const& p) {
    tr_impl(os, '(', p.first, ", ", p.second, ')');
}

template <typename Range>
void tr_impl(std::ostream& os, PrettyRange<Range> const& v) {
    tr_range_impl<true>(os, v.r, v.indent);
}
template <typename T>
void tr_impl(std::ostream& os, cbl::Span<T> const& v) {
    tr_range_impl<false>(os, v);
}
template <typename T>
void tr_impl(std::ostream& os, std::vector<T> const& v) {
    tr_range_impl<false>(os, v);
}

//------------------------------------------------------------------------------
//
template <typename Arg, typename... Args>
void tr_impl(std::ostream& os, Arg const& arg, Args const&... args) {
    tr_impl(os, arg);
    tr_impl(os, args...);
}

//------------------------------------------------------------------------------
//
template <typename... Args>
void tr(std::ostream& os, int line, Args const&... args) {
    os << line << ": ";
    tr_impl(os, Indent(tr_indent), args...);
    os << std::endl;
}

struct TrGuard {
    constexpr static int s_indent = 4;

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

} // namespace cbl::tr

#endif
