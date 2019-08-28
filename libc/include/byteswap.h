#ifndef _BYTESWAP_H
#define _BYTESWAP_H 1

#define bswap_16(X) ((((X) >> 8) & 0xFF) | (((X) & 0xFF) << 8))

#endif
