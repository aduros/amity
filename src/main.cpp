#include <cstdlib>
#include <cstdio>

#include "SDL.h"
#include "SDL_opengles.h"

#ifdef __WEBOS__
#include "PDL.h"
#endif

#include "canvas/CanvasContext.h"
#include "script/Script.h"
#include "AmityContext.h"

#include "log.h"
#include "assets.h"

#define FPS_INTERVAL (5000)

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

#if SDL_VERSION_ATLEAST(1,3,0)
        SDL_GL_SwapWindow(amityCtx.window);
#else
        SDL_GL_SwapBuffers();
#endif
        delayFrame();
    }
}

int main (int argc, char* argv[])
{
#ifdef __WEBOS__
    openlog("com.threerings.amity", 0, LOG_USER);
#endif

    LOGI("Main screen turn on, compiled " __DATE__ " at " __TIME__);

    if (SDL_Init(SDL_INIT_VIDEO | SDL_INIT_AUDIO | SDL_INIT_NOPARACHUTE) < 0) {
        LOGE("Unable to init SDL: %s", SDL_GetError());
        return 1;
    }
    atexit(SDL_Quit);

#ifdef __WEBOS__
    PDL_Init(0);
    atexit(PDL_Quit);
#endif

    SDL_GL_SetAttribute(SDL_GL_CONTEXT_MAJOR_VERSION, 1);
    SDL_GL_SetAttribute(SDL_GL_DEPTH_SIZE, 0);
    SDL_GL_SetAttribute(SDL_GL_RETAINED_BACKING, 0);
    SDL_GL_SetAttribute(SDL_GL_ACCELERATED_VISUAL, 1);

#if SDL_VERSION_ATLEAST(1,3,0)
    SDL_Window* window = SDL_CreateWindow(NULL, 0, 0, 0, 0,
        SDL_WINDOW_OPENGL | SDL_WINDOW_SHOWN | SDL_WINDOW_BORDERLESS);
    if (window == NULL) {
        LOGE("Unable to create window: %s", SDL_GetError());
        return 1;
    }
    amityCtx.window = window;

    int width, height;
    SDL_GetWindowSize(window, &width, &height);

    SDL_GL_CreateContext(window);
    SDL_GL_SetSwapInterval(1); // Enable vsync
#else
    amityCtx.screen = SDL_SetVideoMode(0, 0, 0, SDL_OPENGL);
    int width = amityCtx.screen->w;
    int height = amityCtx.screen->h;
#endif

    LOGI("Created window [width=%i, height=%i]", width, height);

    glViewport(0, 0, width, height);
    glMatrixMode(GL_PROJECTION);
    glLoadIdentity();
    glOrthof(0, width, height, 0, 0, 1);
    glMatrixMode(GL_MODELVIEW);
    glLoadIdentity();

    glEnable(GL_TEXTURE_2D);
    glEnableClientState(GL_VERTEX_ARRAY);
    glEnableClientState(GL_TEXTURE_COORD_ARRAY);
    glEnable(GL_BLEND);

    mainLoop();

    LOGI("Quitting normally");
    return 0;
}
