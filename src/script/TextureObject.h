#pragma once

#include "canvas/Texture.h"
#include "script/util.h"

SCRIPT_FUNCTION (amity_createTexture, jsCtx, argc, vp);

void initTextureObject (JSContext* jsCtx);

// Returns the Texture associated with an object, or NULL if not available
Texture* getTextureFromObject (JSContext* jsCtx, JSObject* obj);
