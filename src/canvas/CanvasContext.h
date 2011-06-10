#pragma once

#include <stack>

#define GL_GLEXT_PROTOTYPES
#include "SDL_opengles.h"
#undef GL_GLEXT_PROTOTYPES

#include "canvas/Texture.h"

class AmityContext;

enum CanvasBlendMode {
    BLEND_DEFAULT
};

struct CanvasState
{
    float alpha;
    CanvasBlendMode blendMode;

    // State used for the GL draw_texture extension
    bool canDrawTexture;
    float translateX, translateY;
    float scaleX, scaleY;
};

class CanvasContext
{
public:
    CanvasContext (AmityContext* amityCtx);

    void save ();
    void restore ();

    void scale (float x, float y);
    void rotate (float angle);
    void translate (float x, float y);

    void multiplyAlpha (float factor);
    void setBlendMode (CanvasBlendMode blendMode);

    void drawImage (const Texture* texture, float destX, float destY,
        float sourceX, float sourceY, float sourceW, float sourceH);

    void drawPattern (const Texture* texture, float destX, float destY, float destW, float destH);

    void fillRect (uint32_t color, float x, float y, float width, float height);

    void getSize (int* width, int* height) const;

protected:

    void prepare (const Texture* texture);

    std::stack<CanvasState> _states;
    AmityContext* _amityCtx;
};
