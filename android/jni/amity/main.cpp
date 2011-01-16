#include <cstdlib>
#include "SDL.h"

#include "log.h"

int main (int argc, char* argv[])
{
    LOGI("Main screen turn on");

    if (SDL_Init(SDL_INIT_VIDEO | SDL_INIT_AUDIO) < 0) {
        LOGE("Unable to init SDL: %s", SDL_GetError());
        return 1;
    }
    atexit(SDL_Quit);

    SDL_Surface* screen = SDL_SetVideoMode(
        0, 0, 0, SDL_DOUBLEBUF | SDL_HWSURFACE);
    if (screen == NULL) {
        LOGE("Unable to set up screen: %s", SDL_GetError());
        return 1;
    }

    LOGI("Created screen [width=%i, height=%i]", screen->w, screen->h);

    //SDL_Surface *temp;

    //temp = SDL_LoadBMP("image.bmp");
    //if (temp == NULL) {
    //    printf("Unable to load bitmap: %s", SDL_GetError());
    //    return 1;
    //}

    //image = SDL_DisplayFormat(temp);
    //SDL_FreeSurface(temp);

    //SDL_Rect src, dest;

    //src.x = 0;
    //src.y = 0;
    //src.w = image->w;
    //src.h = image->h;

    //dest.x = 100;
    //dest.y = 100;
    //dest.w = image->w;
    //dest.h = image->h;

    //SDL_BlitSurface(image, &src, screen, &dest);
    
    SDL_Rect dest = { 100, 100, 150, 200 };
    SDL_FillRect(screen, &dest, SDL_MapRGB(screen->format, 255, 0, 0));

    SDL_Flip(screen);
    SDL_Delay(10000);

    LOGI("Bye!");
    return 0;
}
