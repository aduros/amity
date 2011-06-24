//
// Amity - cross-platform game engine
// https://github.com/aduros/amity/blob/master/LICENSE.txt

#pragma once

#define XP_UNIX
#include "jsapi.h"
#undef XP_UNIX

#define SCRIPT_FUNCTION(fn,jsCtx,argc,vp) JSBool fn (JSContext* jsCtx, uintN argc, jsval* vp)
#define SCRIPT_PROPERTY(fn,jsCtx,obj,id,vp) JSBool fn (JSContext* jsCtx, JSObject* obj, jsid id, jsval* vp)

#define AMITY_EVENT_SDL_CODE 0

struct AmityEvent {
    JSObject* obj;
    jsid handler;
    jsval param;
};

jsid getInternedId (JSContext* jsCtx, const char* name);

void postEvent (JSContext* jsCtx, AmityEvent* event);
void consumeEvent (JSContext* jsCtx, AmityEvent* event);
