#include "asset/AssetPack.h"

#include "canvas/Texture.h"

Texture* AssetPack::createTexture (const char* name)
{
    // TODO: Rename asset to resource?
    return Texture::fromAsset(name);
}
