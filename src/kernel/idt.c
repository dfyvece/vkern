#include <system.h>

struct idt_entry {
    uint16_t base_lo;
    uint16_t sel;
    uint8_t always0;
    uint8_t flags;
    uint16_t base_hi;
} __attribute__((packed));

struct idt_ptr {
    uint16_t limit;
    uint32_t base;
} __attribute__((packed));

struct idt_entry idt[256];
struct idt_ptr idtp;

extern void idt_load(); // loads the IDT

void idt_set_gate(uint8_t num, uint64_t base, uint16_t sel, uint8_t flags) {

    // base address
    idt[num].base_lo = (base & 0xFFFF);
    idt[num].base_hi = (base >> 16) & 0xFFFF;

    // selector and flags
    idt[num].sel = sel;
    idt[num].always0 = 0;
    idt[num].flags = flags;
}

void idt_install() {

    idtp.limit = (sizeof (struct idt_entry) * 256) - 1;
    idtp.base = &idt;

    // clear IDT
    memset(&idt, 0, sizeof(struct idt_entry) * 256);

    idt_load();
}
