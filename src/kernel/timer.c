#include <system.h>

uint32_t ticks = 0;
volatile uint32_t freq = 18;

void timer_handler(struct regs *r) {
    //printf("Called %d\n", ++ticks);
    ticks = ++ticks % freq;
    if (ticks == 0) printf("1 second has passed\n");
    //context_switch();
}

void timer_freq(uint32_t hz) {
    freq = hz;
    uint32_t divisor = 1193180 / hz;
    outportb(0x43, 0x36);
    outportb(0x40, divisor & 0xFF);
    outportb(0x40, divisor >> 8);
}


void timer_install() {
    timer_freq(200);
    irq_install_handler(0, timer_handler);
}
