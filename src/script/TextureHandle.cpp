#include "script/TextureHandle.h"

#include "canvas/Texture.h"
#include "script/util.h"

JSPropertySpec TextureHandle::jsProperties[] = {
    { "width", 0, JSPROP_ENUMERATE | JSPROP_READONLY,
        TextureHandle::bindProperty<&TextureHandle::getWidth>, NULL },
    { "height", 0, JSPROP_ENUMERATE | JSPROP_READONLY,
        TextureHandle::bindProperty<&TextureHandle::getHeight>, NULL },
    { NULL }
};
DEFINE_HANDLE_JSCLASS(TextureHandle, "Texture");

TextureHandle::~TextureHandle ()
{
    delete _texture;
}

SCRIPT_PROPERTY (TextureHandle::getWidth, jsCtx, obj, id, vp)
{
    JS_SET_RVAL(jsCtx, vp, INT_TO_JSVAL(_texture->getWidth()));
    return JS_TRUE;
}

SCRIPT_PROPERTY (TextureHandle::getHeight, jsCtx, obj, id, vp)
{
    JS_SET_RVAL(jsCtx, vp, INT_TO_JSVAL(_texture->getHeight()));
    return JS_TRUE;
}

SCRIPT_FUNCTION (amity_createTexture, jsCtx, argc, vp)
{
    const char* assetName;
    if (!JS_ConvertArguments(jsCtx, 1, JS_ARGV(jsCtx, vp), "s", &assetName)) {
        return JS_FALSE;
    }

    Texture* texture = Texture::fromAsset(assetName);
    if (texture == NULL) {
        return JS_FALSE;
    }

    TextureHandle* textureHandle = TextureHandle::create(jsCtx);
    textureHandle->setTexture(texture);
    JS_SET_RVAL(jsCtx, vp, OBJECT_TO_JSVAL(textureHandle->getJSObject()));

    return JS_TRUE;
}
