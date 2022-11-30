#ifndef CBL_IO_STREAMABLE_H
#define CBL_IO_STREAMABLE_H

#include <utility>

namespace cbl {

//==============================================================================
// CLASS Streamable
//==============================================================================

/// \brief Helper class to output anything to an output stream.
template <typename Func>
class Streamable {
public:
    constexpr explicit Streamable(Func&& func)
        : m_func(std::forward<Func>(func)) {}
    constexpr Streamable(Streamable&&) noexcept = default;

    Streamable()                  = delete;
    Streamable(Streamable const&) = delete;
    Streamable& operator=(Streamable const&) = delete;
    Streamable& operator=(Streamable&&) = delete;

    template <typename Os>
    friend constexpr Os& operator<<(Os& os, Streamable const& streamable) {
        auto const& f = streamable.m_func;
        f(os);
        return os;
    }

private:
    Func m_func;
};

//------------------------------------------------------------------------------
//
template <typename Func>
constexpr auto make_streamable(Func&& func) {
    return Streamable<Func>{std::forward<Func>(func)};
}

} // namespace cbl

#endif
