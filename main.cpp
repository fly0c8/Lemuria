#include <SDL.h>
#include <iostream>
#include <sstream>

using namespace std;

//const char* HELLO_BMP = "/home/arnold/Pictures/hello.bmp";
const char* HELLO_BMP = "C:/Users/arnold/Pictures/hello.bmp";

SDL_Renderer* gRenderer = 0;
SDL_Window* gWindow = 0;
stringstream gErr;
SDL_Texture* gTex = 0;

SDL_Rect gPos { 0,0,800,600};
int gDx = 1;

void die(const char* error) {
    cerr << error << endl;
    exit(1);
}

SDL_Texture* loadTexture(const char* filepath) {

    SDL_Texture* newTexture = NULL;
    SDL_Surface* loadedSurface = SDL_LoadBMP(filepath);
    if(!loadedSurface) {
        gErr << "Cannot load image " << filepath;
        die(gErr.str().c_str());
    }
    newTexture = SDL_CreateTextureFromSurface(gRenderer,loadedSurface);    
    if(!newTexture){
        gErr << "Unable to create texture from " << filepath << endl;
        die(gErr.str().c_str());
    }
    SDL_FreeSurface(loadedSurface);
    return newTexture;
}

void initGame() {

    if(SDL_Init(SDL_INIT_VIDEO) < 0) die("cannot init SDL");

    gWindow = SDL_CreateWindow("LEMURIA",0,0,800,600,SDL_WINDOW_SHOWN | SDL_WINDOW_OPENGL | SDL_WINDOW_FULLSCREEN_DESKTOP);
    if(!gWindow) die("Cannot create window");

    gRenderer = SDL_CreateRenderer( gWindow, -1, SDL_RENDERER_ACCELERATED );
    if(!gRenderer) die("Cannot create renderer");

    SDL_SetRenderDrawColor( gRenderer, 0x0, 0x0, 0x33, 0xFF );

}
void cleanupGame() {
    SDL_DestroyRenderer(gRenderer);
    SDL_DestroyWindow(gWindow);
    SDL_Quit();
}

void updateGame()
{
    gPos.x+=gDx;
    if (gPos.x <0 || (gPos.x + gPos.w) > 1680) gDx*=-1;
}
void renderGame()
{
    SDL_RenderCopy(gRenderer,gTex, NULL, &gPos);
}

void loadResources()
{
    gTex = loadTexture(HELLO_BMP);
}
void freeResources()
{
    SDL_DestroyTexture(gTex);
}

int main(int argc, char *argv[])
{
    initGame();
    loadResources();

    // Gameloop
    bool running = true;
    SDL_Event e;

    while(running) {
        while(SDL_PollEvent(&e)) {
            if(e.type == SDL_KEYDOWN && e.key.keysym.sym == SDLK_ESCAPE) running = false;
        }
        updateGame();
        SDL_RenderClear(gRenderer);
        renderGame();
        SDL_RenderPresent(gRenderer);
    }

    cleanupGame();
    return 0;
}
