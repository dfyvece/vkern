#include <string.h>
#include <stdio.h>
#include <system.h>

#define PROMPT ">"

void vcsh() {
    char buffer[512];
    char a1[512];
    char a2[512];
    char a3[512];
    int d;
    for(;;) {
        printf("%s ", PROMPT); 
        scanf("%s", buffer);
        switch (buffer[0]) {
        case 'e':
            scanf("%s", a1);
            printf("%s\n", a1);
            break;
        case 'd':
            scanf("%d", &d);
            printf("You entered digit %d\n", d);
            break;
        }
    }
}
