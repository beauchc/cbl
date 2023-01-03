#ifndef ANY_IMPL_H
#define ANY_IMPL_H

#include <type_traits>
#include <typeinfo>
#include <utility>

class Any;
using TypeId = const char*;

template <typename... Ts>
struct PayloadTypes;

namespace Impl {

struct VisitorBase;

template <typename T>
inline TypeId typeid_v = typeid(T).name();

enum class AnyOp { eGet, eVisit };

class AnyRep;
using Buffer  = std::aligned_storage_t<3 * sizeof(void*), alignof(void*)>;
using Handler = void* (*)(AnyOp, Any&, void const*);

//==============================================================================
// CLASS Handler
//==============================================================================

template <typename T>
class HandlerT {
    friend AnyRep;
    static void* handler(AnyOp op, Any& any, void const* args) {
        switch (op) {
            case AnyOp::eGet: return get(any, args);
            case AnyOp::eVisit: return visit(any, args);
        }
        return nullptr;
    }

    static void* get(Any& any, void const* args) {
        TypeId typeId = reinterpret_cast<TypeId>(args);
        return (typeId == typeid(T).name()) ? &any : nullptr;
    }

    static void* visit(Any& any, void const* args);
};

//==============================================================================
// CLASS AnyRep
//==============================================================================

class AnyRep {
protected:
    template <typename T>
    explicit AnyRep(T value) : m_handler(&Impl::HandlerT<T>::handler) {
        new (reinterpret_cast<T*>(&m_buffer)) T(std::move(value));
    }

    friend VisitorBase;
    Buffer m_buffer;
    Handler m_handler;
};

//==============================================================================
// Visitors
//==============================================================================

struct VisitorBase {
    virtual void failed(Any const& any, void* out) = 0;

    Handler getHandler(Any const& any) {
        return reinterpret_cast<AnyRep const&>(any).m_handler;
    }
};

template <typename T>
struct VisitorT {
    virtual void visit(T const& t, void* ret) = 0;
};

template <typename Impl, typename RetType, typename... Ts>
struct VisitorImpl;

template <typename Impl, typename RetType>
struct VisitorImpl<Impl, RetType> : public VisitorBase {
    using return_type = RetType;
    VisitorImpl(Impl& impl) : m_impl(impl) {}

    void failed(Any const& any, void* ret) override {
        if constexpr (std::is_same_v<RetType, void>) {
            m_impl(any);
        } else {
            new (ret) RetType(this->m_impl(any));
        }
    }

    Impl& m_impl;
};

template <typename Impl, typename RetType, typename T, typename... Ts>
struct VisitorImpl<Impl, RetType, T, Ts...> //
    : public VisitorT<T>, VisitorImpl<Impl, RetType, Ts...> {

    explicit VisitorImpl(Impl& impl)
        : VisitorImpl<Impl, RetType, Ts...>(impl) {}

    void visit(T const& t, void* ret) override {
        if constexpr (std::is_same_v<RetType, void>) {
            m_impl(t);
        } else {
            new (ret) RetType(this->m_impl(t));
        }
    }
};

struct VisitArgs {
    VisitorBase* visitor;
    void*        ret;
};

template<typename Impl, typename RetType, typename T, typename... Ts>
struct Visitor : public VisitorImpl<Impl, RetType, T, Ts...> {
    Visitor(Impl& impl) : VisitorImpl<Impl, RetType, T, Ts...>(impl) {}

    RetType operator()(Any const& any) {
        auto handler = this->getHandler(any);
        if constexpr (std::is_same_v<RetType, void>) {
            auto args = VisitArgs{this, nullptr};
            handler(AnyOp::eVisit, any, &args);
        } else {
            BufferType buffer;
            auto args = VisitArgs{this, &buffer};
            handler(AnyOp::eVisit, const_cast<Any&>(any), &args);
            return std::move(*reinterpret_cast<RetType*>(&buffer));
        }
    }

    auto make_buffer() const {
        constexpr bool isVoid = std::is_same_v<RetType, void>;
        using U               = std::conditional_t<isVoid, char, RetType>;
        return std::aligned_storage_t<sizeof(U), alignof(U)>();
    }

    using BufferType = std::conditional_t<
        std::is_same_v<RetType, void>,
        std::aligned_storage_t<sizeof(RetType), alignof(RetType)>,
        std::nullptr_t>;
};

template<typename T>
void* HandlerT<T>::visit(Any& any, void const* args) {
    VisitArgs const& args2 = *reinterpret_cast<VisitArgs const*>(args);
    VisitorT<T>* vis = dynamic_cast<VisitorT<T>*>(args2.visitor);
    if (vis) {
        vis->visit(*reinterpret_cast<T*>(&any), args2.ret);
    } else {
        args2.visitor->failed(any, args2.ret);
    }
    return nullptr;
}

} // namespace Impl

#endif
