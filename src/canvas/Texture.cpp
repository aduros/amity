//
// Amity - cross-platform game engine
// https://github.com/aduros/amity/blob/master/LICENSE.txt

#include "canvas/Texture.h"

#include "SDL_video.h"
#include "SDL_opengles.h"
#include "SDL_image.h"

#include "assets.h"
#include "log.h"

static int nextPowerOfTwo (int x)
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

    int widthPow2 = nextPowerOfTwo(surface->w);
    int heightPow2 = nextPowerOfTwo(surface->h);

    Texture* texture = new Texture();
    texture->_width = surface->w;
    texture->_height = surface->h;
    texture->_maxU = (float)surface->w / widthPow2;
    texture->_maxV = (float)surface->h / heightPow2;

    glGenTextures(1, &texture->_id);
    glBindTexture(GL_TEXTURE_2D, texture->_id);

    // Create the upscaled texture in OpenGL
    glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA, widthPow2, heightPow2, 0,
        GL_RGBA, GL_UNSIGNED_BYTE, NULL);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);

    // Convert the loaded image into a standard pixel format for OpenGL
    SDL_PixelFormat format;
    SDL_memset(&format, 0, sizeof(SDL_PixelFormat));
    format.BytesPerPixel = 4;
    format.BitsPerPixel = 8*format.BytesPerPixel;
    format.Amask = 0xff000000;
    format.Bmask = 0x00ff0000;
    format.Gmask = 0x0000ff00;
    format.Rmask = 0x000000ff;
    SDL_Surface* surfaceFormatted = SDL_ConvertSurface(surface, &format, 0);

    // Upload the pixels, which may only be a subregion for NPO2 textures
    glTexSubImage2D(GL_TEXTURE_2D, 0, 0, 0, surfaceFormatted->w, surfaceFormatted->h,
       GL_RGBA, GL_UNSIGNED_BYTE, surfaceFormatted->pixels);

    SDL_FreeSurface(surface);
    SDL_FreeSurface(surfaceFormatted);

    return texture;
}

Texture::~Texture ()
{
    glDeleteTextures(1, &_id);
}
