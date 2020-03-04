#include <assert.h>
#include <SDL2/SDL.h>
#include "SDL2/SDL_image.h"
#include <SDL2/SDL_ttf.h>

#include "Text.h"

SDL_Texture *CreateTextureFromText(SDL_Renderer *renderer, const char* fontFilePath, const Text *text) {
    assert(renderer != NULL);
    assert(fontFilePath != NULL);
    assert(text != NULL);
    
    TTF_Font *font = TTF_OpenFont(fontFilePath, text->ptsize);
    assert(font != NULL);

    SDL_Surface *sur = TTF_RenderText_Blended(font, text->text, text->textColor);

    SDL_Texture *tex = SDL_CreateTextureFromSurface(renderer, sur);

    SDL_FreeSurface(sur);
    TTF_CloseFont(font);

    return tex;
}