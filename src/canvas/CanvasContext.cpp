#include "canvas/CanvasContext.h"

#include "SDL_opengles.h"

CanvasContext::CanvasContext ()
{
    CanvasState state;
    state.alpha = 1;
    state.blendMode = 0;
    _states.push(state);
}

void CanvasContext::save ()
{
    glPushMatrix();
    _states.push(_states.top());
}

void CanvasContext::restore ()
{
    glPopMatrix();
    _states.pop();
}

void CanvasContext::scale (float x, float y)
{
    glScalef(x, y, 0);
}

void CanvasContext::rotate (float angle)
{
    glRotatef(angle, 0, 0, 1);
}

void CanvasContext::translate (float x, float y)
{
    glTranslatef(x, y, 0);
}

void CanvasContext::setAlpha (float alpha)
{
    _states.top().alpha = alpha;
}

void CanvasContext::setBlendMode (int blendMode)
{
}

void CanvasContext::drawTexture (const Texture* texture, float dx, float dy)
{
    float w = texture->getWidth();
    float h = texture->getHeight();
    const CanvasState& state = _states.top();

    GLfloat verts[] = {
        dx, dy,
        dx + w, dy,
        dx, dy + h,
        dx + w, dy + h,
    };
    GLfloat uv[] = {
        0, 0,
        1, 0,
        0, 1,
        1, 1,
    };

    glEnable(GL_TEXTURE_2D);
    glBindTexture(GL_TEXTURE_2D, texture->getId());
    glVertexPointer(2, GL_FLOAT, 0, verts);
    glEnableClientState(GL_VERTEX_ARRAY);
    glTexCoordPointer(2, GL_FLOAT, 0, uv);
    glEnableClientState(GL_TEXTURE_COORD_ARRAY);

    //glTexEnvf(GL_TEXTURE_ENV, GL_TEXTURE_ENV_MODE, GL_MODULATE);
    //glEnable(GL_BLEND);
    //glBlendFunc(GL_SRC_ALPHA, GL_ONE);
    //glColor4f(1, 1, 1, 0.5);

    glDrawArrays(GL_TRIANGLE_STRIP, 0, 4);

    //glColor4f(1, 1, 1, 1);
    //glDisable(GL_BLEND);
    glDisableClientState(GL_VERTEX_ARRAY);
    glDisableClientState(GL_TEXTURE_COORD_ARRAY);
    glDisable(GL_TEXTURE_2D);
}
