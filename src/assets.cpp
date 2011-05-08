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
