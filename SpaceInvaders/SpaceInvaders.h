#pragma once

#include "Alien.h"
#include "Player.h"
#include "Lazer.h"
#include "Barrier.h"
#include "Ship.h"

#include "../Engine/RectMap.h"
#include "../Engine/App.h"
#include "../Engine/EventArray.h"

struct App;
struct SDL_Texture;

typedef struct SpaceInvaders SpaceInvaders;
struct SpaceInvaders {
    Aliens aliens;
    Lazers lazers;
    Player player;
    Ship ship;
    RectMap rmap;
    SDL_Texture *sprite_sheet;
    uint32_t points;
    int8_t lives;
    Barriers barriers;
};

SpaceInvaders CreateSpaceInvaders(App *app);
void DestroySpaceInvaders(SpaceInvaders *sinvaders);

void RenderSpaceInvaders(SpaceInvaders *sinvaders, App *app);
void UpdateSpaceInvaders(SpaceInvaders *sinvaders, App *app, const EventArray *event_arr, uint32_t delta_time_ms);