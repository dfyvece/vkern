#include <string.h>
#include <stdio.h>

#define PROMPT ">"

void vcsh() {
    char buffer[512];
    for(;;) {
        printf("%s ", PROMPT); 
        scanf("%s", buffer);
    }
}
