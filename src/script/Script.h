//
// Amity - cross-platform game engine
// https://github.com/aduros/amity/blob/master/LICENSE.txt

#pragma once

#include "script/util.h"

class AmityContext;
union SDL_Event;

class Script
{
public:
    Script (AmityContext* amityCtx) : _amityCtx(amityCtx), _jsCtx(0), _eventObj(0) { }
    ~Script();

    int parse (const char* filename, const char* source);

    void onEnterFrame (unsigned int dt);
    void onEvent (const SDL_Event* event);

protected:

    void initAmityClasses ();

    SCRIPT_FUNCTION(amity_canvas_drawImage, jsCtx, argc, vp);
    SCRIPT_FUNCTION(amity_canvas_drawPattern, jsCtx, argc, vp);
    SCRIPT_FUNCTION(amity_canvas_fillRect, jsCtx, argc, vp);
    SCRIPT_FUNCTION(amity_canvas_multiplyAlpha, jsCtx, argc, vp);
    SCRIPT_FUNCTION(amity_canvas_restore, jsCtx, argc, vp);
    SCRIPT_FUNCTION(amity_canvas_rotate, jsCtx, argc, vp);
    SCRIPT_FUNCTION(amity_canvas_save, jsCtx, argc, vp);
    SCRIPT_FUNCTION(amity_canvas_scale, jsCtx, argc, vp);
    SCRIPT_FUNCTION(amity_canvas_translate, jsCtx, argc, vp);

    // Interned strings
    jsid _onEnterFrame;
    jsid _onMouseDown;
    jsid _onMouseMove;
    jsid _onMouseUp;

    AmityContext* _amityCtx;
    JSContext* _jsCtx;
    JSObject* _eventObj;
};
