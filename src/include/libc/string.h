#ifndef __STRING_H
#define __STRING_H 1

#include <stdint.h>
#include <stdbool.h>
#include <stddef.h>

extern void *memcpy(void *dest, const void *src, size_t count);
extern void *memset(void *dest, char val, size_t count);
extern unsigned short *memsetw(unsigned short *dest, unsigned short val, size_t count);

extern size_t strlen(const char *str);
extern int isupper(unsigned char c);
extern int islower(unsigned char c);
extern int isalpha(unsigned char c);
extern int isspace(unsigned char c);
extern int isalnum(unsigned char c);
extern int isdigit(unsigned char c);
extern unsigned char toupper(unsigned char c);
extern unsigned char tolower(unsigned char c);

#endif
