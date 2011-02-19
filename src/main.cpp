#include <cstdlib>
#include "SDL.h"
#include "SDL_image.h"
#include "SDL_mixer.h"
#include "SDL_opengles.h"

#include "canvas/CanvasContext.h"
#include "log.h"

#define DATAFILE(filename) ("/sdcard/amity/" filename)

static CanvasContext canvas;

SDL_Surface* loadImage (const char* path)
{
    SDL_Surface* image = IMG_Load(path);

    if (image == NULL) {
        LOGE("Couldn't load image %s: %s", path, IMG_GetError());
        return NULL;
    }

    SDL_Surface* surface = SDL_DisplayFormat(image);
    SDL_FreeSurface(image);
    return surface;
}

Uint32 delayFrame ()
{
    static Uint32 nextTick = 0;
    Uint32 currentTick = SDL_GetTicks();

    Uint32 delay = 0;
    if (currentTick < nextTick) {
        delay = nextTick - currentTick;
        SDL_Delay(delay);
    }

    nextTick = currentTick + (1000/60);
    return delay;
}

void renderTest (int x, int y)
{
    Texture fakeTexture = { 0, 60, 60 };

    glClearColor(0.1, 0.1, 0.1, 1.0);
    glClear(GL_COLOR_BUFFER_BIT);

    glColor4f(0.0, 1.0, 0.0, 1.0);

    canvas.save();
    canvas.translate(x, y);
    canvas.drawImage(&fakeTexture, 0, 0);
    canvas.restore();

    SDL_RenderPresent();
}

void mainLoop ()
{
    glEnableClientState(GL_VERTEX_ARRAY);

    SDL_Event event;
    int x = 0, y = 0;
    Uint32 elapsed = 0;

    for (;;) {
        while (SDL_PollEvent(&event)) {
            switch (event.type) {
                case SDL_MOUSEMOTION:
                    x = event.motion.x;
                    y = event.motion.y;
                    //LOGI("Moved to %i,%i", x, y);
                    break;
                case SDL_QUIT:
                    return;
                default:
                    LOGI("Unhandled event %i", event.type);
            }
        }

        renderTest(x, y);

        elapsed = delayFrame();
    }
}

int main (int argc, char* argv[])
{
    LOGI("Main screen turn on, compiled " __DATE__ " at " __TIME__);

    if (SDL_Init(SDL_INIT_VIDEO | SDL_INIT_AUDIO) < 0) {
        LOGE("Unable to init SDL: %s", SDL_GetError());
        return 1;
    }
    atexit(SDL_Quit);

    SDL_GL_SetAttribute(SDL_GL_DEPTH_SIZE, 0);
    SDL_GL_SetAttribute(SDL_GL_RETAINED_BACKING, 0);
    SDL_GL_SetAttribute(SDL_GL_ACCELERATED_VISUAL, 1);

    SDL_Window* window = SDL_CreateWindow(NULL, 0, 0, 0, 0,
        SDL_WINDOW_OPENGL | SDL_WINDOW_SHOWN | SDL_WINDOW_BORDERLESS);
    if (window == NULL) {
        LOGE("Unable to create window: %s", SDL_GetError());
        return 1;
    }

    int w, h;
    SDL_GetWindowSize(window, &w, &h);
    LOGI("Created window [width=%i, height=%i]", w, h);

    if (SDL_CreateRenderer(window, 0, 0) < 0) {
        LOGE("Unable to create renderer: %s", SDL_GetError());
        return 1;
    }

    if (Mix_OpenAudio(11025, AUDIO_U8, 1, 512) < 0) {
        LOGW("Unable to set up audio: %s", SDL_GetError());
        // Just a warning, try to press on
    }

    mainLoop();

    LOGI("Quitting normally");
    Mix_CloseAudio();
    return 0;
}
