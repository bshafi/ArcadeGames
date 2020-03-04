#pragma once

struct SDL_Texture;
struct SDL_Renderer;
struct Connect4;

typedef struct GUI GUI;

struct GUI {
    SDL_Texture *background;
};

typedef struct Sprite Sprite;
struct Sprite {

};

void CreateGUI(struct GUI *gui, SDL_Renderer *renderer);
void RenderGUI(struct GUI *gui, SDL_Renderer *renderer, const struct Connect4 *game);
void DestroyGUI(struct GUI *gui);