#include "tests.h"

#include <gtest/gtest.h>

using namespace std::string_view_literals;

//==============================================================================
// TESTS
//==============================================================================

struct MyClass {
    bool get() { return value; }

    void bla(int);
    void bla(float);

    bool value = false;
};

class GetableView {
public:
    struct Vtable {
        using Destroy = void(*)(void*);
        using GetFn = bool (*)(void*);

        Destroy destroy_;
        GetFn get;
    };

    template <typename T>
    struct VtableFor {
        constexpr static Vtable vtable{
            [](void* this_) { delete static_cast<T*>(this_); },
            [](void* this_) {
                return std::invoke(&T::get, static_cast<T*>(this_));
            }};
    };

    template <typename T>
    static constexpr auto vtable_for_v = &VtableFor<T>::vtable;

    Vtable const* vtable;
    void*         ptr;

    template <typename T>
    explicit GetableView(T* t) : vtable(vtable_for_v<T>), ptr(t) {}
    template <typename T>
    explicit GetableView(T& t) : vtable(vtable_for_v<T>), ptr(&t) {}

    ~GetableView() {}

    bool get() { return std::invoke(vtable->get, ptr); }
};

class Getable : public GetableView {
public:
    template <typename T>
    Getable(T* t) = delete;
    template <typename T>
    Getable(T&& t)
        : GetableView(new std::remove_const_t<std::remove_reference_t<T>>(
              std::forward<T>(t))) {}
    ~Getable() { vtable->destroy_(ptr); }
};

//------------------------------------------------------------------------------
//
TEST(virtualize, tests) {
    MyClass mc1{true};
    MyClass mc2{false};

    GetableView gv1{mc1};
    GetableView gv2{mc2};

    EXPECT_EQ(true, gv1.get());
    EXPECT_EQ(false, gv2.get());

    Getable g1{mc1};
    Getable g2{mc2};

    EXPECT_EQ(true, g1.get());
    EXPECT_EQ(false, g2.get());

    auto getter = virtualize(&MyClass::get);
    // std::cerr << (sizeof(getter.m_fn)) << std::endl;

    EXPECT_EQ(true, getter(&mc1));
    EXPECT_EQ(false, getter(&mc2));
}
