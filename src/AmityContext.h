#pragma once

#include "canvas/CanvasContext.h"
#include "script/Script.h"

#include "SDL.h"

class AmityContext
{
public:
    AmityContext () : canvas(this), script(this) {};

#if SDL_VERSION_ATLEAST(1,3,0)
    SDL_Window* window;
#else
    SDL_Surface* screen;
#endif

    CanvasContext canvas;
    Script script;
protected:
};
