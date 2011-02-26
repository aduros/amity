#include "canvas/CanvasContext.h"

#include "SDL_opengles.h"

void CanvasContext::save ()
{
    glPushMatrix();
}

void CanvasContext::restore ()
{
    glPopMatrix();
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
}

void CanvasContext::setBlendMode (int blendMode)
{
}

void CanvasContext::drawImage (const Texture* texture, float dx, float dy)
{
    float w = texture->getWidth();
    float h = texture->getHeight();

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

    glDrawArrays(GL_TRIANGLE_STRIP, 0, 4);

    glDisableClientState(GL_VERTEX_ARRAY);
    glDisableClientState(GL_TEXTURE_COORD_ARRAY);
    glDisable(GL_TEXTURE_2D);
}
