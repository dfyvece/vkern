#include <string.h>

void *memcpy(void *dest, const void *src, size_t count)
{
    const char *sp = (const char *)src;
    char *dp = (char *)dest;
    for(; count != 0; count--) *dp++ = *sp++;
    return dest;
}

void *memset(void *dest, char val, size_t count)
{
    char *temp = (char *)dest;
    for( ; count != 0; count--) *temp++ = val;
    return dest;
}

unsigned short *memsetw(unsigned short *dest, unsigned short val, size_t count)
{
    unsigned short *temp = (unsigned short *)dest;
    for( ; count != 0; count--) *temp++ = val;
    return dest;
}


size_t strlen(const char *str)
{
    size_t retval;
    for(retval = 0; *str != '\0'; str++) retval++;
    return retval;
}

int islower(unsigned char c) {
    return (c >= 97 && c <= 122);
}

int isupper(unsigned char c) {
    return (c >= 65 && c <= 90);
}

int isalpha(unsigned char c) {
    return (islower(c) || isupper(c));
}

int isspace(unsigned char c) {
    return (c == ' ' || c == '\t' || c == '\n');
}

int isdigit(unsigned char c) {
    return (c == '0' ||
            c == '1' ||
            c == '2' ||
            c == '3' ||
            c == '4' ||
            c == '5' ||
            c == '6' ||
            c == '7' ||
            c == '8' ||
            c == '9');
}

int isalnum(unsigned char c) {
    return (isalpha(c) || isdigit(c));
}

unsigned char toupper(unsigned char c) {
    if (c < 97 || c > 122)
        return c;
    return c-32;
}

unsigned char tolower(unsigned char c) {
    if (c < 65 || c > 90)
        return c;
    return c+32;

}


