#ifndef CBL_PRIORITY_QUEUE_H
#define CBL_PRIORITY_QUEUE_H

#include "internal/internal_priority_queue.h"

#include <functional>

//==============================================================================
// NAMESPACE cbl
//==============================================================================

namespace cbl {

//==============================================================================
// CLASS PriorityQueue<T, Compare>
//==============================================================================

/// \brief Updatable priority queue.
template <typename T, typename Compare = std::less<T>>
class PriorityQueue {
public:
    /*----- types -----*/

    /// \brief The type of values stored in the queue.
    using value_type = T;
    using size_type  = unsigned;

    /*----- member functions -----*/

    /// \brief Constructor
    PriorityQueue() = default;

    /// \brief Reserve space to allocate up to `n` elements in the queue.
    void reserve(size_type n) { m_impl.reserve(n); }

    /// \brief Get the queue's top element.
    T const& top() const {
        assert(!empty());
        return m_impl.top();
    }

    /// \brief Get the value in the queue associated with the given index.
    T const& operator[](size_type idx) const {
        assert(valid(idx));
        return m_impl[idx];
    }

    /// \brief Pushes a value in the queue and returns the associated index.
    ///
    /// The value can be retrieved using (*this)[idx] as long as it has not
    /// been popped.
    size_type push(T value) { return m_impl.push(std::move(value)); }

    /// \brief Emplaces a value in the queue and returns the associated index.
    ///
    /// The value can be retrieved using (*this)[idx] as long as it has not
    /// been popped.
    template <typename... Args>
    std::enable_if_t<std::is_constructible_v<T, Args...>, size_type> emplace(
        Args&&... args) {
        return m_impl.emplace(std::forward<Args>(args)...);
    }

    /// \brief Updates the value at the given index in the queue.
    void update(size_type idx, T value) {
        assert(valid(idx));
        return m_impl.update(idx, std::move(value));
    }

    /// \brief Pops the top value from the queue.
    void pop() {
        assert(!empty());
        return m_impl.pop();
    }

    /// \brief Returns true if the queue is empty, false otherwise.
    bool empty() const { return m_impl.empty(); }

    /// \brief Returns the size of the queue (number of values in the queue).
    ///
    /// \warning This DOES NOT imply that (*this)[idx] is valid for all idx
    /// from 0 to size()-1. Since indices are stable, they remain the same even
    /// when the queue is popped. This implies that there can still exist
    /// indices larger or equql to size() that are valid and indices smaller
    /// than size() that are invalid (because they where popped).
    size_t size() const { return m_impl.size(); }

    /// \brief Returns whether `idx` is a valid index in this queue.
    ///
    /// \warning Note that a same index can refer to different values in the
    /// queue during its lifetime. When a value is popped, its index will
    /// not longer be valid. But this index may be reused by a later call to
    /// push.
    bool valid(size_type idx) const { return m_impl.isValid(idx); }

private:
    using impl_t = cbl::internal::PriorityQueue<T, Compare>;

    /// \brief Internal implementation of the queue.
    impl_t m_impl;

    static_assert(std::is_same_v<size_type, typename impl_t::size_type>);
    static_assert(std::is_same_v<value_type, typename impl_t::value_type>);
};

} // namespace cbl

#endif
