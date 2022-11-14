#ifndef CBL_POOL_H
#define CBL_POOL_H

#include "debug_code.h"

#include <cassert>
#include <memory>

#if 0
#include <iostream>
#define POOL_LOG(...) log(std::cerr, __VA_ARGS__)
#else
#define POOL_LOG(...) static_assert(true)
#endif

//==============================================================================
// CLASS Pool<T>
//==============================================================================

template <typename T>
class Pool {
public:
    using value_type = T;
    using size_type  = size_t;

    void reset(size_type n) {
        POOL_LOG("reset");
        assert(allocated == 0);
        pool.reset(new T[n]);
        begin = pool.get();
        end   = begin + n;
    }

    void clear() {
        POOL_LOG("clear");
        assert(allocated == 0);
        begin = pool.get();
    }

    void destroy() {
        POOL_LOG("destroy");
        assert(allocated == 0);
        pool.reset();
        begin = end = nullptr;
    }

    T* allocate(size_type n, const void* /*hint*/ = 0) {
        POOL_LOG("allocate ", n);
        assert(begin + n <= end);
        CBL_DEBUG_CODE(allocated += n);
        auto t = begin;
        begin += n;
        return t;
    }

    void deallocate(T* /*p*/, size_type n) {
        POOL_LOG("deallocate ", n);
        assert(allocated >= n);
        CBL_DEBUG_CODE(allocated -= n);
    }

private:
    template <typename Os, typename... Args>
    static void log(Os& os, Args const&... args) {
        auto print = [&os](auto const& a) { os << a; };
        (print(args), ...);
        os << '\n';
        os.flush();
    }

    std::unique_ptr<T[]> pool = nullptr;

    T*        begin     = nullptr;
    T*        end       = nullptr;
    size_type allocated = 0;
};

#undef POOL_LOG

#endif
