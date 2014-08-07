#include <SDL.h>
#include <SDL_ttf.h>
#include <iostream>
#include <sstream>
#include <assert.h>

using namespace std;

const char* HELLO_BMP = "/home/arnold/Pictures/hello.bmp";
//const char* HELLO_BMP = "C:/Users/arnold/Pictures/hello.bmp";
const char* FONT = "/home/arnold/Fonts/DejaVuSans.ttf";


const char* MESSAGE_TEXT = "Please note that smoking is prohibited on this platform. Violators will be prosecuted. Please remain seated until the smoking sign stops. Please hold on to the handlebars...";
SDL_Texture* gMsgTex = 0;
int gMsgX = 1280;

SDL_Renderer* gRenderer = 0;
SDL_Window* gWindow = 0;
stringstream gErr;
SDL_Texture* gTex = 0;
TTF_Font* gFont = 0;

SDL_Rect gPos { 0,300,800,600};
int gDx = 20;
int gFps = 0;
SDL_Color WHITE = { 0xff,0xff,0xff };
char gFpsString[255];

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

    gWindow = SDL_CreateWindow("LEMURIA",0,0,1280,720,
                               SDL_WINDOW_SHOWN |
                               SDL_WINDOW_OPENGL |
                               SDL_WINDOW_FULLSCREEN_DESKTOP
    );
    if(!gWindow) die("Cannot create window");

    gRenderer = SDL_CreateRenderer( gWindow, -1, SDL_RENDERER_ACCELERATED | SDL_RENDERER_PRESENTVSYNC);
    if(!gRenderer) die("Cannot create renderer");

    if(TTF_Init() == -1 ) die("cannot init TTF");

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
    if (gPos.x <0 || (gPos.x + gPos.w) > 1920) gDx*=-1;

    gMsgX-=10;
    if (gMsgX < -3000) gMsgX = 1280;
}

SDL_Texture* createTextureFromText(const char* text) {

    SDL_Surface* textSurface = TTF_RenderText_Solid(gFont,text,WHITE);
    assert(textSurface);

    SDL_Texture* tex = SDL_CreateTextureFromSurface(gRenderer,textSurface);
    SDL_FreeSurface(textSurface);

    assert(tex);
    return tex;
}

void renderGame()
{
    sprintf(gFpsString,"%d",gFps);
    SDL_Texture* fpsTex = createTextureFromText(gFpsString);

    SDL_RenderCopy(gRenderer,gTex, NULL, &gPos);

    SDL_Rect textPos { 10,10,100,30 };
    SDL_RenderCopy(gRenderer,fpsTex,NULL,&textPos);

    SDL_DestroyTexture(fpsTex);

    Uint32 format;
    int access;
    int w; int h;
    SDL_QueryTexture(gMsgTex, &format, &access,&w,&h);

    SDL_Rect msgPos { gMsgX, 10, w, h };
    SDL_RenderCopy(gRenderer,gMsgTex,NULL,&msgPos);
}


void loadResources()
{
    gTex = loadTexture(HELLO_BMP);
    gFont = TTF_OpenFont(FONT,96);
    if(!gFont) {
        gErr << "TTF_OpenFont failed:" << TTF_GetError();
        die(gErr.str().c_str());
    }

    gMsgTex = createTextureFromText(MESSAGE_TEXT);
    assert(gMsgTex);
}
void freeResources()
{
    SDL_DestroyTexture(gMsgTex);
    SDL_DestroyTexture(gTex);
}

int main(int argc, char *argv[])
{
    initGame();
    loadResources();

    // Gameloop
    bool running = true;
    SDL_Event e;

    int lastUpdate = 0;
    int frameCounter = 0;


    while(running) {

        frameCounter++;
        if(SDL_GetTicks() - lastUpdate > 1000) {
            lastUpdate = SDL_GetTicks();
            gFps = frameCounter;
            frameCounter = 0;
        }

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
