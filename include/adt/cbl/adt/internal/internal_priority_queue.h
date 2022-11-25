#ifndef CBL_PRIVATE_PRIORITY_QUEUE_IMPL_H
#define CBL_PRIVATE_PRIORITY_QUEUE_IMPL_H

#include <cassert>
#include <memory>
#include <vector>

//==============================================================================
// FORWARD DECLARATIONS
//==============================================================================

namespace cbl {
template <typename T>
class PriorityQueueEntry;
template <typename T, typename Compare>
class PriorityQueue;
} // namespace cbl


//==============================================================================
// NAMESPACE cbl::pimpl
//==============================================================================

namespace cbl::internal {

//==============================================================================
// CLASS PriorityQueueEntry<T>
//==============================================================================

template <typename T>
class PriorityQueueEntry {
private:
    friend cbl::PriorityQueueEntry<T>;
    T      value;
    size_t heap_idx;
};

//==============================================================================
// CLASS PriorityQueue<T>
//==============================================================================

template <typename T, typename Compare>
class PriorityQueue {
private:
    friend PriorityQueue<T, Compare>;

    template <typename Ret, typename... Args>
    using enable_if_construtible_t =
        std::enable_if_t<std::is_constructible_v<T, Args...>, Ret>;

    using Entry = PriorityQueueEntry<T>;

    explicit PriorityQueue(size_t reserve) {
        m_heap.reserve(reserve);
        m_pages.push_back(Page{reserve});
    }

    bool empty() const { return m_heap.empty(); }

    size_t size() const { return m_heap.size(); }

    T const& top() const { return m_heap.front()->value; }

    template <typename... Args>
    enable_if_construtible_t<Entry const*, Args...> emplace(Args&&... args) {
        auto e = alloc();
        new (&(e->value)) T(std::forward<Args>(args)...);
        e->heap_idx = m_heap.size();
        m_heap.push_back(e);
        sift_up(e);
        return e;
    }
    Entry const* push(T value) { return emplace(std::move(value)); }

    void pop() {
        assert(!empty());
        m_heap[0]->value.~T();
        free(m_heap[0]);
        m_heap[0] = m_heap.back();
        m_heap.pop_back();
        m_heap[0]->heap_idx = 0;
        sift_down(m_heap.front());
    }

    template <typename... Args, enable_if_construtible_t<void, Args...>>
    void update_emplace(Entry const* e, Args&&... args) {
        assert(m_heap[e->heap_idx] == e);
        auto e2   = const_cast<Entry*>(e);
        e2->value = T(std::forward<Args>(args)...);
        sift(e2);
    }
    void update_push(Entry const* e, T value) {
        assert(m_heap[e->heap_idx] == e);
        auto e2   = const_cast<Entry*>(e);
        e2->value = std::move(value);
        sift(e2);
    }

    bool belongs(Entry const* e) const {
        return e->heap_idx < m_heap.size() && m_heap[e->heap_idx] == e;
    }

    struct Page {
        explicit Page(size_t size) : m_ptr(new Entry[size]), size(size) {
            assert(m_ptr);
        }
        Entry* next() { return (idx <= size) ? nullptr : m_ptr.get()[idx++]; }
        std::unique_ptr<Entry[]> m_ptr;
        size_t                   idx = 0, size;
    };
    void sift(Entry* e) {
        auto eidx = e->heap_idx;
        sift_up(e);
        if (e->heap_idx != eidx) return;
        sift_down(e);
    }
    void sift_up(Entry* child) {
        auto cidx = child->heap_idx;
        while (cidx > 0) {
            auto  pidx   = (cidx - 1) / 2;
            auto* parent = m_heap[pidx];
            if (!compare()(child->value, parent->value)) break;

            std::swap(m_heap[cidx], m_heap[pidx]);
            parent->heap_idx = cidx;
            cidx             = pidx;
        }
        child->heap_idx = cidx;
    }
    void sift_down(Entry* parent) {
        auto max_idx = m_heap.size() / 2;

        auto pidx = parent->heap_idx;
        while (pidx < max_idx) {
            auto cidx = (pidx * 2) + 1;
            if (cidx + 1 < m_heap.size()) {
                if (!compare()(m_heap[cidx], m_heap[cidx])) ++cidx;
            }
            auto* child = m_heap[cidx];
            if (!compare()(child->value, parent->value)) break;

            std::swap(m_heap[cidx], m_heap[pidx]);
            child->heap_idx = pidx;
            pidx            = cidx;
        }
        parent->heap_idx = pidx;
    }
    Entry* alloc() {
        if (!m_freed.empty()) {
            auto e = m_freed.back();
            m_freed.pop_back();
            return e;
        }
        if (auto e = m_pages.back().next()) return e;
        m_pages.push_back(Page{m_heap.size() * 2});
        return m_pages.back().next();
    }
    void dealloc(Entry* e) {
        e->value->~T();
        m_freed.push_back(e);
    }
    Compare const& compare() const { return m_compare; }

    std::vector<Entry*> m_heap;
    std::vector<Page>   m_pages;
    std::vector<Entry*> m_freed;
    Compare             m_compare;
};

} // namespace cbl::pimpl

#endif
