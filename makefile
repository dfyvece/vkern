CPATH=$(HOME)/opt/cross/bin
CC=$(CPATH)/i686-elf-gcc
CFLAGS=-w -O -fstrength-reduce -fomit-frame-pointer -finline-functions -nostdinc -fno-builtin -I./include
AS=nasm
LD=$(CPATH)/i686-elf-ld

SRCDIR=src
BUILDDIR=build
LIBC=$(SRCDIR)/libc

all: boot.iso

boot.iso: kernel.bin isodir/boot/grub/grub.cfg
	cp kernel.bin isodir/boot/
	grub-mkrescue -o boot.iso isodir/

$(BUILDDIR)/start.o: $(SRCDIR)/start.asm
	$(AS) -f elf32 -o $(BUILDDIR)/start.o $(SRCDIR)/start.asm

$(BUILDDIR)/main.o: $(SRCDIR)/main.c
	$(CC) $(CFLAGS) -c -o $(BUILDDIR)/main.o $(SRCDIR)/main.c

$(BUILDDIR)/scrn.o: $(SRCDIR)/scrn.c
	$(CC) $(CFLAGS) -c -o $(BUILDDIR)/scrn.o $(SRCDIR)/scrn.c

$(BUILDDIR)/gdt.o: $(SRCDIR)/gdt.c
	$(CC) $(CFLAGS) -c -o $(BUILDDIR)/gdt.o $(SRCDIR)/gdt.c

$(BUILDDIR)/idt.o: $(SRCDIR)/idt.c
	$(CC) $(CFLAGS) -c -o $(BUILDDIR)/idt.o $(SRCDIR)/idt.c

$(BUILDDIR)/isrs.o: $(SRCDIR)/isrs.c
	$(CC) $(CFLAGS) -c -o $(BUILDDIR)/isrs.o $(SRCDIR)/isrs.c

$(BUILDDIR)/irq.o: $(SRCDIR)/irq.c
	$(CC) $(CFLAGS) -c -o $(BUILDDIR)/irq.o $(SRCDIR)/irq.c

$(BUILDDIR)/timer.o: $(SRCDIR)/timer.c
	$(CC) $(CFLAGS) -c -o $(BUILDDIR)/timer.o $(SRCDIR)/timer.c

$(BUILDDIR)/vcsh.o: $(SRCDIR)/vcsh.c
	$(CC) $(CFLAGS) -c -o $(BUILDDIR)/vcsh.o $(SRCDIR)/vcsh.c

$(BUILDDIR)/kb.o: $(SRCDIR)/kb.c
	$(CC) $(CFLAGS) -c -o $(BUILDDIR)/kb.o $(SRCDIR)/kb.c

$(BUILDDIR)/system.o: $(SRCDIR)/system.c
	$(CC) $(CFLAGS) -c -o $(BUILDDIR)/system.o $(SRCDIR)/system.c

$(BUILDDIR)/stdio.o: $(LIBC)/stdio.c
	$(CC) $(CFLAGS) -c -o $(BUILDDIR)/stdio.o $(LIBC)/stdio.c

$(BUILDDIR)/stdlib.o: $(LIBC)/stdlib.c
	$(CC) $(CFLAGS) -c -o $(BUILDDIR)/stdlib.o $(LIBC)/stdlib.c

$(BUILDDIR)/string.o: $(LIBC)/string.c
	$(CC) $(CFLAGS) -c -o $(BUILDDIR)/string.o $(LIBC)/string.c


kernel.bin: $(BUILDDIR)/start.o $(BUILDDIR)/main.o $(BUILDDIR)/scrn.o $(BUILDDIR)/gdt.o \
	$(BUILDDIR)/idt.o $(BUILDDIR)/isrs.o $(BUILDDIR)/irq.o $(BUILDDIR)/timer.o $(BUILDDIR)/kb.o \
	$(BUILDDIR)/string.o $(BUILDDIR)/stdlib.o $(BUILDDIR)/system.o $(BUILDDIR)/stdio.o \
	$(BUILDDIR)/vcsh.o
	$(LD) --script link.ld -o kernel.bin $(BUILDDIR)/start.o $(BUILDDIR)/main.o \
	$(BUILDDIR)/scrn.o $(BUILDDIR)/gdt.o $(BUILDDIR)/idt.o $(BUILDDIR)/isrs.o \
	$(BUILDDIR)/stdlib.o $(BUILDDIR)/string.o $(BUILDDIR)/system.o \
	$(BUILDDIR)/irq.o $(BUILDDIR)/timer.o $(BUILDDIR)/kb.o $(BUILDDIR)/stdio.o \
	$(BUILDDIR)/vcsh.o 

clean:
	rm $(BUILDDIR)/*.o kernel.bin
