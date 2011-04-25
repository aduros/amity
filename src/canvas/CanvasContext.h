#pragma once

#include <stack>

#include "SDL_opengles.h"

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

    void drawImage (const Texture* texture, float destX, float destY);
    void drawImage (const Texture* texture, float destX, float destY,
        float destW, float destH, float sourceX, float sourceY);

    void drawPattern (const Texture* texture, float destX, float destY, float destW, float destH);

protected:

    void drawQuad (const Texture* texture, GLfloat* verts, GLfloat* uv);
    std::stack<CanvasState> _states;
};
