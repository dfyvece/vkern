export CPATH=$HOME/opt/cross
export CC=$CPATH/bin/i686-elf-gcc
export CCINCLUDE=$CPATH/lib/gcc/i686-elf/4.9.2/include
export CCINCLUDE_FIXED=$CPATH/lib/gcc/i686-elf/4.9.2/include-fixed
includes=()
for folder in `ls src/include`; do
    if [ -d src/include/$folder ]; then
        includes+=(-I`pwd`/src/include/$folder)
    fi
done
export CFLAGS="-w -O -fstrength-reduce -fomit-frame-pointer -finline-functions -nostdinc -fno-builtin"
export CFLAGS="$CFLAGS ${includes[@]} -I$CCINCLUDE -I$CCINCLUDE_FIXED"
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

function assemble {
    local CWD=`pwd`
    cd arch/$ARCH
    for file in `ls`; do
        if [[ $file == *.asm || $file == *.s ]]; then
            echo "Assembling $file"
            export out=`echo $file | sed -e 's/\.asm/.o/' -e 's/\.s/.o/'`
            $AS -f elf32 -o $BUILDDIR/$out $file || exit
        fi
    done
    cd $CWD
}


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
            if [[ $file == arch ]]; then
                if [ -d arch/$ARCH ]; then
                    assemble
                else 
                    echo "Error: missing architecture for `pwd`/$ARCH"
                    exit
                fi
            else
                compile $file
            fi
        fi
    done
    cd $CWD
}

cd src/

echo "Building kernel libraries"
compile `pwd`
#for entry in `ls`; do
#    compile $entry
#done

echo "Linking files"
export BUILDS=`ls $BUILDDIR | sed -e "s_\(\w*\).\(\w*\)_$BUILDDIR/\1.\2_g"`
$LD --script arch/$ARCH/link.ld -o $SWD/kernel.bin $BUILDS || exit
echo "Completed build"

echo "Generating ISO"
cd $SWD
cp kernel.bin isodir/boot/ 
grub-mkrescue -o boot.iso isodir/
