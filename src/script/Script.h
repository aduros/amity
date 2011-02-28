#pragma once

#define XP_UNIX
#include "jsapi.h"
#undef XP_UNIX

class JSContext;
class JSFunction;
class AmityContext;
union SDL_Event;

#define SCRIPT_FUNCTION(fn,jsCtx,argc,vp) JSBool fn (JSContext* jsCtx, uintN argc, jsval* vp)
#define SCRIPT_PROPERTY(fn,jsCtx,obj,id,vp) JSBool fn (JSContext* jsCtx, JSObject* obj, jsid id, jsval* vp)

class Script
{
public:
    Script (AmityContext* amityCtx) : _amityCtx(amityCtx), _jsCtx(0), _onEnterFrame(0) { }
    ~Script();

    int parse (const char* filename, const char* source);

    void onEnterFrame (unsigned int dt);
    void onEvent (const SDL_Event* event);

protected:

    void initAmityClasses ();

    SCRIPT_FUNCTION(canvasSave, jsCtx, argc, vp);
    SCRIPT_FUNCTION(canvasRestore, jsCtx, argc, vp);
    SCRIPT_FUNCTION(canvasRotate, jsCtx, argc, vp);
    SCRIPT_FUNCTION(canvasTranslate, jsCtx, argc, vp);
    SCRIPT_FUNCTION(canvasScale, jsCtx, argc, vp);
    SCRIPT_FUNCTION(canvasDrawTestImage, jsCtx, argc, vp);

    SCRIPT_PROPERTY(setOnEnterFrame, jsCtx, obj, id, vp);
    SCRIPT_PROPERTY(setOnMouseMove, jsCtx, obj, id, vp);
    SCRIPT_PROPERTY(setAlpha, jsCtx, obj, id, vp);

    AmityContext* _amityCtx;
    JSContext* _jsCtx;

    JSFunction* _onEnterFrame;
    JSFunction* _onMouseMove;
};
