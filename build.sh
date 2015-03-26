export CPATH=$HOME/opt/cross
export CC=$CPATH/bin/i686-elf-gcc
export CCINCLUDE=$CPATH/lib/gcc/i686-elf/4.9.2/include
export CCINCLUDE_FIXED=$CPATH/lib/gcc/i686-elf/4.9.2/include-fixed
export CFLAGS="-w -O -fstrength-reduce -fomit-frame-pointer -finline-functions -nostdinc -fno-builtin -I./include -I$CCINCLUDE -I$CCINCLUDE_FIXED"
export AS=nasm
export LD=$CPATH/bin/i686-elf-ld

export SRCDIR=src
export LIBCDIR=$SRCDIR/libc
export BUILDDIR=build

KERNEL_SRC=( main scrn gdt idt isrs irq timer vcsh kb system )
LIBC_SRC=(stdio stdlib string)

mkdir -p $(BUILDDIR)

echo "Building entry point"
$AS -f elf32 -o $BUILDDIR/start.o $SRCDIR/start.asm &&
echo "Building system files" &&
for file in ${KERNEL_SRC[*]}; do
	$CC $CFLAGS -c -o $BUILDDIR/$file.o $SRCDIR/$file.c
done &&
echo "Building libc files" &&
for file in ${LIBC_SRC[*]}; do
	$CC $CFLAGS -c -o $BUILDDIR/$file.o $LIBCDIR/$file.c
done &&
echo "Generating kernel" &&
for i in ${!KERNEL_SRC[*]}; do
	KERNEL_SRC[i]=$BUILDDIR/${KERNEL_SRC[i]}.o
done &&
for i in ${!LIBC_SRC[*]}; do
	LIBC_SRC[i]=$BUILDDIR/${LIBC_SRC[i]}.o
done &&
$LD --script link.ld -o kernel.bin $BUILDDIR/start.o \
	${KERNEL_SRC[@]} ${LIBC_SRC[@]} &&
cp kernel.bin isodir/boot/  &&
grub-mkrescue -o boot.iso isodir/ &&
echo "Completed build"
