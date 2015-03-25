#ifndef __STRING_H
#define __STRING_H 1

#include <system.h>

extern void *memcpy(void *dest, const void *src, size_t count);
extern void *memset(void *dest, char val, size_t count);
extern unsigned short *memsetw(unsigned short *dest, unsigned short val, size_t count);

extern size_t strlen(const char *str);
extern bool isupper(unsigned char c);
extern bool islower(unsigned char c);
extern bool isalpha(unsigned char c);
extern unsigned char toupper(unsigned char c);
extern unsigned char tolower(unsigned char c);

#endif
