#ifndef _INTTYPES_H
#define _INTTYPES_H 1

#include <stdint.h>

#define PRId8  "hhd"
#define PRId16 "hd"
#define PRId32 "ld"
#define PRId64 "lld"

#define PRILEASTd8  PRId8
#define PRILEASTd16 PRId16
#define PRILEASTd32 PRId32
#define PRILEASTd64 PRId64

#define PRIFASTd8  PRId8
#define PRIFASTd16 PRId16
#define PRIFASTd32 PRId32
#define PRIFASTd64 PRId64

#define PRIdMAX PRId64
#define PRIdPTR PRId32

#endif
