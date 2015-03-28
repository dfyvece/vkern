#include <system.h>
#include <string.h>
#include <stdlib.h>
#include <stdio.h>

void kernel_main() {

    gdt_install();
    idt_install();
    isrs_install();
    irq_install();
    init_video();
    timer_install();
    keyboard_install();
    __asm__ __volatile__ ("sti");

    char word1[256];
    char word2[256];

    int num;

    char c;

    printf("Welcome to VCOS\n");
    vcsh();
    for (;;);
}
