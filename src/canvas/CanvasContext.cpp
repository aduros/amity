#include "canvas/CanvasContext.h"

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

void CanvasContext::drawImage (const Texture* image, float dx, float dy)
{
    float w = image->width;
    float h = image->height;

    GLfloat verts[] = {
        dx, dy,
        dx + w, dy,
        dx, dy + h,
        dx + w, dy + h,
    };

    //glBindTexture(GL_TEXTURE_2D, image->textureId);

    glVertexPointer(2, GL_FLOAT, 0, verts);
    glDrawArrays(GL_TRIANGLE_STRIP, 0, 4);

    //glBindTexture(GL_TEXTURE_2D, 0);
}
