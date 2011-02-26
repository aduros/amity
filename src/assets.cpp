#include "assets.h"

#include "log.h"

SDL_RWops* loadAsset (const char* assetName)
{
    SDL_RWops* asset = SDL_RWFromFile(assetName, "r");
    if (asset == NULL) {
        LOGW("Error loading asset: %s", SDL_GetError());
    }
    return asset;
}
