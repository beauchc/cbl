#ifndef CBL_IO_TRACING_H
#define CBL_IO_TRACING_H

#include <cbl/io/iostream.h>

#include <cbl/io/internal/tracing_internal.h>

// cbl::ostream to use for tracing.
// can be redefined if needed.
#define TR_OS cbl::dbgs

// TR and macros that can be used inside TR blocks
#undef TR
#define TR(...) CBL_INTERNAL_TR(TR_OS, __VA_ARGS__)
#define TRS(...) CBL_INTERNAL_TRS(__VA_ARGS__) // separator
#define TRV(expr) CBL_INTERNAL_TRV(expr)       // variable/expression

// TRC and macros that can be used inside TRC blocks
#undef TRC
#define TRC(...) CBL_INTERNAL_TRC(__VA_ARGS__)        // code block
#define TRI(...) CBL_INTERNAL_TRI(TR_OS, __VA_ARGS__) // indent block

#endif
