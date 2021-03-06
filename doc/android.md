Requirements
------------

* Android SDK, with the 2.1 (android-7) emulator image installed.
* Android NDK (r5 or higher).
* Mozilla's [Android NDK r4c](http://ftp.mozilla.org/pub/mozilla.org/mobile/source/android-ndk-r4c-0moz3.tar.bz2), required for now to build Spidermonkey.
* autoconf2.13, to build Spidermonkey (Available in an Ubuntu package of the same name).

Building
--------

1.  Download the SDL submodules:

        git submodule init
        git submodule update

2.  Copy external/build.cfg.in to external/build.cfg and edit to point to your SDK and NDKs:

        ANDROID_SDK=/absolute/path/to/android-sdk-linux_86
        ANDROID_NDK=/absolute/path/to/android-ndk-r5b
        MOZILLA_ANDROID_NDK=/absolute/path/to/android-ndk-r4c

    (Note the lack of spaces around the equal signs. This file is actually a Bash script)

3.  Build dependencies:

        cd external
        ./download-deps.sh
        ./build-deps.sh

4.  Build Amity with NDK r5:

        cd android
        android update project -p .    # This only needs to be done once to setup the build
        ndk-build

5.  Deploy:

        ant install
        ./data.sh    # Pushes the test app to the staging area on the device. If you're running on the emulator, make sure you create an SD card with plenty of space.

Then launch the app! You can use 'adb logcat' to show log output of a connected device.
