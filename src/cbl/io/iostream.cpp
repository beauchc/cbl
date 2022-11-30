#include <cbl/io/iostream.h>

#include <iostream>

namespace cbl {
ostream cerr{std::cerr};
ostream cout{std::cout};

#ifndef NDEBUG
ostream dbgs{std::cerr};
#endif

} // namespace cbl
