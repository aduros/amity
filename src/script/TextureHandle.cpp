//
// Amity - cross-platform game engine
// https://github.com/aduros/amity/blob/master/LICENSE.txt

#include "script/TextureHandle.h"

#include "canvas/Texture.h"
#include "script/util.h"

JSPropertySpec TextureHandle::jsProperties[] = {
    { "width", 0, JSPROP_ENUMERATE | JSPROP_READONLY | JSPROP_PERMANENT | JSPROP_SHARED,
        TextureHandle::bindProperty<&TextureHandle::getWidth>, NULL },
    { "height", 0, JSPROP_ENUMERATE | JSPROP_READONLY | JSPROP_PERMANENT | JSPROP_SHARED,
        TextureHandle::bindProperty<&TextureHandle::getHeight>, NULL },
    { NULL }
};
JSFunctionSpec TextureHandle::jsFunctions[] = { NULL };
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
    JSString* assetName;
    if (!JS_ConvertArguments(jsCtx, 1, JS_ARGV(jsCtx, vp), "S", &assetName)) {
        return JS_FALSE;
    }

    char* str = JS_EncodeString(jsCtx, assetName);
    Texture* texture = Texture::fromAsset(str);
    JS_free(jsCtx, str);
    if (texture == NULL) {
        return JS_FALSE;
    }

    TextureHandle* textureHandle = TextureHandle::create(jsCtx);
    textureHandle->setTexture(texture);
    JS_SET_RVAL(jsCtx, vp, OBJECT_TO_JSVAL(textureHandle->getJSObject()));

    return JS_TRUE;
}
