#include <cstdlib>
#include <cstdio>

#include "SDL.h"
#include "SDL_opengles.h"

#include "canvas/CanvasContext.h"
#include "script/Script.h"
#include "AmityContext.h"

#include "log.h"
#include "assets.h"

#define FPS_INTERVAL (5000)

static SDL_Window* window;
static AmityContext amityCtx;

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

void loadScript (Script* script, const char *filename)
{
    // TODO: C++ stream IO
    char buffer[500 * 1024];
    SDL_RWops* asset = loadAsset(filename);
    if (asset == NULL) {
        exit(1);
    }
    int r = SDL_RWread(asset, buffer, sizeof(char), sizeof(buffer));
    buffer[r] = '\0';
    script->parse(filename, buffer);
}

void mainLoop ()
{
    SDL_Event event;
    int x = 0, y = 0;

    Uint32 fpsFrames = 0;
    Uint32 fpsTime = 0;
    Uint32 lastTime = SDL_GetTicks();

    loadScript(&amityCtx.script, "app.js");

    for (;;) {
        Uint32 startTime = SDL_GetTicks();

        while (SDL_PollEvent(&event)) {
            amityCtx.script.onEvent(&event);
            if (event.type == SDL_QUIT) {
                return;
            }
        }

        glClearColor(0.1, 0.1, 0.1, 1.0);
        glClear(GL_COLOR_BUFFER_BIT);

        Uint32 nowTime = SDL_GetTicks();
        Uint32 elapsed = nowTime - lastTime;
        lastTime = nowTime;

        ++fpsFrames;
        fpsTime += elapsed;
        if (fpsTime > FPS_INTERVAL) {
            float fps = 1000 * (float)fpsFrames / fpsTime;
            LOGI("FPS: %.2f (%i frames in %.2fs)", fps, fpsFrames, (float)fpsTime/1000);
            fpsTime = 0;
            fpsFrames = 0;
        }
        amityCtx.script.onEnterFrame(elapsed);

        SDL_GL_SwapWindow(window);
        delayFrame();
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

    window = SDL_CreateWindow(NULL, 0, 0, 0, 0,
        SDL_WINDOW_OPENGL | SDL_WINDOW_SHOWN | SDL_WINDOW_BORDERLESS);
    if (window == NULL) {
        LOGE("Unable to create window: %s", SDL_GetError());
        return 1;
    }
    amityCtx.window = window;

    int width, height;
    SDL_GetWindowSize(window, &width, &height);
    LOGI("Created window [width=%i, height=%i]", width, height);

    SDL_GL_CreateContext(window);
    SDL_GL_SetSwapInterval(1);

    glViewport(0, 0, width, height);
    glMatrixMode(GL_PROJECTION);
    glLoadIdentity();
    glOrthof(0, width, height, 0, 0, 1);
    glMatrixMode(GL_MODELVIEW);
    glLoadIdentity();

    mainLoop();

    LOGI("Quitting normally");
    return 0;
}
