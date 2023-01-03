#ifndef ANY_H
#define ANY_H

#include "any_impl.h"

#include <utility>

class Any;
using TypeId = const char*;

template <typename... Ts>
struct PayloadTypes {};

template<typename Visitor>
struct GetPayloadTypes {
    using types = typename Visitor::payload_types;
};

template<typename Visitor>
using payload_types_t = typename GetPayloadTypes<Visitor>::types;

//==============================================================================
// CLASS Any
//==============================================================================

class Any : private Impl::AnyRep {
public:
    template<typename T>
    explicit Any(T value) : AnyRep(value) {}

    template<typename T>
    T* as() {
        void* payload = m_handler(Impl::AnyOp::eGet, *this, Impl::typeid_v<T>);
        return payload ? static_cast<T*>(payload) : nullptr;
    }
};

template<typename Visitor, typename T, typename... Ts>
auto make_visitor(Visitor& visitor, PayloadTypes<T, Ts...>) {
    using RetType = decltype(visitor(std::declval<T>()));
    return Impl::Visitor<Visitor, RetType, T, Ts...>(visitor);
}

template<typename Visitor, typename PayloadTypes = payload_types_t<Visitor>>
auto visit(Visitor&& visitor, Any const& any) {
    return make_visitor(visitor, PayloadTypes{})(any);
}

#endif
