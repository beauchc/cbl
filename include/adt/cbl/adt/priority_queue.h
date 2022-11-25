#ifndef CBL_PRIORITY_QUEUE_H
#define CBL_PRIORITY_QUEUE_H

#include "internal/internal_priority_queue.h"

namespace cbl {

//==============================================================================
// CLASS PriorityQueue<T, Compare>
//==============================================================================

template <typename T>
class PriorityQueueEntry : private internal::PriorityQueueEntry<T> {
public:
    T const& get() const { return this->value; }
};

//==============================================================================
// CLASS PriorityQueue<T, Compare>
//==============================================================================

template <typename T, typename Compare>
class PriorityQueue {
public:
    /*----- types -----*/

    using Entry = PriorityQueueEntry<T>;

    /*----- member functions -----*/

    explicit PriorityQueue(size_t reserve) : m_impl(reserve) {}

    T const& top() const {
        assert(!empty());
        return m_impl.top();
    }

    Entry const* push(T value) { return m_impl.push(std::move(value)); }

    template <typename... Args>
    std::enable_if_t<std::is_constructible_v<T, Args...>, Entry const*> emplace(
        Args&&... args) {
        return static_cast<Entry const*>(
            m_impl.emplace(std::forward<Args>(args)...));
    }

    void update_push(Entry const* e, T value) {
        assert(m_impl.belongs(e));
        return m_impl.update_push(e, std::move(value));
    }
    template <typename... Args>
    std::enable_if_t<std::is_constructible_v<T, Args...>> update_emplace(
        Entry const* e, Args&&... args) {
        assert(m_impl.belongs(e));
        return m_impl.update_emplace(e, std::forward<Args>(args)...);
    }

    void pop() { return m_impl.pop(); }

    bool empty() const { return m_impl.empty(); }

    size_t size() const { return m_impl.size(); }

private:
    cbl::internal::PriorityQueue<T, Compare> m_impl;
};

} // namespace cbl

#endif
