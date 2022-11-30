#ifndef CBL_IO_TRACING_H
#define CBL_IO_TRACING_H

#include <cbl/io/internal/tracing_internal.h>

#undef TR
#undef TRC

#define TR_OS cbl::dbgs

// TR and TRX that can be used inside TR block
#define TR(...) CBL_INTERNAL_TR(TR_OS, __VA_ARGS__)
#define TRS(...) CBL_INTERNAL_TRS(__VA_ARGS__) // separator
#define TRV(expr) CBL_INTERNAL_TRV(expr)       // variable/expression

// TRC and TRX that can be used inside TR block
#define TRC(...) CBL_INTERNAL_TRC(__VA_ARGS__) // code block
#define TRI(...) CBL_INTERNAL_TRI(__VA_ARGS__) // indent block

#endif
