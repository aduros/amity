#include "script/TextureObject.h"

#include "canvas/Texture.h"
#include "script/util.h"

SCRIPT_PROPERTY (getWidth, jsCtx, obj, id, vp)
{
    Texture* texture = static_cast<Texture*>(JS_GetPrivate(jsCtx, obj));
    JS_SET_RVAL(jsCtx, vp, INT_TO_JSVAL(texture->getWidth()));
    return JS_TRUE;
}

SCRIPT_PROPERTY (getHeight, jsCtx, obj, id, vp)
{
    Texture* texture = static_cast<Texture*>(JS_GetPrivate(jsCtx, obj));
    JS_SET_RVAL(jsCtx, vp, INT_TO_JSVAL(texture->getHeight()));
    return JS_TRUE;
}

static JSPropertySpec textureProperties[] = {
    { "width", 0, JSPROP_ENUMERATE | JSPROP_READONLY, getWidth, NULL },
    { "height", 0, JSPROP_ENUMERATE | JSPROP_READONLY, getHeight, NULL },
    { NULL }
};

static JSClass textureClass = {
    "Texture", JSCLASS_HAS_PRIVATE,
    JS_PropertyStub, JS_PropertyStub, JS_PropertyStub, JS_PropertyStub,
    JS_EnumerateStub, JS_ResolveStub, JS_ConvertStub, finalize<Texture>,
};

SCRIPT_FUNCTION (amity_createTexture, jsCtx, argc, vp)
{
    const char* assetName;
    if (!JS_ConvertArguments(jsCtx, 1, JS_ARGV(jsCtx, vp), "s", &assetName)) {
        return JS_FALSE;
    }

    JSObject* textureObj = JS_NewObject(jsCtx, &textureClass, NULL, NULL);
    Texture* texture = Texture::fromAsset(assetName);
    JS_SetPrivate(jsCtx, textureObj, texture);

    JS_SET_RVAL(jsCtx, vp, OBJECT_TO_JSVAL(textureObj));
    return JS_TRUE;
}

void initTextureObject (JSContext* jsCtx)
{
    JS_InitClass(jsCtx, JS_GetGlobalObject(jsCtx), NULL, &textureClass,
        NULL, 0, textureProperties, NULL, NULL, NULL);
}

Texture* getTextureFromObject (JSContext* jsCtx, JSObject* obj)
{
    return static_cast<Texture*>(JS_GetInstancePrivate(jsCtx, obj, &textureClass, NULL));
}
