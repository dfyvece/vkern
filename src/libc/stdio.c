#include <stdarg.h>
#include <stdio.h>
#include <string.h>

static void print(const char* data, size_t data_length) {
    size_t i;
	for ( i = 0; i < data_length; i++ )
		putch((int) ((const unsigned char*) data)[i]);
}

char conversion[] = {'0','1','2','3','4','5','6','7','8','9'};

int printf(const char* format, ...) {
	va_list parameters;
	va_start(parameters, format);

	int written = 0;
	size_t amount;
	bool rejected_bad_specifier = false;

	while ( *format != '\0' ) {
		if ( *format != '%' ) {
		print_c:
			amount = 1;
			while ( format[amount] && format[amount] != '%' )
				amount++;
			print(format, amount);
			format += amount;
			written += amount;
			continue;
		}

		const char* format_begun_at = format;

		if ( *(++format) == '%' )
			goto print_c;

		if ( rejected_bad_specifier ) {
		incomprehensible_conversion:
			rejected_bad_specifier = true;
			format = format_begun_at;
			goto print_c;
		}

		if ( *format == 'c' ) {
			format++;
			char c = (char) va_arg(parameters, int /* char promotes to int */);
			print(&c, sizeof(c));
		}
		else if ( *format == 's' ) {
			format++;
			const char* s = va_arg(parameters, const char*);
			print(s, strlen(s));
		}
		else if ( *format == 'u' ) {
			format++;
			unsigned int d = va_arg(parameters, unsigned int);
            if (d == 0) {
                putch('0');
                continue;
            }
            int i = 0;
            int c = 0;
            while (d && ++i) {
                c = d%10;
                d /= 10;
                __asm__ __volatile__("sub $4, %%esp;"
                                     "push %%ebx;"
                                     "movl %0, %%ebx;"
                                     "movl %%ebx, 4(%%esp);"
                                     "pop %%ebx;"
                                     :: "a" (c));

            }
            while (--i+1) {
                __asm__ __volatile__("push %%ebx;"
                                     "movl 4(%%esp), %%ebx;"
                                     "movl %%ebx, %0;"
                                     "pop %%ebx;"
                                     "add $4, %%esp"
                                     : "=a" (c));
                putch(conversion[c]);
            }
		}
        else if ( *format == 'd' ) {
			format++;
			int d = va_arg(parameters, int);
            if (d == 0) {
                putch('0');
                continue;
            }
            int i = 0;
            int c = 0;
            if (d < 0) {
                d *= -1;
                putch('-');
            }
            while (d && ++i) {
                c = d%10;
                d /= 10;
                __asm__ __volatile__("sub $4, %%esp;"
                                     "push %%ebx;"
                                     "movl %0, %%ebx;"
                                     "movl %%ebx, 4(%%esp);"
                                     "pop %%ebx;"
                                     :: "a" (c));

            }
            while (--i+1) {
                __asm__ __volatile__("push %%ebx;"
                                     "movl 4(%%esp), %%ebx;"
                                     "movl %%ebx, %0;"
                                     "pop %%ebx;"
                                     "add $4, %%esp"
                                     : "=a" (c));
                putch(conversion[c]);
            }
		}
		else {
			goto incomprehensible_conversion;
		}
	}

	va_end(parameters);

	return written;
}

inline char get_and_put() {
    char c = getch();
    putch(c);
    return c;
}

void scanf(const char* format, ...) {
	va_list parameters;
	va_start(parameters, format);
    char c;

	bool rejected_bad_specifier = false;

	while ( *format != '\0' ) {
		if ( *format != '%' ) {
		get_c:
            do {
                c = get_and_put();
            } while ( *format != c );
            ++format;
			continue;
		}

		const char* format_begun_at = format;

		if ( *(++format) == '%' )
			goto get_c;

		if ( rejected_bad_specifier ) {
		incomprehensible_conversion:
			rejected_bad_specifier = true;
			format = format_begun_at;
			goto get_c;
		}

		if ( *format == 'c' ) {
			format++;
			char* c_ptr = va_arg(parameters, char*);
            c = get_and_put();
            *c_ptr = c;
		}
		else if ( *format == 's' ) {
			format++;
			char* s = va_arg(parameters, char*);
            unsigned int i;
            c = get_and_put();
            for (i = 0; !(isspace(c)); c = get_and_put(), ++i)
                s[i] = c;
            s[i] = 0;
            
		}
		else if ( *format == 'd' ) {
			format++;
			int* num = va_arg(parameters, int*);
            *num = 0;
            c = get_and_put();
            bool is_negative = false;
            if (c == '-') {
                is_negative = true;
                c = get_and_put();;
            }
            for (;(isdigit(c)); c = get_and_put()) {
                *num *= 10;
                *num += c-48;
            }
            if (is_negative)
                *num *= -1;
        }
		else {
			goto incomprehensible_conversion;
		}
	}

	va_end(parameters);

    while (c != '\n')
        c = get_and_put();

}
