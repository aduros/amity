#pragma once

#include "SDL_opengles.h"

class Texture
{
public:
    static Texture* fromAsset (const char* assetName);
    ~Texture ();

    inline GLuint getId () const {
        return _id;
    }

    inline int getWidth () const {
        return _width;
    }

    inline int getHeight () const {
        return _height;
    }

    inline float getMaxU () const {
        return _maxU;
    }

    inline float getMaxV () const {
        return _maxV;
    }

protected:
    Texture () { }

    GLuint _id;
    int _width;
    int _height;

    // The UV texture coordinates of the bottom right of this texture. May be less than
    // 1 if this texture's dimensions are not powers of two.
    float _maxU;
    float _maxV;
};
