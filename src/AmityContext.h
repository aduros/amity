#pragma once

#include "canvas/CanvasContext.h"
#include "script/Script.h"

class AmityContext
{
public:
    AmityContext () : script(this) {};

    CanvasContext canvas;
    Script script;
protected:
};
