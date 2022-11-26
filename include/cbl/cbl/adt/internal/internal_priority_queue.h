#ifndef CBL_INTERNAL_PRIORITY_QUEUE_H
#define CBL_INTERNAL_PRIORITY_QUEUE_H

/// \cond CBL_INTERNAL_DOCS

#include <cbl/adt/idx.h>

#include <cassert>
#include <vector>

//==============================================================================
// FORWARD DECLARATIONS
//==============================================================================

namespace cbl {
template <typename T, typename Compare>
class PriorityQueue;
} // namespace cbl

//==============================================================================
// NAMESPACE cbl::internal
//==============================================================================

namespace cbl::internal {

//==============================================================================
// CLASS PriorityQueue<T, Compare>
//==============================================================================

template <typename T, typename Compare>
class PriorityQueue {
private:
    friend cbl::PriorityQueue<T, Compare>;

    /*----- types -----*/

    /// \copydoc cbl::PriorityQueue<T,Compare>::value_type
    using value_type = T;

    /// \copydoc cbl::PriorityQueue<T,Compare>::size_type
    using size_type = unsigned;

    struct EntryIdx : public Idx<EntryIdx, size_type> {
        using Idx<EntryIdx, size_type>::Idx;
    };
    struct HeapIdx : public Idx<HeapIdx, size_type> {
        using Idx<HeapIdx, size_type>::Idx;
    };
    struct Entry {
        Entry(value_type value, EntryIdx idx) : value(value), idx(idx) {}
        value_type value;
        EntryIdx   idx;
    };

    /*----- member functions -----*/

    /// \copydoc cbl::PriorityQueue<T,Compare>::PriorityQueue
    PriorityQueue() = default;

    /// \copydoc cbl::PriorityQueue<T,Compare>::reserve
    void reserve(size_type n) {
        m_heap.reserve(n);
        m_e2h.reserve(n);
    }

    /// \copydoc cbl::PriorityQueue<T,Compare>::top
    T const& top() const { return value(m_heap.front()); }

    /// \copydoc cbl::PriorityQueue<T,Compare>::operator[]
    T const& operator[](size_type e_idx) const {
        return value(entry(EntryIdx{e_idx}));
    }

    /// \copydoc cbl::PriorityQueue<T,Compare>::empty
    bool empty() const { return m_heap.empty(); }

    /// \copydoc cbl::PriorityQueue<T,Compare>::size
    size_t size() const { return m_heap.size(); }

    /// \copydoc cbl::PriorityQueue<T,Compare>::emplace
    template <typename... Args>
    size_type emplace(Args&&... args) {
        m_heap.emplace_back(std::forward<Args>(args)..., EntryIdx{size()});
        return sift_pushed();
    }

    /// \copydoc cbl::PriorityQueue<T,Compare>::push
    size_type push(T value) {
        m_heap.emplace_back(std::move(value), EntryIdx{size()});
        return sift_pushed();
    }

    /// \copydoc cbl::PriorityQueue<T,Compare>::pop
    void pop() {
        assert(!empty());
        m_freed.push_back(m_heap.front().idx);
        heap_idx(m_freed.back()) = HeapIdx{};
        if (m_heap.size() == 1) {
            m_heap.pop_back();
            return;
        }
        m_heap.front() = m_heap.back();
        m_heap.pop_back();
        heap_idx(m_heap.front()) = HeapIdx{0};

        sift_down(HeapIdx{0});
        assert_invariants();
    }

    //--------------------------------------------------------------------------
    void update(size_type idx, T v) {
        auto e_idx   = EntryIdx{idx};
        auto h_idx   = heap_idx(e_idx);
        value(h_idx) = std::move(v);
        sift(h_idx);
        assert_invariants();
    }

    //--------------------------------------------------------------------------
    /// \brief Sift up the element that was just pushed / emplaced
    size_type sift_pushed() {
        EntryIdx e_idx = m_heap.back().idx;
        HeapIdx  h_idx{m_heap.size() - 1};
        if (!m_freed.empty()) {
            e_idx = m_freed.back();
            m_freed.pop_back();
            m_heap.back().idx           = e_idx;
            heap_idx(m_heap.back().idx) = h_idx;
        } else {
            assert(m_e2h.size() == h_idx.get());
            m_e2h.push_back(h_idx);
        }
        sift_up(h_idx);
        assert_invariants();
        return e_idx.get();
    }

