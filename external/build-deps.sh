#!/bin/bash
#
# Builds all of Amity's dependencies for each available platform

EXTERNAL_DIR="`dirname $(readlink -f $0)`"
INSTALL_DIR="$EXTERNAL_DIR/build"

. $EXTERNAL_DIR/build.cfg

function build {
    TMP_DIR="/tmp/amity_build"
    PLATFORM="$1"
    PROJECT_DIR="$2"

    echo "Building $PROJECT_DIR for $PLATFORM ..."

    mkdir -p $TMP_DIR
    cd $TMP_DIR
    $PROJECT_DIR/configure \
        "--prefix=$INSTALL_DIR/$PLATFORM" \
        "${@:3}"
    make
    make install
    rm -rf $TMP_DIR
}

mkdir -p $INSTALL_DIR

MOZILLA_REPO=$EXTERNAL_DIR/mozilla-release
cd $MOZILLA_REPO/js/src && (autoconf2.13 || autoconf213)

CURL_FLAGS="
    --disable-ares
    --disable-ftp
    --disable-imap
    --disable-ipv6
    --disable-ldap
    --disable-ldaps
    --disable-pop3
    --disable-rtsp
    --disable-smtp
    --disable-sspi
    --disable-dict
    --disable-file
    --disable-gopher
    --disable-telnet
    --disable-tftp
    --without-ca-bundle
    --without-random
    --without-ssl
    --without-zlib"

#
# Android dependencies
#
if [ ! -n "$ANDROID_SDK" -o ! -n "$ANDROID_NDK" ]; then
    echo "ANDROID_SDK or ANDROID_NDK not set. Skipping Android."
else
    build android $MOZILLA_REPO/nsprpub \
        --target=arm-android-eabi \
        --with-android-ndk=$MOZILLA_ANDROID_NDK \
        --disable-thumb2

    build android $MOZILLA_REPO/js/src \
        --target=arm-android-eabi \
        --with-android-ndk=$MOZILLA_ANDROID_NDK \
        --with-android-sdk=$ANDROID_SDK \
        --with-nspr-cflags="-I$INSTALL_DIR/android/include/nspr" \
        --with-nspr-libs="-L$INSTALL_DIR/android/lib -lnspr4 -lplc4 -lplds4" \
        --enable-threadsafe \
        --with-endian=little \
        --with-arm-kuser \
        --disable-thumb2

    ANDROID_TOOLCHAIN=$INSTALL_DIR/android-toolchain
    $ANDROID_NDK/build/tools/make-standalone-toolchain.sh \
        --platform=android-8 --install-dir=$ANDROID_TOOLCHAIN
    export PATH=$ANDROID_TOOLCHAIN/bin:$PATH
    export CC="arm-linux-androideabi-gcc -DANDROID"

    build android $EXTERNAL_DIR/curl \
        $CURL_FLAGS \
        --host=arm-linux-eabi
fi

#
# webOS dependencies
#
if [ ! -n "$PALM_PDK" ]; then
    echo "PALM_PDK not set. Skipping webOS."
else
    PALM_TOOLCHAIN="$PALM_PDK/arm-gcc"

    # These compiler flags rule out Pixi compatibility. Should we care?
    PALM_CFLAGS="-I$PALM_PDK/include -I$PALM_PDK/include/SDL -mfloat-abi=softfp -fno-short-enums -fno-exceptions -mcpu=cortex-a8 -mfpu=neon -DCROSS_COMPILE=1"
    PALM_LDFLAGS="-L$PALM_PDK/device/lib -Wl,--allow-shlib-undefined --sysroot=$PALM_TOOLCHAIN/sysroot"

    export AS="$PALM_TOOLCHAIN/bin/arm-none-linux-gnueabi-as"
    export CC="$PALM_TOOLCHAIN/bin/arm-none-linux-gnueabi-gcc $PALM_CFLAGS"
    export CXX="$PALM_TOOLCHAIN/bin/arm-none-linux-gnueabi-g++ $PALM_CFLAGS"
    export CPP="$PALM_TOOLCHAIN/bin/arm-none-linux-gnueabi-cpp"
    export LD="$PALM_TOOLCHAIN/bin/arm-none-linux-gnueabi-ld $PALM_LDFLAGS"
    export AR="$PALM_TOOLCHAIN/bin/arm-none-linux-gnueabi-ar"
    export RANLIB="$PALM_TOOLCHAIN/bin/arm-none-linux-gnueabi-ranlib"
    export STRIP="$PALM_TOOLCHAIN/bin/arm-none-linux-gnueabi-strip"
    export CXXFLAGS=""
    export PALM_CFLAGS=""
    export HOST_CC=`which gcc`
    export HOST_CXX=`which g++`
    export CROSS_COMPILE=1

    build webos $MOZILLA_REPO/nsprpub \
        --disable-installer \
        --enable-optimize \
        --disable-tests \
        --host=arm-linux
    # Force mozjs to static link NSPR. --disable-shared does not seem to work.
    rm -f $INSTALL_DIR/webos/lib/*.so

    build webos $MOZILLA_REPO/js/src \
        --target=arm-linux \
        --with-nspr-cflags="-I$INSTALL_DIR/webos/include/nspr" \
        --with-nspr-libs="-L$INSTALL_DIR/webos/lib -lnspr4 -lplc4 -lplds4" \
        --enable-threadsafe \
        --with-endian=little \
        --with-arm-kuser

    build webos $EXTERNAL_DIR/curl \
        $CURL_FLAGS \
        --host=arm-linux
fi
