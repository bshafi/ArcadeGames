#include <assert.h>

#include "SDL2/SDL.h"
#include "SDL2/SDL_image.h"

#include <SDL2/SDL.h>
#include <SDL2/SDL_ttf.h>

#include "App.h"

App CreateApp(const char *name, const size_t width, const size_t height) {
    SDL_Init(SDL_INIT_EVERYTHING);
    TTF_Init();
    IMG_Init(IMG_INIT_PNG);
    
    App app;

    CHANGE_SCENE_EVENT = SDL_RegisterEvents(1);
    assert(CHANGE_SCENE_EVENT != ((uint32_t)-1));

    CREATE_LAZER_EVENT = SDL_RegisterEvents(1);
    assert(CREATE_LAZER_EVENT != ((uint32_t)-1));

    USEREVENT_TYPE = SDL_RegisterEvents(1);
    assert(USEREVENT_TYPE != ((uint32_t)-1));

    GAMEOVER_EVENT = SDL_RegisterEvents(1);
    assert(GAMEOVER_EVENT != ((uint32_t)-1));

    SDL_CreateWindowAndRenderer(width, height, SDL_WINDOW_SHOWN | SDL_WINDOW_RESIZABLE | SDL_WINDOW_ALLOW_HIGHDPI, &app.window, &app.renderer);
    SDL_SetWindowTitle(app.window, name);
    SDL_SetWindowPosition(app.window, SDL_WINDOWPOS_CENTERED, SDL_WINDOWPOS_CENTERED);
    SDL_RenderSetLogicalSize(app.renderer, (int)width, (int)height);


    app.scale = 1.0f;
    app.offset = (vec2i){ 0, 0 };

    return app;
}
void DestroyApp(App *app) {
    assert(app != NULL);

    SDL_DestroyRenderer(app->renderer);
    SDL_DestroyWindow(app->window);

    IMG_Quit();
    TTF_Quit();
    SDL_Quit();
}

void RenderApp(App *app, SDL_Texture *texture, const SDL_Rect *src, const SDL_Rect *dest) {
    assert(app);
    assert(texture);

    SDL_RenderCopy(app->renderer, texture, src, dest);
}


void SetAppCamera(App *app, const size_t w, const size_t h) {
    assert(app != NULL);

    SDL_RenderSetLogicalSize(app->renderer, w, h);
}
void TakeScreenShot(App *app) {
    assert(app);

    SDL_Rect wrect;
    SDL_RenderGetViewport(app->renderer, &wrect);
    SDL_Surface *sshot = SDL_CreateRGBSurface(0, wrect.w, wrect.h, 32, 0x00ff0000, 0x0000ff00, 0x000000ff, 0xff000000);
    SDL_RenderReadPixels(app->renderer, NULL, SDL_PIXELFORMAT_ARGB8888, sshot->pixels, sshot->pitch);
    SDL_SaveBMP(sshot, "resources/screenshot.bmp");
    SDL_FreeSurface(sshot);
}

void PushSceneChangeEvent(const char *scene_name) {
    SDL_Event event;
    SDL_zero(event);

    event.type = USEREVENT_TYPE;
    event.user.data1 = (void*)strdup(scene_name);
    int err = SDL_PushEvent(&event);
    assert(err != -1);
}