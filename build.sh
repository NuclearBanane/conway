#!/bin/bash

set -e # exits immediately if exit 0 somewhere

ESDK=${EPIPHANY_HOME}
ELIBS="-L ${ESDK}/tools/host/lib"
EINCS="-I ${ESDK}/tools/host/include"
ELDF=${ESDK}/bsps/current/internal.ldf
HOST_FILES="src/game.c  src/game.h  src/main.c"

CROSS_PREFIX=
case $(uname -p) in
    arm*)
        # Use native arm compiler (no cross prefix)
        CROSS_PREFIX=
        ;;
       *)
        # Use cross compiler
        CROSS_PREFIX="arm-linux-gnueabihf-"
        ;;
esac

[ ! -d './build' ] && mkdir ./build

# Build HOST side application
${CROSS_PREFIX}gcc ${HOST_FILES} -o ./build/main.elf ${EINCS} ${ELIBS} -le-hal -le-loader -lpthread -ggdb

# Build DEVICE side program
# -T specifies the linker file
# -O0 reduce compilation time, specifies optimization level
OPT=0
e-gcc -T ${ELDF} -O${OPT} ./src/emain.c -o ./build/emain.elf -le-lib
