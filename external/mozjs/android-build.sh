#!/bin/sh

# Creates a prebuilt libmozjs.so for the NDK build.
# This is a work-around until Spidermonkey builds against NDK r5:
#   https://bugzilla.mozilla.org/show_bug.cgi?id=617115

# Change these:
ANDROID_NDK=/home/bruno/work/android-ndk-r4c
ANDROID_SDK=/home/bruno/work/android-sdk-linux_86

MOZILLA_CENTRAL=$PWD/mozilla-central
BUILD_DIR=$PWD/android-build

cd $MOZILLA_CENTRAL/js/src && autoconf2.13

mkdir -p $BUILD_DIR
cd $BUILD_DIR
mkdir -p nspr
cd nspr
$MOZILLA_CENTRAL/nsprpub/configure \
    --target=arm-android-eabi \
    --disable-thumb2 \
    --with-android-ndk=$ANDROID_NDK
make

cd $BUILD_DIR
mkdir -p js
cd js
$MOZILLA_CENTRAL/js/src/configure \
    --target=arm-android-eabi \
    --with-android-ndk=$ANDROID_NDK \
    --with-android-sdk=$ANDROID_SDK \
    --with-nspr-cflags="-I$BUILD_DIR/nspr/dist/include/nspr" \
    --with-nspr-libs="-L$BUILD_DIR/nspr/dist/lib -lnspr4 -lplc4 -lplds4" \
    --enable-threadsafe \
    --with-endian=little \
    --with-arm-kuser \
    --disable-thumb2
make BUILD_OPT=1
