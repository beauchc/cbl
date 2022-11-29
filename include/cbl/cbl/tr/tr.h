#ifndef CBL_TR_TR_H
#define CBL_TR_TR_H

#include <cbl/tr/internal/tr_internal.h>

//==============================================================================
// NAMESPACE cbl::tr_ns
//==============================================================================

namespace cbl::tr_ns {
inline constexpr auto endl = tr_internal::endl{};

template <typename T>
auto tr(T&& t) {
    return tr_internal::tr1(std::forward<T>(t));
}
template <typename T>
auto tr_nice(T&& t, int indent = 4) {
    return tr_internal::tr1(indent, std::forward<T>(t));
}
} // namespace cbl::tr_ns

//==============================================================================
// NAMESPACE cbl
//==============================================================================

namespace cbl {
using tr_ns::endl;
using tr_ns::tr;
} // namespace cbl

#endif
