#include "script/TextureObject.h"

#include "canvas/Texture.h"
#include "script/util.h"

static JSClass classTexture = {
    "Texture", JSCLASS_HAS_PRIVATE | JSCLASS_CONSTRUCT_PROTOTYPE,
    JS_PropertyStub, JS_PropertyStub, JS_PropertyStub, JS_PropertyStub,
    JS_EnumerateStub, JS_ResolveStub, JS_ConvertStub, finalize<Texture>,
    JSCLASS_NO_OPTIONAL_MEMBERS
};

SCRIPT_FUNCTION (amity_createTexture, jsCtx, argc, vp)
{
    const char* assetName;
    if (!JS_ConvertArguments(jsCtx, 1, JS_ARGV(jsCtx, vp), "s", &assetName)) {
        return JS_FALSE;
    }

    JSObject* textureObj = JS_NewObject(jsCtx, &classTexture, NULL, NULL);
    Texture* texture = Texture::fromAsset(assetName);
    JS_SetPrivate(jsCtx, textureObj, texture);

    JS_SET_RVAL(jsCtx, vp, OBJECT_TO_JSVAL(textureObj));
    return JS_TRUE;
}

Texture* getTextureFromObject (JSContext* jsCtx, JSObject* obj)
{
    return static_cast<Texture*>(JS_GetInstancePrivate(jsCtx, obj, &classTexture, NULL));
}
