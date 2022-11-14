#ifndef CBL_DEBUG_STRING_CURSORS_H
#define CBL_DEBUG_STRING_CURSORS_H

#include <array>
#include <iosfwd>
#include <string_view>

namespace cbl::debug {

//==============================================================================
// CLASS StringCursors
//==============================================================================

template <unsigned N>
class StringCursors;

template <typename... Args>
auto make_string_cursors(std::string_view s, Args const&... args);

template <unsigned N>
std::ostream& operator<<(std::ostream& os, StringCursors<N> const& dc);

//==============================================================================
// CLASS StringCursorsImpl
//==============================================================================

class StringCursorslmpl {
public:
    StringCursorslmpl() = delete;

    /*---- static member functions ----*/

    template <typename Arg, typename... Args>
    static auto make_cursors(std::string_view s, Arg a, Args const&... args);

    template <unsigned N>
    static void print(std::ostream& os, StringCursors<N> const& cursors);

private:
    template <unsigned N>
    friend class StringCursors;

    /*---- member functions ----*/

    using Pair = std::pair<char const*, char>;

    template <unsigned N>
    using Array = std::array<Pair, N>;

    template <unsigned N>
    using CursorsBase = std::pair<std::string_view, Array<N>>;

    /*---- static member functions ----*/

    static void unpack1(Pair*) {}
    template <typename... Args>
    static void unpack1(Pair* dst, char c, char const* p, Args const&... args) {
        *dst = {p, c};
        unpack1(++dst, args...);
    }

    static void unpack2(Pair*, char) {}
    template <typename... Args>
    static void unpack2(Pair* dst, char c, char const* p, Args const&... args) {
        *dst = {p, c};
        unpack2(++dst, ++c, args...);
    }

    template <typename... Args>
    static void unpack(Pair* dst, char c, char const* p, Args const&... args) {
        unpack1(dst, c, p, args...);
    }
    template <typename... Args>
    static void unpack(Pair* dst, char const* p, Args const&... args) {
        unpack2(dst, 'A', p, args...);
    }

    template <unsigned N, typename... Args>
    auto make_cursors(std::string_view s, Args const&... args) {
        StringCursors<N> output;
        output.first = s;
        unpack(&output.second[0], args...);
        sort(output.second.data(), N);
        return output;
    }

    static void sort(Pair* data, size_t size);

    static void print( //
        std::ostream&    os,
        std::string_view str,
        Pair const*      pairs,
        bool*            bools,
        unsigned         n);
};

//------------------------------------------------------------------------------
//
template <typename Arg, typename... Args>
auto StringCursorslmpl::make_cursors(std::string_view s,
                                     Arg              a,
                                     Args const&... args) {
    constexpr unsigned DIV = std::is_same_v<char, Arg> ? 2 : 1;
    constexpr unsigned N   = (sizeof...(args) + 1) / DIV;
    StringCursors<N>   output;
    output.first = s;
    unpack(&output.second[0], a, args...);
    sort(output.second.data(), N);
    return output;
}

template <unsigned N>
void StringCursorslmpl::print(std::ostream&           os,
                              StringCursors<N> const& cursors) {
    std::array<bool, N> printed{};
    print(os, cursors.first, cursors.second.data(), printed.data(), N);
}

//------------------------------------------------------------------------------
//
template <unsigned N>
class StringCursors : public StringCursorslmpl::CursorsBase<N> {};

template <typename... Args>
auto make_string_cursors(std::string_view s, Args const&... args) {
    return StringCursorslmpl::make_cursors(s, args...);
}

template <unsigned N>
std::ostream& operator<<(std::ostream& os, StringCursors<N> const& cursors) {
    StringCursorslmpl::print(os, cursors);
    return os;
}

} // namespace cbl::debug

#endif
