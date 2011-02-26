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

protected:
    Texture () { }

    GLuint _id;
    int _width;
    int _height;
};
