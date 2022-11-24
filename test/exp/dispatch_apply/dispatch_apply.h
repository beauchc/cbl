#include <array>
#include <variant>

//------------------------------------------------------------------------------
//
using Ints = std::array<int, 4>;
using Flts = std::array<float, 4>;

using Iterator = std::variant<Ints*, Flts*>;

template <typename... Params>
struct Signature {};

namespace apply_impl {

//------------------------------------------------------------------------------
//
template <typename P, typename It>
struct Accessor {
    static constexpr bool is_output =
        std::is_reference_v<P> && !std::is_const_v<std::remove_reference_t<P>>;

    Accessor(It begin, It end) : it(begin), end(end) {}
    Accessor(Accessor const& o) : Accessor(o.it, o.end) {}

    size_t size() const { return std::distance(it, end); }

    void get_range(size_t start, size_t size) {
        it  = it + start;
        end = it + size;
    }

    /*implicit*/ operator P() { // implicit conversion operator
        if constexpr (is_output) {
            return m_buffer;
        } else {
            return static_cast<P>(*it);
        }
    }
    void operator++() {
        if constexpr (is_output)
            *it = static_cast<std::remove_reference_t<decltype(*it)>>(m_buffer);
        ++it;
    }
    It it;
    It end;

    using Buffer = std::
        conditional_t<is_output, std::remove_reference_t<P>, std::nullptr_t>;
    Buffer m_buffer{};
};

template <typename P, typename Range>
auto make_accessor(Range* rng) {
    auto begin = std::begin(*rng), end = std::end(*rng);
    return Accessor<P, decltype(begin)>(begin, end);
}

//------------------------------------------------------------------------------
//
template <typename F>
auto make_range_trampoline(F tramp) {
    return [f = std::move(tramp)](size_t start, size_t size, auto... accs) {
        (accs.get_range(start, size), ...);
        for (size_t i = 0; i < size; ++i) {
            f(accs...);
            (++accs, ...);
        }
    };
}

template <typename F, typename... Accs>
void apply_loop(F tramp, size_t grain, size_t size, Accs... accs) {
    auto rtramp = make_range_trampoline(std::move(tramp));
    // This is sequential, but each chunk could be given to different threads.
    // (assuming the trampoline's body
    for (size_t i = 0; i < size; i += grain) {
        auto s = std::min(grain, size - i);
        rtramp(i, s, accs...);
    }
}

//------------------------------------------------------------------------------
//
template <typename P, typename... Ps, typename F, typename... Args>
void apply(
    Signature<P, Ps...>, F tramp, size_t grain, Iterator it, Args&&... args);

template <typename F, typename Acc, typename... Accs>
void apply(Signature<>, F tramp, size_t grain, Acc acc, Accs... accs) {
    auto size = acc.size();
    apply_loop(std::move(tramp), grain, size, acc, accs...);
}

template <typename P, typename... Ps, typename F, typename... Args>
void apply(
    Signature<P, Ps...>, F tramp, size_t grain, Iterator it, Args&&... its) {
    auto visitor = [&, grain](auto* arr) -> void {
        apply(Signature<Ps...>{}, std::move(tramp), grain,
              std::forward<Args>(its)..., make_accessor<P>(arr));
    };
    std::visit(visitor, it);
}

//------------------------------------------------------------------------------
//
template <typename... Args, typename F>
auto make_trampoline(Signature<Args...>, F const& func) {
    // The trampoline is necessary in case func has templated arguments.
    // We also need to force the implicit conversion operator to be called
    // which only happens if the argument it converts to is of the exact
    // required type.
    return [&func](Args... args) { return func(std::forward<Args>(args)...); };
}
} // namespace apply_impl

//------------------------------------------------------------------------------
//
template <typename Signature, typename Func, typename... Args>
void apply(Func const& func, size_t grain, Args&&... args) {
    apply_impl::apply( //
        Signature{}, apply_impl::make_trampoline(Signature{}, func), grain,
        std::forward<Args>(args)...);
}
