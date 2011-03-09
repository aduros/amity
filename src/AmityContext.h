#pragma once

#include "canvas/CanvasContext.h"
#include "script/Script.h"

class SDL_Window;

class AmityContext
{
public:
    AmityContext () : script(this) {};

    SDL_Window* window;

    CanvasContext canvas;
    Script script;
protected:
};
