#ifndef IAPPLY_H
#define IAPPLY_H

#include "iaccessor.h"
#include "unique_ptr.h"

#include <cassert>
#include <tuple>
#include <vector>

using IAccessorPtr = UniquePtr<IAccessor>;

template <typename... Args>
struct Signature {};

template <typename... Cbs>
using Callbacks = std::tuple<Cbs...>;

template <typename Signature, typename... CB, typename IAccessors>
bool apply(Callbacks<CB...> const& cbs, IAccessors const& inputs);

//==============================================================================
// APPLY IMPLEMENTATION
//==============================================================================

namespace apply_impl {

//------------------------------------------------------------------------------
//
template <typename... Args>
using CBs = Callbacks<Args...>;

template <typename... Args>
using S = Signature<Args...>;

template <typename... Args>
constexpr auto signature_size(S<Args...>) {
    return sizeof...(Args);
}

//------------------------------------------------------------------------------
//
template <typename CB, typename... Args>
bool apply_cb(CB const& cb, Args&&... args) {
    if constexpr (std::is_same_v<void, decltype(cb(args...))>) {
        cb(args...);
        return true;
    } else {
        return cb(args...);
    }
}

template <size_t I, size_t N, typename... CB, typename... Args>
bool apply_cbs(CBs<CB...> const& cbs, Args&&... args) {
    if constexpr (I != N) {
        return apply_cb(std::get<I>(cbs), args...) &&
               apply_cbs<I + 1, N>(cbs, std::forward<Args>(args)...);
    } else {
        return true;
    }
}

//------------------------------------------------------------------------------
//
template <typename CB, typename S, typename... Args>
bool apply(CB const& cbs, S, IAccessor* const* inputs, Args&&... args);

template <typename... CB, typename... Args>
bool apply(CBs<CB...> const& cbs, S<>, IAccessorPtr const*, Args&&... args) {
    constexpr auto N = sizeof...(CB);
    return apply_cbs<0, N>(cbs, std::forward<Args>(args)...);
}

template <typename... CB, typename P, typename... Ps, typename... Args>
bool apply(CBs<CB...> const& cbs,
           S<P, Ps...>,
           IAccessorPtr const* inputs,
           Args&&... args) {
    auto v = (*inputs)->get<P>();
    if (!v) return false;
    return apply(cbs, S<Ps...>{}, ++inputs, std::forward<Args>(args)...,
                 std::move(*v));
}

} // namespace apply_impl

//------------------------------------------------------------------------------
//
template <typename Signature, typename... CB, typename IAccessors>
bool apply(Callbacks<CB...> const& cbs, IAccessors const& inputs) {
    assert(inputs.size() >= apply_impl::signature_size(Signature{}));
    return apply_impl::apply(cbs, Signature{}, inputs.data());
}

#endif
