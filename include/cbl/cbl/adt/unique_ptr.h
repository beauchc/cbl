#ifndef CBL_UNIQUE_PTR_H
#define CBL_UNIQUE_PTR_H

#include <memory>

//==============================================================================
// NAMESPACE cbl
//==============================================================================

namespace cbl {

//==============================================================================
// FORWARD DECLARATIONS
//==============================================================================

template <typename T>
class UniquePtr;

template <typename T, typename... Args>
UniquePtr<T> make_unique(Args&&... args);

//==============================================================================
// CLASS UniquePtr<T>
//==============================================================================

/// \brief Similar to std::unique_ptr<T>, but propagates const when accessing
/// the managed object.
template <typename T>
class UniquePtr {
private:
    template <typename U, typename R = void>
    using enable_if_convertible =
        std::enable_if_t<std::is_convertible_v<U*, T*>, R>;

public:
    UniquePtr() noexcept = default;

    template <typename U, typename = enable_if_convertible<U>>
    /*implicit*/ UniquePtr(UniquePtr<U>&& o) noexcept
        : m_ptr(std::move(o.m_ptr)) {}

    UniquePtr(UniquePtr&&) noexcept = default;
    UniquePtr& operator=(UniquePtr&&) noexcept = default;

    UniquePtr(UniquePtr const&) = delete;
    UniquePtr& operator=(UniquePtr const&) = delete;

    template <typename U, typename = enable_if_convertible<U>>
    UniquePtr& operator=(UniquePtr<U>&& o) noexcept {
        m_ptr = std::move(o.m_ptr);
        return *this;
    }
    ~UniquePtr() = default;

    explicit operator bool() const noexcept { return static_cast<bool>(m_ptr); }

    T const& operator*() const noexcept { return *m_ptr; }
    T&       operator*() noexcept { return *m_ptr; }

    T const* operator->() const noexcept { return m_ptr.operator->(); }
    T*       operator->() noexcept { return m_ptr.operator->(); }

    T const* get() const noexcept { return m_ptr.get(); }
    T*       get() noexcept { return m_ptr.get(); }

    T* release() noexcept { return m_ptr.release(); }

    void swap(UniquePtr& o) noexcept { m_ptr.swap(o.m_ptr); }

private:
    template <typename U>
    friend class UniquePtr;

    template <typename U, typename... Args>
    friend UniquePtr<U> make_unique(Args&&... args);

    explicit UniquePtr(std::unique_ptr<T> ptr) noexcept
        : m_ptr(std::move(ptr)) {}

    std::unique_ptr<T> m_ptr;
};

//==============================================================================
// FUNCTION make_unique<T>()
//==============================================================================

template <typename T, typename... Args>
UniquePtr<T> make_unique(Args&&... args) {
    return UniquePtr<T>(std::make_unique<T>(std::forward<Args>(args)...));
}

} // namespace cbl

//==============================================================================
// COMPARISON OPERATORS
//==============================================================================

template <typename U, typename V>
bool operator==(cbl::UniquePtr<U> const& lhs, cbl::UniquePtr<V> const& rhs) {
    return lhs.get() == rhs.get();
}
template <typename U, typename V>
bool operator!=(cbl::UniquePtr<U> const& lhs, cbl::UniquePtr<V> const& rhs) {
    return lhs.get() != rhs.get();
}
template <typename U, typename V>
bool operator<(cbl::UniquePtr<U> const& lhs, cbl::UniquePtr<V> const& rhs) {
    return lhs.get() < rhs.get();
}
template <typename U, typename V>
bool operator<=(cbl::UniquePtr<U> const& lhs, cbl::UniquePtr<V> const& rhs) {
    return lhs.get() <= rhs.get();
}
template <typename U, typename V>
bool operator>(cbl::UniquePtr<U> const& lhs, cbl::UniquePtr<V> const& rhs) {
    return lhs.get() > rhs.get();
}
template <typename U, typename V>
bool operator>=(cbl::UniquePtr<U> const& lhs, cbl::UniquePtr<V> const& rhs) {
    return lhs.get() >= rhs.get();
}

#endif
