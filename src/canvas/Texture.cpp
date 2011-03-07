#include "canvas/Texture.h"

#include "SDL_video.h"
#include "SDL_opengles.h"
#include "SDL_image.h"

#include "assets.h"
#include "log.h"

int nextPowerOfTwo (int x)
{
    int n = 1;
    while (n < x) {
        n <<= 1;
    }
    return n;
}

Texture* Texture::fromAsset (const char* assetName)
{
    int bpp;
    Uint32 maskR, maskG, maskB, maskA;
    SDL_Surface* surface = IMG_Load_RW(loadAsset(assetName), 1);

    if (surface == NULL) {
        LOGW("Couldn't load texture: %s", SDL_GetError());
        return NULL;
    }

    // Convert the loaded image into a standard pixel format for OpenGL
    SDL_PixelFormatEnumToMasks(SDL_PIXELFORMAT_ABGR8888,
        &bpp, &maskR, &maskG, &maskB, &maskA);
    SDL_Surface* surface8888 = SDL_CreateRGBSurface(
        0, surface->w, surface->h, bpp, maskR, maskG, maskB, maskA);
    SDL_BlitSurface(surface, NULL, surface8888, NULL);

    Texture* texture = new Texture();
    texture->_width = surface->w;
    texture->_height = surface->h;

    glEnable(GL_TEXTURE_2D);
    glGenTextures(1, &texture->_id);
    glBindTexture(GL_TEXTURE_2D, texture->_id);

    // Create the texture in OpenGL
    glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA,
        nextPowerOfTwo(texture->_width), nextPowerOfTwo(texture->_height),
        0, GL_RGBA, GL_UNSIGNED_BYTE, NULL);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);

    // Upload the asset
    glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA, surface->w, surface->h, 0,
        GL_RGBA, GL_UNSIGNED_BYTE, surface8888->pixels);

    glDisable(GL_TEXTURE_2D);
    SDL_FreeSurface(surface);
    SDL_FreeSurface(surface8888);

    return texture;
}

Texture::~Texture ()
{
    glDeleteTextures(1, &_id);
}
