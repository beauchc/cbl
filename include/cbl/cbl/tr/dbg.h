#ifndef CBL_TR_DBG_H
#define CBL_TR_DBG_H

#include <cbl/tr/internal/dbg_internal.h>

#define TR_OS std::cerr
#define TR(...) CBL_INTERNAL_TR(TR_OS, __VA_ARGS__)
#define TRC(...) CBL_INTERNAL_TRC(__VA_ARGS__) // code block

#define TRS(...) CBL_INTERNAL_TRS(__VA_ARGS__) // separator
#define TRV(expr) CBL_INTERNAL_TRV(expr)       // variable/expression
#define TRI(...) CBL_INTERNAL_TRI(__VA_ARGS__) // indent block

#endif
