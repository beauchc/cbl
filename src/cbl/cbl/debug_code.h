#ifndef CBL_DEBUG_CODE_H
#define CBL_DEBUG_CODE_H

#ifndef NDEBUG
#define CBL_DEBUG_CODE(code) code
#define CBL_DEBUG_SELECT(Dbg, Rel) Dbg
#else
#define CBL_DEBUG_CODE(code) static_assert(true)
#define CBL_DEBUG_SELECT(Dbg, Rel) Rel
#endif

#endif
