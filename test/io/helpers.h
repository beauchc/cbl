#ifndef CBL_TEST_IO_HELPERS_H
#define CBL_TEST_IO_HELPERS_H

#include <cbl/io/ostream.h>

#include <gtest/gtest.h>

namespace test_io_helpers {
template <typename T>
constexpr bool is_iterable_v =
    cbl::internal::ostream_details::is_iterable<T>::value;

template <typename... Args>
std::string str(Args&&... args) {
    std::stringstream ss;
    cbl::ostream      os{ss};
    ((os << std::forward<Args>(args)), ...);
    return ss.str();
}
} // namespace test_io_helpers

#endif
