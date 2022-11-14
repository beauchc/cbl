#include <cbl/debug/string_cursors.h>

#include <algorithm>
#include <iostream>

namespace cbl::debug {

//------------------------------------------------------------------------------
//
void StringCursorslmpl::sort(Pair* data, size_t size) {
    std::stable_sort(data, data + size, [](auto const& a, auto const& b) {
        return a.first < b.first;
    });
}

//------------------------------------------------------------------------------
//
void StringCursorslmpl::print( //
    std::ostream&    os,
    std::string_view str,
    Pair const*      pairs,
    bool*            bools,
    unsigned         n) {
    os << "\n'" << str << "'";

    unsigned missing = n;
    while (missing) {
        os << "\n ";
        auto prev = str.data();
        for (unsigned i = 0; i < n; ++i) {
            if (bools[i]) continue;
            bools[i] = true;
            --missing;
            for (; prev < pairs[i].first; ++prev) os << ' ';
            os << pairs[i].second;
            ++prev;
            while (i < n && pairs[i].first < prev) ++i;
            --i;
        }
    }
}

} // namespace cbl::debug
