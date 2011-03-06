#pragma once

#include "canvas/Texture.h"
#include "script/util.h"

SCRIPT_FUNCTION (amity_createTexture, jsCtx, argc, vp);

// Returns the Texture associated with an object, or NULL if not available
Texture* getTextureFromObject (JSContext* jsCtx, JSObject* obj);
