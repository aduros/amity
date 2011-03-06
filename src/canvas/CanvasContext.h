#pragma once

#include <stack>

#include "canvas/Texture.h"

struct CanvasState
{
    float alpha;
    int blendMode;
};

class CanvasContext
{
public:
    CanvasContext ();

    void save ();
    void restore ();

    void scale (float x, float y);
    void rotate (float angle);
    void translate (float x, float y);

    void setAlpha (float alpha);
    void setBlendMode (int blendMode);

    void drawTexture (const Texture* image, float dx, float dy);

protected:
    std::stack<CanvasState> _states;
};
