#include <SDL.h>
#include <iostream>

using namespace std;

const char* HELLO_BMP = "/home/arnold/Pictures/hello.bmp";

SDL_Renderer* gRenderer = 0;
SDL_Window* gWindow = 0;

SDL_Texture* loadTexture(const char* filepath) {

    SDL_Texture* newTexture = NULL;
    SDL_Surface* loadedSurface = SDL_LoadBMP(filepath);
    if(!loadedSurface) {
        cerr << "Cannot load image " << filepath << endl;
        exit(1);
    }
    newTexture = SDL_CreateTextureFromSurface(gRenderer,loadedSurface);
    if(!newTexture){
        cerr << "Unable to create texture from " << filepath << endl;
        exit(1);
    }
    SDL_FreeSurface( loadedSurface );
    return newTexture;
}

int main()
{
    if(SDL_Init(SDL_INIT_VIDEO) < 0) {
        cerr << "Cannot init SDL" << endl;
        exit(1);
    }
    gWindow = SDL_CreateWindow("LEMURIA",0,0,800,600,SDL_WINDOW_SHOWN | SDL_WINDOW_OPENGL | SDL_WINDOW_FULLSCREEN_DESKTOP);
    if(!gWindow) {
        cerr << "Cannot create window" << endl;
        exit(1);
    }
    gRenderer = SDL_CreateRenderer( gWindow, -1, SDL_RENDERER_ACCELERATED );
    if(!gRenderer) {
        cerr << "Cannot create renderer" << endl;
        exit(1);
    }
    SDL_Texture* tex = loadTexture(HELLO_BMP);
    if(!tex) {
        cerr << "Cannot load texture" << HELLO_BMP << endl;
        exit(1);
    }

    SDL_SetRenderDrawColor( gRenderer, 0xFF, 0xFF, 0xFF, 0xFF );

    bool running = true;

    SDL_Event e;

    while(running) {
        while(SDL_PollEvent(&e)) {
            if(e.type == SDL_KEYDOWN && e.key.keysym.sym == SDLK_ESCAPE) running = false;
        }
        SDL_RenderClear(gRenderer);
        SDL_RenderCopy(gRenderer,tex,NULL,NULL);
        SDL_RenderPresent(gRenderer);
    }
    SDL_DestroyTexture(tex);
    SDL_DestroyRenderer(gRenderer);
    SDL_DestroyWindow(gWindow);
    SDL_Quit();

    return 0;
}

