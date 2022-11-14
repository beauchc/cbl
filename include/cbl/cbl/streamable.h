#ifndef CBL_STREAMABLE_H
#define CBL_STREAMABLE_H

#include <utility>

namespace cbl {

//==============================================================================
// CLASS Streamable
//==============================================================================

/// \brief Helper class to output anything to an output stream.
template <typename Func>
class Streamable {
public:
    constexpr Streamable(Func func) : m_func(std::move(func)) {}

    template <typename Os>
    friend constexpr Os& operator<<(Os& os, Streamable const& streamable) {
        streamable.m_func(os);
        return os;
    }

private:
    Func m_func;
};

//------------------------------------------------------------------------------
//
template <typename Func>
constexpr auto make_streamable(Func func) {
    return Streamable<Func>{std::move(func)};
}

} // namespace cbl

#endif
