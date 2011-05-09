#!/bin/sh

# Builds a copy of Spidermonkey for webOS

. ./build.cfg

if [ ! -n "$PALM_PDK" ]; then
    echo PALM_PDK not set. Did you set up your build.cfg?
    exit
fi

MOZILLA_CENTRAL=$PWD/mozilla-central
BUILD_DIR=$PWD/webos-build

toolchain="$PALM_PDK/arm-gcc"

# These compiler flags rule out Pixi compatibility. Should we care?
cflags="-I$PALM_PDK/include -I$PALM_PDK/include/SDL -mfloat-abi=softfp -fno-short-enums -fno-exceptions -mcpu=cortex-a8 -mfpu=neon -DCROSS_COMPILE=1"
ldflags="-L$PALM_PDK/device/lib -Wl,--allow-shlib-undefined --sysroot=$toolchain/sysroot"

export AS="$toolchain/bin/arm-none-linux-gnueabi-as"
export CC="$toolchain/bin/arm-none-linux-gnueabi-gcc $cflags"
export CXX="$toolchain/bin/arm-none-linux-gnueabi-g++ $cflags"
export CPP="$toolchain/bin/arm-none-linux-gnueabi-cpp"
export LD="$toolchain/bin/arm-none-linux-gnueabi-ld $ldflags"
export AR="$toolchain/bin/arm-none-linux-gnueabi-ar"
export RANLIB="$toolchain/bin/arm-none-linux-gnueabi-ranlib"
export STRIP="$toolchain/bin/arm-none-linux-gnueabi-strip"
export CXXFLAGS=""
export CFLAGS=""
export HOST_CC=`which gcc`
export HOST_CXX=`which g++`
export CROSS_COMPILE=1

cd $MOZILLA_CENTRAL/js/src && autoconf2.13

mkdir -p $BUILD_DIR
cd $BUILD_DIR
mkdir -p nspr
cd nspr
$MOZILLA_CENTRAL/nsprpub/configure \
    --disable-installer \
    --enable-optimize \
    --disable-tests \
    --host=arm-linux
make

# Removed shared libs to force building against static libs
rm dist/lib/*.so

cd $BUILD_DIR
mkdir -p js
cd js
$MOZILLA_CENTRAL/js/src/configure \
    --target=arm-linux \
    --with-nspr-cflags="-I$BUILD_DIR/nspr/dist/include/nspr" \
    --with-nspr-libs="-L$BUILD_DIR/nspr/dist/lib -lnspr4 -lplc4 -lplds4" \
    --enable-threadsafe \
    --with-endian=little \
    --with-arm-kuser
make
