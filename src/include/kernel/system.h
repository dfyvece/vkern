/* bkerndev - Bran's Kernel Development Tutorial
*  By:   Brandon F. (friesenb@gmail.com)
*  Desc: Global function declarations and type definitions
*
*  Notes: No warranty expressed or implied. Use at own risk. */
#ifndef __SYSTEM_H
#define __SYSTEM_H

#include <stdint.h>
#include <stdbool.h>
#include <stddef.h>

/* This defines what the stack looks like after an ISR was running */
struct regs {
    uint32_t gs, fs, es, ds;
    uint32_t edi, esi, ebp, esp, ebx, edx, ecx, eax;
    uint32_t int_no, err_code;
    uint32_t eip, cs, eflags, useresp, ss;    
};

/* SYSTEM.C */
extern uint8_t inportb (uint16_t _port);
extern void outportb (uint16_t _port, uint8_t _data);

/* CONSOLE.C */
extern void init_video(void);
extern void puts(uint8_t *text);
extern void putch(uint8_t c);
extern void cls();

/* KEYBOARD.C */
extern void keyboard_install();
extern uint8_t getch();

/* GDT.C */
extern void gdt_set_gate(uint32_t num, uint64_t base, uint64_t limit, uint8_t access, uint8_t gran);
extern void gdt_install();

/* IDT.C */
extern void idt_set_gate(uint8_t num, uint64_t base, uint16_t sel, uint8_t flags);
extern void idt_install();

/* ISRS.C */
extern void isrs_install();

/* IRQ.C */
extern void irq_install_handler(uint32_t irq, void (*handler)(struct regs *r));
extern void irq_uninstall_handler(uint32_t irq);
extern void irq_install();

/* TIMER.C */
extern void timer_install();
extern void context_switch();

#endif
