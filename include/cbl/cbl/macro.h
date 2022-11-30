#ifndef CBL_MACRO_H
#define CBL_MACRO_H

#ifndef NDEBUG
#define CBL_DEBUG_SELECT(A, B) A
#define CBL_DEBUG_CODE(A) A
#else
#define CBL_DEBUG_SELECT(A, B) B
#define CBL_DEBUG_CODE(A)
#endif

#endif
