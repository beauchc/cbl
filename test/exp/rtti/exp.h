#ifndef RTTI_H
#define RTTI_H

#include <cstddef>
#include <string_view>
#include <functional>
#include <vector>
#include <array>

class IArray {
public:
    void resize(size_t size);
};

// Could we create our own RTTI types?

class RTTI { // RunTime Type Information

};

template <typename T>
struct RTTI_for {
    static RTTI const* const s_rtti;
};

class RTII { // RunTime Interface Information
    // VTable const* getVTableFor(RTTI rtti);
};

template <typename T>
struct RTII_for {
    static RTII const* const s_rtii;
};

template<typename T>
RTTI* rtti() {
    return RTTI_for<T>::s_rtti;
}

struct FunctionType {};

template <typename RetType, typename... ArgTypes>
struct FunctionTypeT : public FunctionType {};

struct FunctionId {
    std::string_view name;
};

template<typename RetType, typename... ArgTypes>
struct Function {
    std::string_view name;
};

class Interface {
public:
    bool satisfies(Interface const& other);
    std::vector<FunctionId const*> m_functions;
};

class TypeId {

};

template<typename T>
TypeId const* getOrInsert();

template<typename RetType, typename... ArgTypes>
FunctionId const* getOrInsert(Function<RetType, ArgTypes...> const& fn);

Interface const* getOrInsert(FunctionId const** ids, size_t size);

template<typename T, typename... Functions>
Interface const* make_interface(Functions const&... functions) {
    constexpr size_t N = sizeof...(functions);
    std::array<FunctionId const*, N> fns;
    unsigned i = 0;
    ((fns[i++] = getOrInsert(functions)), ...);
    return getOrInsert(fns.data(), N);
}

template<typename Ret, typename T, typename... Args>
using MemberFunction = Ret(T::*)(Args...);

template<typename Ret, typename T, typename... Args>
struct Virtualized {
    Ret operator()(void* this_, Args... args) {
        auto t = static_cast<T*>(this_);
        return std::invoke(m_fn, t, std::forward<Args>(args)...);
    }
    MemberFunction<Ret, T, Args...> m_fn;
};

template<typename Ret, typename T, typename... Args>
auto virtualize(MemberFunction<Ret, T, Args...> fn) {
    return Virtualized<Ret, T, Args...>{fn};
}


#endif
