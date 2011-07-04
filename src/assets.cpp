//
// Amity - cross-platform game engine
// https://github.com/aduros/amity/blob/master/LICENSE.txt

#include "assets.h"

#include "SDL_rwops.h"

#include "log.h"

#ifdef __ANDROID__
#define STAGING_ROOT "/sdcard/amity-dev/"
#elif __WEBOS__
#define STAGING_ROOT "/tmp/amity-dev/"
#endif

SDL_RWops* loadAsset (const char* assetName)
{
    char sdcard[sizeof(STAGING_ROOT) + strlen(assetName)];
    strcpy(sdcard, STAGING_ROOT);
    strcat(sdcard, assetName);

    SDL_RWops* asset = SDL_RWFromFile(sdcard, "r");
    if (asset == NULL) {
        LOGW("Error loading asset: %s", SDL_GetError());
    }
    return asset;
}

char* readBytesFromAsset (SDL_RWops* asset, size_t* size)
{
    // Seek to the end and get the length
    *size = SDL_RWseek(asset, 0, RW_SEEK_END);

    // Rewind before reading everything into an array
    SDL_RWseek(asset, 0, RW_SEEK_SET);

    char* buffer = new char[*size];
    SDL_RWread(asset, buffer, *size, 1);
    return buffer;
}
