#define TR(...) static_assert(true)
#define TRV(v) TR()
#define TRS TR()
#define TR_BLOCK(code) TR()
#define TR_INDENT(...) TR()

#define CBL_DEBUG_TRACE_ENABLED 1
#if CBL_DEBUG_TRACE_ENABLED
#include <cbl/debug_trace.h>
#endif
