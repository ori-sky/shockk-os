#ifndef _STDINT_H
#define _STDINT_H 1

// signed exact width types

#ifdef __INT8_TYPE__
#undef __INT_MAX_TYPE__
#define __INT_MAX_TYPE__ __INT8_TYPE__
typedef __INT8_TYPE__ int8_t;
#endif

#ifdef __INT16_TYPE__
#undef __INT_MAX_TYPE__
#define __INT_MAX_TYPE__ __INT16_TYPE__
typedef __INT16_TYPE__ int16_t;
#endif

#ifdef __INT32_TYPE__
#undef __INT_MAX_TYPE__
#define __INT_MAX_TYPE__ __INT32_TYPE__
typedef __INT32_TYPE__ int32_t;
#endif

#ifdef __INT64_TYPE__
#undef __INT_MAX_TYPE__
#define __INT_MAX_TYPE__ __INT64_TYPE__
typedef __INT64_TYPE__ int64_t;
#endif

// unsigned exact width types

#ifdef __UINT8_TYPE__
#undef __UINT_MAX_TYPE__
#define __UINT_MAX_TYPE__ __UINT8_TYPE__
typedef __UINT8_TYPE__ uint8_t;
#endif

#ifdef __UINT16_TYPE__
#undef __UINT_MAX_TYPE__
#define __UINT_MAX_TYPE__ __UINT16_TYPE__
typedef __UINT16_TYPE__ uint16_t;
#endif

#ifdef __UINT32_TYPE__
#undef __UINT_MAX_TYPE__
#define __UINT_MAX_TYPE__ __UINT32_TYPE__
typedef __UINT32_TYPE__ uint32_t;
#endif

#ifdef __UINT64_TYPE__
#undef __UINT_MAX_TYPE__
#define __UINT_MAX_TYPE__ __UINT64_TYPE__
typedef __UINT64_TYPE__ uint64_t;
#endif

// signed least width types

#ifdef __INT_LEAST8_TYPE__
typedef __INT_LEAST8_TYPE__ int_least8_t;
#endif

#ifdef __INT_LEAST16_TYPE__
typedef __INT_LEAST16_TYPE__ int_least16_t;
#endif

#ifdef __INT_LEAST32_TYPE__
typedef __INT_LEAST32_TYPE__ int_least32_t;
#endif

#ifdef __INT_LEAST64_TYPE__
typedef __INT_LEAST64_TYPE__ int_least64_t;
#endif

// unsigned least width types

#ifdef __UINT_LEAST8_TYPE__
typedef __UINT_LEAST8_TYPE__ uint_least8_t;
#endif

#ifdef __UINT_LEAST16_TYPE__
typedef __UINT_LEAST16_TYPE__ uint_least16_t;
#endif

#ifdef __UINT_LEAST32_TYPE__
typedef __UINT_LEAST32_TYPE__ uint_least32_t;
#endif

#ifdef __UINT_LEAST64_TYPE__
typedef __UINT_LEAST64_TYPE__ uint_least64_t;
#endif

// signed fast width types

#ifdef __INT_FAST8_TYPE__
typedef __INT_FAST8_TYPE__ int_fast8_t;
#endif

#ifdef __INT_FAST16_TYPE__
typedef __INT_FAST16_TYPE__ int_fast16_t;
#endif

#ifdef __INT_FAST32_TYPE__
typedef __INT_FAST32_TYPE__ int_fast32_t;
#endif

#ifdef __INT_FAST64_TYPE__
typedef __INT_FAST64_TYPE__ int_fast64_t;
#endif

// unsigned fast width types

#ifdef __UINT_FAST8_TYPE__
typedef __UINT_FAST8_TYPE__ uint_fast8_t;
#endif

#ifdef __UINT_FAST16_TYPE__
typedef __UINT_FAST16_TYPE__ uint_fast16_t;
#endif

#ifdef __UINT_FAST32_TYPE__
typedef __UINT_FAST32_TYPE__ uint_fast32_t;
#endif

#ifdef __UINT_FAST64_TYPE__
typedef __UINT_FAST64_TYPE__ uint_fast64_t;
#endif

// signed pointer type

#ifdef __INTPTR_TYPE__
typedef __INTPTR_TYPE__ intptr_t;
#endif

// unsigned pointer type

#ifdef __UINTPTR_TYPE__
typedef __UINTPTR_TYPE__ uintptr_t;
#endif

// signed max width type

#ifdef __INT_MAX_TYPE__
typedef __INT_MAX_TYPE__ intmax_t;
#endif

// unsigned max width type

#ifdef __UINT_MAX_TYPE__
typedef __UINT_MAX_TYPE__ uintmax_t;
#endif

#endif
