#pragma once

#include <stack>

#include "SDL_opengles.h"

#include "canvas/Texture.h"

struct CanvasState
{
    float alpha;
    int blendMode;

    // State used for the GL draw_texture extension
    bool canDrawTexture;
    float translateX, translateY;
    float scaleX, scaleY;
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

    void multiplyAlpha (float factor);
    void setBlendMode (int blendMode);

    void drawImage (const Texture* texture, float destX, float destY,
        float sourceX, float sourceY, float sourceW, float sourceH);

    void drawPattern (const Texture* texture, float destX, float destY, float destW, float destH);

protected:

    void prepare (const Texture* texture);

    std::stack<CanvasState> _states;
};
