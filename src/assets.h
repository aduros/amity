//
// Amity - cross-platform game engine
// https://github.com/aduros/amity/blob/master/LICENSE.txt

#pragma once

#include <cstring>

class SDL_RWops;

SDL_RWops* loadAsset (const char* assetName);

char* readBytesFromAsset (SDL_RWops* asset, size_t* size);
