#include "canvas/CanvasContext.h"

#include "AmityContext.h"

CanvasContext::CanvasContext (AmityContext* amityCtx) : _amityCtx(amityCtx)
{
    CanvasState state;
    state.alpha = 1;
    state.blendMode = BLEND_DEFAULT;

    // FIXME: Detect if this extension is supported.
    // SDL_GL_ExtensionSupported("GL_OES_draw_texture") appears to return false on the N1 even
    // though it works fine.
    state.canDrawTexture = true;

    state.translateX = 0;
    state.translateY = 0;
    state.scaleX = 1;
    state.scaleY = 1;

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
    CanvasState& state = _states.top();
    if (state.canDrawTexture) {
        state.scaleX *= x;
        state.scaleY *= y;
    }
    glScalef(x, y, 0); // TODO: Usually no need to modify GL matrix if using draw_texture
}

void CanvasContext::rotate (float angle)
{
    CanvasState& state = _states.top();
    state.canDrawTexture = false;

    glRotatef(angle, 0, 0, 1);
}

void CanvasContext::translate (float x, float y)
{
    CanvasState& state = _states.top();
    if (state.canDrawTexture) {
        state.translateX += x*state.scaleX;
        state.translateY += y*state.scaleY;
    }
    glTranslatef(x, y, 0); // TODO: Usually no need to modify GL matrix if using draw_texture
}

void CanvasContext::multiplyAlpha (float factor)
{
    _states.top().alpha *= factor;
}

void CanvasContext::setBlendMode (CanvasBlendMode blendMode)
{
}

void CanvasContext::drawImage (const Texture* texture,
    float destX, float destY, float sourceX, float sourceY, float sourceW, float sourceH)
{
    const CanvasState& state = _states.top();

    prepare(texture);

    if (state.canDrawTexture) {
        // Use GL draw_texture
        int windowHeight;
        getSize(NULL, &windowHeight);
        float destW = state.scaleX * sourceW;
        float destH = state.scaleY * sourceH;
        GLfloat cropRect[] = {
            sourceX, sourceY + texture->getHeight(), sourceW, -sourceH
        };
        glTexParameterfv(GL_TEXTURE_2D, GL_TEXTURE_CROP_RECT_OES, cropRect);
        glDrawTexfOES(state.translateX + destX,
            windowHeight - destY - destH - state.translateY,
            0, destW, destH);

    } else {
        // Use GL vertex arrays
        GLfloat verts[] = {
            destX, destY,
            destX + sourceW, destY,
            destX, destY + sourceH,
            destX + sourceW, destY + sourceH,
        };

        float inverseMaxU = texture->getMaxU();
        float inverseMaxV = texture->getMaxV();
        float x1 = sourceX/texture->getWidth() * inverseMaxU;
        float y1 = sourceY/texture->getHeight() * inverseMaxV;
        float x2 = x1 + sourceW/texture->getWidth() * inverseMaxU;
        float y2 = y1 + sourceH/texture->getHeight() * inverseMaxV;
        GLfloat uv[] = {
            x1, y1,
            x2, y1,
            x1, y2,
            x2, y2,
        };
        glVertexPointer(2, GL_FLOAT, 0, verts);
        glTexCoordPointer(2, GL_FLOAT, 0, uv);
        glDrawArrays(GL_TRIANGLE_STRIP, 0, 4);
    }
}

void CanvasContext::drawPattern (
    const Texture* texture, float destX, float destY, float destW, float destH)
{
    prepare(texture);

    float w = texture->getWidth();
    float h = texture->getHeight();

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

    glVertexPointer(2, GL_FLOAT, 0, verts);
    glTexCoordPointer(2, GL_FLOAT, 0, uv);
    glDrawArrays(GL_TRIANGLE_STRIP, 0, 4);
}

void CanvasContext::fillRect (uint32_t color, float x, float y, float width, float height)
{
    const CanvasState& state = _states.top();
    GLfloat verts[] = {
        x, y,
        x + width, y,
        x, y + height,
        x + width, y + height,
    };

    glDisable(GL_TEXTURE_2D);
    glColor4ub(
        color >> 16,
        color >> 8,
        color >> 0,
        0xff * state.alpha);
    glVertexPointer(2, GL_FLOAT, 0, verts);
    glDrawArrays(GL_TRIANGLE_STRIP, 0, 4);
    glEnable(GL_TEXTURE_2D);
}

void CanvasContext::getSize (int* width, int* height) const
{
#if SDL_VERSION_ATLEAST(1,3,0)
    SDL_GetWindowSize(_amityCtx->window, width, height);
#else
    if (width != NULL) {
        *width = _amityCtx->screen->w;
    }
    if (height != NULL) {
        *height = _amityCtx->screen->h;
    }
#endif
}

void CanvasContext::prepare (const Texture* texture)
{
    const CanvasState& state = _states.top();

    glBindTexture(GL_TEXTURE_2D, texture->getId());

    // TODO: Only bother turning on blending for textures with an alpha channel
    glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);
    glTexEnvf(GL_TEXTURE_ENV, GL_TEXTURE_ENV_MODE, GL_MODULATE);
    glColor4f(1, 1, 1, state.alpha);
}
