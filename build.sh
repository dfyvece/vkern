export CPATH=$HOME/opt/cross
export CC=$CPATH/bin/i686-elf-gcc
export CCINCLUDE=$CPATH/lib/gcc/i686-elf/4.9.2/include
export CCINCLUDE_FIXED=$CPATH/lib/gcc/i686-elf/4.9.2/include-fixed
includes=(-I`pwd`/src/kernel/include -I`pwd`/src/libc/include)
export CFLAGS="-w -O -fstrength-reduce -fomit-frame-pointer -finline-functions -nostdinc -fno-builtin -I./include ${includes[@]} -I$CCINCLUDE -I$CCINCLUDE_FIXED"
export AS=nasm
export LD=$CPATH/bin/i686-elf-ld

export BUILDDIR=`pwd`/objs
mkdir -p $BUILDDIR

export SWD=`pwd`

if [ -z "$1" ]; then
    export ARCH=x86
else
    export ARCH=$1
fi

if [ -d src/arch/$ARCH ]; then
    echo "Found architecture files"
else
    echo "Error: No support for architecture '$ARCH'"
    exit
fi


function compile {
    local CWD=`pwd`
    cd $1
    for file in `ls`; do
        if [ -f $file ]; then
            if [[ $file == *.c ]]; then
                echo "Compiling $file"
                local out=`echo $file | sed -e 's/\.c/.o/'`
                $CC -c $CFLAGS -o $BUILDDIR/$out $file || exit
            fi
        elif [ -d $file ]; then
            compile $file
        fi
    done
    cd $CWD
}

echo "Entering source directories"
cd src/

echo "Building kernel libraries"
for entry in `ls`; do
    if [ $entry != "arch" ]; then
        compile $entry
    fi
done

echo "Compiling architecture specific code"
cd arch/$ARCH
for file in `ls`; do
    if [[ $file == *.asm || $file == *.s ]]; then
        echo "Assembling $file"
        export out=`echo $file | sed -e 's/\.asm/.o/' -e 's/\.s/.o/'`
        $AS -f elf32 -o $BUILDDIR/$out $file || exit
    fi
done

echo "Linking files"
export BUILDS=`ls $BUILDDIR | sed -e "s_\(\w*\).\(\w*\)_$BUILDDIR/\1.\2_g"`
$LD --script link.ld -o $SWD/kernel.bin $BUILDS || exit
echo "Completed build"

echo "Generating ISO"
cd $SWD
cp kernel.bin isodir/boot/ 
grub-mkrescue -o boot.iso isodir/
