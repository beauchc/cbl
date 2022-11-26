#ifndef CBL_IDX_H
#define CBL_IDX_H

#include <cassert>
#include <limits>

namespace cbl {

template <typename Derived, typename IntType>
class Idx {
public:
    using value_type      = IntType;
    using difference_type = IntType;

    /// \brief Construct an invalid node idx.
    constexpr Idx() noexcept = default;

    /// \brief Construct a index with the given index number.
    template <typename T>
    explicit constexpr Idx(T idx) noexcept : m_idx(value_type(idx)) {
        static_assert(sizeof(T) >= sizeof(value_type), "Too small int type");
        assert(idx >= 0);
        assert(std::size_t(idx) <=
               std::size_t(std::numeric_limits<value_type>::max()));
        assert((m_idx != kInvalid) && "Use default constructor instead.");
    }

    /// \brief Boolean explicit conversation operator.
    explicit constexpr operator bool() const noexcept { return isValid(); }

    /// \brief Return the index number.
    constexpr IntType get() const noexcept { return m_idx; }

    /// \brief Return true if the idx is valid, false otherwise.
    constexpr bool isValid() const noexcept { return m_idx != kInvalid; }

    /// \brief Increment and decrement operators
    /// Useful for iteration.
    /// \{
    Derived& operator++() noexcept {
        ++m_idx;
        return static_cast<Derived&>(*this);
    }
    Derived operator++(int) noexcept {
        Derived cpy{*this};
        ++m_idx;
        return cpy;
    }
    Derived& operator--() noexcept {
        --m_idx;
        return static_cast<Derived&>(*this);
    }
    Derived operator--(int) noexcept {
        Derived cpy{*this};
        --m_idx;
        return cpy;
    }
    /// \}

    /// \brief Get the difference between two indices.
    ///
    /// This is useful to allow genericity of some algorithms. For example,
    /// boost::integer_range<Derived> need such function to compute the
    /// range size.
    difference_type operator-(Derived o) const {
        assert(this->isValid() && o.isValid());
        return get() - o.get();
    }

    friend bool operator==(Idx lhs, Idx rhs) { return lhs.get() == rhs.get(); }
    friend bool operator!=(Idx lhs, Idx rhs) { return lhs.get() != rhs.get(); }
    friend bool operator<=(Idx lhs, Idx rhs) { return lhs.get() <= rhs.get(); }
    friend bool operator>=(Idx lhs, Idx rhs) { return lhs.get() >= rhs.get(); }
    friend bool operator<(Idx lhs, Idx rhs) { return lhs.get() < rhs.get(); }
    friend bool operator>(Idx lhs, Idx rhs) { return lhs.get() > rhs.get(); }

private:
    /*----- data members -----*/

    /// \brief The invalid index sentinel.
    static constexpr value_type kInvalid = ~value_type(0);
    value_type m_idx = kInvalid; ///< The actual index of the node
};

} // namespace cbl

#endif
