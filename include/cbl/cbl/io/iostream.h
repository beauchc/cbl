#ifndef CBL_IO_IOSTREAM_H
#define CBL_IO_IOSTREAM_H

#include <cbl/io/ostream.h>

namespace cbl {
extern ostream cerr;
extern ostream cout;

#ifndef NDEBUG
extern ostream dbgs;
#endif
} // namespace cbl

#endif