    /// \brief Sift an element either up or down (used by update())
    void sift(HeapIdx h_idx) {
        assert(heap_idx(entry(h_idx)) == h_idx);
        sift_up(h_idx);
        if (heap_idx(entry(h_idx)) != h_idx) return;
        sift_down(h_idx);
    }

    /// \brief Sift an element either up in the queue (closer to top)
    void sift_up(HeapIdx child) {
        while (child.get() > 0) {
            auto parent = HeapIdx{(child.get() - 1) / 2};
            if (compare(value(child), value(parent))) break;
            swap(entry(child), entry(parent));
            child = parent;
        }
    }

    /// \brief Sift an element either down in the queue (further from top)
    void sift_down(HeapIdx parent) {
        auto max_idx = m_heap.size() / 2;
        while (parent.get() < max_idx) {
            auto child = HeapIdx{(parent.get() * 2) + 1};
            if (child.get() + 1 < m_heap.size()) {
                auto rchild = HeapIdx{child.get() + 1};
                if (compare(value(child), value(rchild))) child = rchild;
            }
            if (compare(value(child), value(parent))) break;
            swap(entry(child), entry(parent));
            parent = child;
        }
    }

    //--------------------------------------------------------------------------
    /// \brief Swaps elements in the queue
    void swap(Entry& lhs, Entry& rhs) {
        std::swap(heap_idx(lhs.idx), heap_idx(rhs.idx));
        std::swap(lhs, rhs);
        assert(entry(lhs.idx).idx = lhs.idx);
        assert(entry(rhs.idx).idx = rhs.idx);
    }

    //--------------------------------------------------------------------------
    bool isValid(size_type e_idx) const { return isValid(EntryIdx{e_idx}); }
    bool isValid(EntryIdx e_idx) const {
        if (e_idx.get() >= m_e2h.size()) return false;
        auto h_idx = heap_idx(e_idx);
        if (h_idx.get() >= m_heap.size()) return false;
        return entry(h_idx).idx == e_idx;
    }

    //--------------------------------------------------------------------------
    T const& value(Entry const& e) const { return e.value; }
    T&       value(Entry& e) { return e.value; }
    T const& value(HeapIdx i) const { return value(entry(i)); }
    T&       value(HeapIdx i) { return value(entry(i)); }

    Entry const& entry(HeapIdx i) const { return m_heap[i.get()]; }
    Entry&       entry(HeapIdx i) { return m_heap[i.get()]; }
    Entry const& entry(EntryIdx i) const { return entry(heap_idx(i)); }
    Entry&       entry(EntryIdx i) { return entry(heap_idx(i)); }

    HeapIdx const& heap_idx(EntryIdx i) const { return m_e2h[i.get()]; }
    HeapIdx&       heap_idx(EntryIdx i) { return m_e2h[i.get()]; }
    HeapIdx const& heap_idx(Entry const& e) const { return heap_idx(e.idx); }
    HeapIdx&       heap_idx(Entry const& e) { return heap_idx(e.idx); }

    //--------------------------------------------------------------------------
    bool compare(T const& lhs, T const& rhs) const {
        auto const& comp = m_compare;
        return comp(lhs, rhs);
    }

    //--------------------------------------------------------------------------
    void assert_invariants();

    /*----- data members -----*/

    /// \brief The heap used to prioritize elements in the queue.
    std::vector<Entry> m_heap;

    /// \brief Map for EntryIdx to HeapIdx
    std::vector<HeapIdx> m_e2h;

    /// \brief Vector of "freed" EntryIdx.
    ///
    /// Freed EntryIdx will be used on next call to push.
    std::vector<EntryIdx> m_freed;

    /// \brief The compare function used to compare the values in the queue.
    Compare m_compare;
};

//------------------------------------------------------------------------------
//
template <typename T, typename Compare>
inline void PriorityQueue<T, Compare>::assert_invariants() {
#ifndef NDEBUG
    for (auto const& e : m_heap) {
        assert(isValid(e.idx));
    }
    size_type count = 0;
    for (auto h_idx : m_e2h) {
        if (!h_idx) continue;
        assert(h_idx.get() < m_heap.size());
        assert(isValid(entry(h_idx).idx));
        ++count;
    }
    assert(count == m_heap.size());
#endif
}

} // namespace cbl::internal

/// \endcond

#endif
