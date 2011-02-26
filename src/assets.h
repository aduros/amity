#pragma once

#include "SDL_rwops.h"

// TODO: Test assets are looked up on the sdcard for now, eventually it should read from the APK
#define SDCARD(filename) ("/sdcard/data/" filename)

SDL_RWops* loadAsset (const char* assetName);
