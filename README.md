Amity
=====

Amity is a high-level interface between Javascript and mobile phones for games.

It will be possible to use haXe to write games that run on Amity (Android, iPhone) and Flash (Desktop).

Requirements
------------

* Android SDK
* Android NDK (r5 or higher)
* Mozilla's [Android NDK r4c](http://ftp.mozilla.org/pub/mozilla.org/mobile/source/android-ndk-r4c-0moz3.tar.bz2), required for now to build Spidermonkey

Building
--------

1.  Download the SDL and Mozilla submodules:

        git submodule init
        git submodule update

2.  If you intend to use the Android emulator, edit externals/SDL/include/SDL_config_android.h and comment out the following:

        // #define SDL_VIDEO_RENDER_OGL_ES2 1

    The Android emulator doesn't support OpenGLES v2. SDL will soon add a proper fallback to v1, so this is temporary.

3.  Patch externals/mozjs/android-build.sh to point to your directory:
        
        ANDROID_NDK=/absolute/path/to/android-ndk-r4c
        ANDROID_SDK=/absolute/path/to/android-sdk-linux_86

    Yeah, this is an artifact of this being a solo project until now.

4.  Build Spidermonkey:

        cd externals/mozjs
        ./android-build.sh

5.  Build Amity with NDK r5:

        cd android
        android update project -p .    # This only needs to be done once to setup the build
        ndk-build

6.  Deploy:

        ant install
        ./data.sh    # Pushes the assets directory onto the SDCard until reading from the APK is supported. If you're running on the emulator, make sure you give it plenty of SD card space.

Then launch the app! You can use 'adb logcat' to show log output of a connected device.

Did something go wrong? Let me know :)
