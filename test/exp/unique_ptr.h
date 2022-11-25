#ifndef UNIQUE_PTR_H
#define UNIQUE_PTR_H

#include <memory>

//==============================================================================
// FORWARD DECLARATIONS
//==============================================================================

template <typename T>
class UniquePtr;

template<typename T, typename... Args>
UniquePtr<T> make_unique(Args&&... args);

//==============================================================================
// CLASS UniquePtr<T>
//==============================================================================

template <typename T>
class UniquePtr {
public:
    template <typename U>
    /*implicit*/ UniquePtr(UniquePtr<U> o) : m_ptr(std::move(o.m_ptr)) {}

    UniquePtr(UniquePtr&&) noexcept = default;
    UniquePtr& operator=(UniquePtr&&) noexcept = default;

    UniquePtr(UniquePtr const&) noexcept = delete;
    UniquePtr& operator=(UniquePtr const&) noexcept = delete;

    T const& operator*() const { return *m_ptr; }
    T&       operator*() { return *m_ptr; }

    T const* operator->() const { return m_ptr.operator->(); }
    T*       operator->() { return m_ptr.operator->(); }

private:
    template <typename U>
    friend class UniquePtr;

    template<typename U, typename... Args>
    friend UniquePtr<U> make_unique(Args&&... args);

    explicit UniquePtr(std::unique_ptr<T> ptr) : m_ptr(std::move(ptr)) {}

    std::unique_ptr<T> m_ptr;
};

//==============================================================================
// FUNCTION make_unique<T>(Args...)
//==============================================================================

template<typename T, typename... Args>
UniquePtr<T> make_unique(Args&&... args) {
    return UniquePtr<T>(std::make_unique<T>(std::forward<Args>(args)...));
}

#endif
