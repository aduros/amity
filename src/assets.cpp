#include "assets.h"

#include "SDL_rwops.h"

#include "log.h"

#define SDCARD_ROOT "/sdcard/amity-dev/"

SDL_RWops* loadAsset (const char* assetName)
{
    char sdcard[sizeof(SDCARD_ROOT) + strlen(assetName)];
    strcpy(sdcard, SDCARD_ROOT);
    strcat(sdcard, assetName);

    SDL_RWops* asset = SDL_RWFromFile(sdcard, "r");
    if (asset == NULL) {
        LOGW("Error loading asset: %s", SDL_GetError());
    }
    return asset;
}
