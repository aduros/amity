package com.threerings.amity;

import org.libsdl.app.SDLActivity;

public class AmityActivity extends SDLActivity
{
    static {
        // Spidermonkey
        System.loadLibrary("nspr4");
        System.loadLibrary("plc4");
        System.loadLibrary("plds4");
        System.loadLibrary("mozjs");

        // The main attraction
        System.loadLibrary("amity");
    }
}
