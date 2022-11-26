#ifndef CBL_PARTITIONS_H
#define CBL_PARTITIONS_H

#include <vector>

namespace cbl {

//==============================================================================
// CLASS Partitions
//==============================================================================

class Partitions {
public:
    explicit Partitions() : m_partitions{~unsigned()} {};
    explicit Partitions(size_t capacity);

    unsigned create();
    unsigned get(unsigned pidx);
    unsigned merge(unsigned pidx1, unsigned pidx2);
    unsigned merge_or_create(unsigned pidx1, unsigned pidx2);
    unsigned count() const { return m_count; }

private:
    std::vector<unsigned> m_partitions;
    unsigned              m_count = 0;
};

//------------------------------------------------------------------------------
//
inline Partitions::Partitions(size_t capacity) {
    m_partitions.reserve(capacity + 1);
    m_partitions.emplace_back(~unsigned());
}
inline unsigned Partitions::create() {
    auto pidx = m_partitions.size();
    m_partitions.emplace_back(pidx);
    ++m_count;
    return pidx;
}
inline unsigned Partitions::get(unsigned pidx) {
    if (m_partitions[pidx] == pidx) return pidx;
    return (m_partitions[pidx] = get(m_partitions[pidx]));
}
inline unsigned Partitions::merge(unsigned pidx1, unsigned pidx2) {
    auto aidx1 = get(pidx1);
    auto aidx2 = get(pidx2);
    if (aidx1 == aidx2) return aidx1;
    --m_count;
    return (m_partitions[aidx1] = m_partitions[aidx2] = std::min(aidx1, aidx2));
}
inline unsigned Partitions::merge_or_create(unsigned pidx1, unsigned pidx2) {
    if (!pidx1 && !pidx2) return create();
    if (!pidx1) return get(pidx2);
    if (!pidx2) return get(pidx1);
    return merge(pidx1, pidx2);
}

} // namespace cbl

#endif
