//
// Amity - cross-platform game engine
// https://github.com/aduros/amity/blob/master/LICENSE.txt

#include "script/util.h"

#include "SDL.h"

jsid getInternedId (JSContext* jsCtx, const char* name)
{
    JSString* str = JS_InternString(jsCtx, name);
    return INTERNED_STRING_TO_JSID(str);
}

void postEvent (JSContext* jsCtx, AmityEvent* event)
{
    JS_AddObjectRoot(jsCtx, &event->obj);
    JS_AddValueRoot(jsCtx, &event->param);

    SDL_Event sdlEvent;
    sdlEvent.type = SDL_USEREVENT;
    sdlEvent.user.code = AMITY_EVENT_SDL_CODE;
    sdlEvent.user.data1 = event;
    sdlEvent.user.data2 = NULL;
    SDL_PushEvent(&sdlEvent);
}

void consumeEvent (JSContext* jsCtx, AmityEvent* event)
{
    jsval fval;
    if (!JS_GetPropertyById(jsCtx, event->obj, event->handler, &fval)) {
        return; // Missing handler
    }

    jsval rval;
    JS_CallFunctionValue(jsCtx, event->obj, fval, 1, &event->param, &rval);
}
