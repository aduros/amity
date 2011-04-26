#include "canvas/CanvasContext.h"

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

void CanvasContext::drawImage (const Texture* texture, float destX, float destY)
{
    float w = texture->getWidth();
    float h = texture->getHeight();
    drawImage(texture, destX, destY, 0, 0, w, h);
}

void CanvasContext::drawImage (const Texture* texture,
    float destX, float destY, float sourceX, float sourceY, float sourceW, float sourceH)
{
    float maxU = texture->getMaxU();
    float maxV = texture->getMaxV();

    GLfloat verts[] = {
        destX, destY,
        destX + sourceW, destY,
        destX, destY + sourceH,
        destX + sourceW, destY + sourceH,
    };
    // TODO: Use sourceX/Y to get the right rectangle
    GLfloat uv[] = {
        0, 0,
        maxU, 0,
        0, maxV,
        maxU, maxV,
    };
    drawQuad(texture, verts, uv);
}

void CanvasContext::drawPattern (
    const Texture* texture, float destX, float destY, float destW, float destH)
{
    float w = texture->getWidth();
    float h = texture->getHeight();
    float maxU = texture->getMaxU();
    float maxV = texture->getMaxV();

    GLfloat verts[] = {
        destX, destY,
        destX + destW, destY,
        destX, destY + destH,
        destX + destW, destY + destH,
    };
    GLfloat uv[] = {
        0, 0,
        destW/w, 0,
        0, destH/h,
        destW/w, destH/h,
    };
    drawQuad(texture, verts, uv);
}

void CanvasContext::drawQuad (const Texture* texture, GLfloat* verts, GLfloat* uv)
{
    glEnable(GL_TEXTURE_2D);
    glBindTexture(GL_TEXTURE_2D, texture->getId());
    glVertexPointer(2, GL_FLOAT, 0, verts);
    glEnableClientState(GL_VERTEX_ARRAY);
    glTexCoordPointer(2, GL_FLOAT, 0, uv);
    glEnableClientState(GL_TEXTURE_COORD_ARRAY);

    // TODO: Only bother turning on blending for textures with an alpha channel
    glTexEnvf(GL_TEXTURE_ENV, GL_TEXTURE_ENV_MODE, GL_MODULATE);
    glEnable(GL_BLEND);
    glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);

    glDrawArrays(GL_TRIANGLE_STRIP, 0, 4);

    glDisable(GL_BLEND);
    glDisableClientState(GL_VERTEX_ARRAY);
    glDisableClientState(GL_TEXTURE_COORD_ARRAY);
    glDisable(GL_TEXTURE_2D);
}
