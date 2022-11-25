#ifndef IACCESSOR_H
#define IACCESSOR_H

#include <optional>

//------------------------------------------------------------------------------
//
#define IACCESSOR_FOREACH_(MACRO) \
    MACRO(int)                    \
    MACRO(float)                  \
    MACRO(double)                 \
    MACRO(unsigned)

#define IACCESSOR_FOREACH(MACRO) \
    IACCESSOR_FOREACH_(MACRO)    \
    static_assert(true)

//==============================================================================
// CLASS IAccessor
//==============================================================================

class IAccessor {
public:
    virtual ~IAccessor() {}

    template <typename T>
    std::optional<T> get() const = delete;

    template <typename T>
    bool set(T v) = delete;

protected:
#define IACCESSOR_DECL(T)                         \
    virtual std::optional<T> get_##T() const = 0; \
    virtual bool             set_##T(T v)    = 0;
    IACCESSOR_FOREACH(IACCESSOR_DECL);
#undef IACCESSOR_DECL
};

// clang-format off
#define IACCESSOR_DEF(T) \
template<> inline std::optional<T> IAccessor::get() const { return get_##T(); }\
template<> inline bool IAccessor::set(T v) { return set_##T(std::move(v)); }
IACCESSOR_FOREACH(IACCESSOR_DEF);
#undef IACCESSOR_DEF
// clang-format on

//==============================================================================
// CLASS IAccessorT<Derived>
//==============================================================================

template <typename Derived>
class IAccessorT : public IAccessor {
protected:
#define IACCESSOR_DEF(T)                                         \
    std::optional<T> get_##T() const final { return dget<T>(); } \
    bool             set_##T(T v) final { return dset<T>(std::move(v)); }
    IACCESSOR_FOREACH(IACCESSOR_DEF);
#undef IACCESSOR_DEF

private:
    template <typename U>
    static constexpr bool can_get = Derived::template can_get<U>;
    template <typename U>
    static constexpr bool can_set = Derived::template can_set<U>;

    template <typename T>
    std::enable_if_t<can_get<T>, T> dget() const {
        return static_cast<Derived const*>(this)->template get<T>();
    }
    template <typename T>
    std::enable_if_t<!can_get<T>, std::optional<T>> dget() const {
        return std::nullopt;
    }
    template <typename T>
    std::enable_if_t<can_set<T>, bool> dset(T v) {
        static_cast<Derived*>(this)->template set(std::move(v));
        return true;
    }
    template <typename T>
    std::enable_if_t<!can_set<T>, bool> dset(T) {
        return false;
    }
};

//------------------------------------------------------------------------------
//
template <typename T>
class Accessor : public IAccessorT<Accessor<T>> {
public:
    explicit Accessor(T value) : m_value(value) {}

    template <typename U>
    static constexpr bool can_get = std::is_convertible_v<T, U>;
    template <typename U>
    static constexpr bool can_set = std::is_convertible_v<U, T>;

    template <typename U>
    U get() const {
        return static_cast<U>(m_value);
    }
    template <typename U>
    void set(U v) {
        m_value = static_cast<T>(std::move(v));
    }

private:
    T m_value;
};

#endif
