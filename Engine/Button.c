#include <assert.h>
#include <string.h>

#include "SDL2/SDL.h"
#include "SDL2/SDL_image.h"

#include "Button.h"
#include "EventArray.h"

#define TITLE_HEIGHT 100

Button CreateButton(SDL_Renderer *renderer, const Text *text, SDL_Rect rect, SDL_Color backgroundColor) {
    assert(text != NULL);

    return (Button){
        .rect = rect,
        .texture = CreateTextureFromText(renderer, "resources/times-new-roman.ttf", text),
        .clicked = false,
        .backgroundColor = backgroundColor
    };
}
void DestroyButton(Button *button) {
    assert(button != NULL);

    button->clicked = false;
    button->rect = (SDL_Rect){0};
    SDL_DestroyTexture(button->texture);
}

void RenderButton(const Button *button, SDL_Renderer *renderer) {
    assert(button != NULL);
    assert(renderer != NULL);

    SDL_Rect src = {0};
    SDL_QueryTexture(button->texture, NULL, NULL, &src.w, &src.h);

    SDL_Rect dest = button->rect;

    SDL_Rect texture = {0,0,0,0};
    SDL_QueryTexture(button->texture, NULL, NULL, &texture.w, &texture.h);

    float widthScale = ((float) button->rect.w) / ((float) texture.w);
    float heightScale = ((float) button->rect.h) / ((float) texture.h);

    if (widthScale < heightScale) {
        dest.w = texture.w * widthScale;
        dest.h = texture.h * widthScale;
    } else {
        dest.w = texture.w * heightScale;
        dest.h = texture.h * heightScale;
    }

    SDL_SetRenderDrawColor(
        renderer, 
        button->backgroundColor.r, 
        button->backgroundColor.g,
        button->backgroundColor.b, 
        button->backgroundColor.a
    );
    SDL_RenderFillRect(renderer, &button->rect);
    SDL_RenderCopy(renderer, button->texture, &src, &dest);
}
void UpdateButton(Button *button, const EventArray *event_arr) {
    assert(button != NULL);
    assert(event_arr != NULL);

    for (int i = 0; i < event_arr->n; ++i) {
        SDL_Event event = event_arr->event[i];

        if (event.type == SDL_MOUSEBUTTONDOWN) {
            SDL_Rect mouseRect = {
                .x = event.button.x,
                .y = event.button.y,
                1,
                1
            };
            if (SDL_HasIntersection(&mouseRect, &button->rect)) {
                button->clicked = true;
                break;
            }
        }
    }
}