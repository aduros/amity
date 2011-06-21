//
// Amity - cross-platform game engine
// https://github.com/aduros/amity/blob/master/LICENSE.txt

#pragma once

#include "canvas/Texture.h"
#include "script/util.h"
#include "script/Handle.h"
#include "log.h"

class TextureHandle : public Handle<TextureHandle>
{
public:
    static JSPropertySpec jsProperties[];
    static JSClass jsClass;

    ~TextureHandle ();

    inline Texture* getTexture () const {
        return _texture;
    }

    inline void setTexture (Texture* texture) {
        _texture = texture;
    }

protected:
    SCRIPT_PROPERTY(getWidth, jsCtx, obj, id, vp);
    SCRIPT_PROPERTY(getHeight, jsCtx, obj, id, vp);

    Texture* _texture;
};

SCRIPT_FUNCTION (amity_createTexture, jsCtx, argc, vp);
