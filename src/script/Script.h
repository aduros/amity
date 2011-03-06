#pragma once

#include "script/util.h"

class AmityContext;
union SDL_Event;

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

    SCRIPT_FUNCTION(amity_canvas_drawTexture, jsCtx, argc, vp);
    SCRIPT_FUNCTION(amity_canvas_restore, jsCtx, argc, vp);
    SCRIPT_FUNCTION(amity_canvas_rotate, jsCtx, argc, vp);
    SCRIPT_FUNCTION(amity_canvas_save, jsCtx, argc, vp);
    SCRIPT_FUNCTION(amity_canvas_scale, jsCtx, argc, vp);
    SCRIPT_FUNCTION(amity_canvas_translate, jsCtx, argc, vp);

    SCRIPT_PROPERTY(amity_canvas_setAlpha, jsCtx, obj, id, vp);
    SCRIPT_PROPERTY(amity_setOnEnterFrame, jsCtx, obj, id, vp);
    SCRIPT_PROPERTY(amity_setOnMouseMove, jsCtx, obj, id, vp);

    AmityContext* _amityCtx;
    JSContext* _jsCtx;

    JSFunction* _onEnterFrame;
    JSFunction* _onMouseMove;
};
