#include <utility>

namespace cbl {

//==============================================================================
// CLASS Streamable
//==============================================================================

template <typename Func>
class Streamable {
public:
    Streamable(Func func) : m_func(std::move(func)) {}

    template <typename OStream>
    friend OStream& operator<<(OStream& os, Streamable const& streamable) {
        streamable.m_func(os);
        return os;
    }

private:
    Func m_func;
};

//------------------------------------------------------------------------------
//
template <typename Func>
auto make_streamable(Func func) {
    return Streamable<Func>{std::move(func)};
}

} // namespace cbl
