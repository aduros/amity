#include <cstdlib>
#include <cstdio>

#include "SDL.h"
#include "SDL_opengles.h"

#include "canvas/CanvasContext.h"
#include "script/Script.h"

#include "log.h"

// TODO: Test assets are looked up on the sdcard for now, eventually it should read from the APK
#define ASSET(filename) ("/sdcard/data/" filename)

extern CanvasContext canvas;
CanvasContext canvas;

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
    FILE* file = fopen(filename, "r");
    char buffer[500 * 1024];
    int r = fread(buffer, sizeof(char), sizeof(buffer), file);
    buffer[r] = '\0';
    fclose(file);

    script->parse(filename, buffer);
    //"var n = 0; $amity.onEnterFrame = function (dt) { $amity.log('elapsed ' + dt) };");
}

void mainLoop ()
{
    glEnableClientState(GL_VERTEX_ARRAY);

    SDL_Event event;
    int x = 0, y = 0;
    Uint32 elapsed = 0;

    Script script;
    loadScript(&script, ASSET("test.js"));

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

        glClearColor(0.1, 0.1, 0.1, 1.0);
        glClear(GL_COLOR_BUFFER_BIT);
        // I'm painting textures solid green for now
        glColor4f(0.0, 1.0, 0.0, 1.0);

        script.onEnterFrame(elapsed);

        SDL_RenderPresent();
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

    mainLoop();

    LOGI("Quitting normally");
    return 0;
}
