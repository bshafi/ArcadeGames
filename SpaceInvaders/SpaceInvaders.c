#include <assert.h>
#include <stdio.h>
#include <stdbool.h>
#include <math.h>

#include <SDL2/SDL.h>
#include <SDL2/SDL_image.h>

#include "SpaceInvaders.h"

#include "../Engine/App.h"
#include "../Engine/EventArray.h"

#define FOR_I(var, MAX) for (int (var) = 0; (var) < (MAX); ++(var))
#define SCORE_POS (vec2i){ 38, 16 }

SpaceInvaders CreateSpaceInvaders(App *app) {
    assert(app);

    SpaceInvaders sinvaders;

    sinvaders.aliens = CreateAliens();

    sinvaders.rmap = ReadRectMapFromFile("resources/space_invaders.ss");
    sinvaders.sprite_sheet = IMG_LoadTexture(app->renderer, "resources/space_invaders.png");

    sinvaders.player = CreatePlayer();

    sinvaders.lazers = CreateLazers();
    sinvaders.barriers = CreateBarriers(app, sinvaders.sprite_sheet);
    sinvaders.ship = CreateShip();

    sinvaders.points = 0;
    sinvaders.lives = 3;

    return sinvaders;
}
void DestroySpaceInvaders(SpaceInvaders *sinvaders) {
    assert(sinvaders);

    DestroyRectMap(&sinvaders->rmap);
    SDL_DestroyTexture(sinvaders->sprite_sheet);
}


#define NUM_DIGITS 5
typedef struct num4digits num4digits;
struct num4digits {
    char digits[NUM_DIGITS];
};

num4digits num_to_num4digits(int num) {
    assert(num >= 0);
    //assert(num <= 9999);

    num4digits ret;
    for (int i = 0; i < NUM_DIGITS; ++i) {
        ret.digits[NUM_DIGITS - i - 1] = (num % 10) + '0';
        num /= 10;
    }
    return ret;
}

void RenderSpaceInvaders(SpaceInvaders *sinvaders, App *app) {
    assert(sinvaders);
    assert(app);
    
    const vec2i pos = SCORE_POS;
    num4digits h_digits = num_to_num4digits(sinvaders->points);
    char *digits = &h_digits.digits[0];
    for (int i = NUM_DIGITS - 1; i >= 0; --i) {
        char c = digits[i];
        if (!('0' <= c && c <= '9')) {
            c = '0';
        }
        const char sprite_name[] = { c, '\0' };
        const SDL_Rect src = GetFromRectMap(&sinvaders->rmap, sprite_name);
        assert(src.w != -1 && src.h != -1);
        const SDL_Rect dest = {
            .x = pos.x + (i * 8),
            .y = pos.y,
            .w = 8,
            .h = 8
        };
        RenderApp(app, sinvaders->sprite_sheet, &src, &dest);
    }

    {
        SDL_Rect src = GetFromRectMap(&sinvaders->rmap, "ship");
        SDL_Rect dst = { sinvaders->ship.pos.x, sinvaders->ship.pos.y, src.w, src.h };
        RenderApp(app, sinvaders->sprite_sheet, &src, &dst);
    }

    for (int i = 0; i < sinvaders->lives; ++i) {
        const SDL_Rect src = GetFromRectMap(&sinvaders->rmap, "player");
        const SDL_Rect dst = { (256 - 82) + (17 * i), 17, src.w, src.h };
        RenderApp(app, sinvaders->sprite_sheet, &src, &dst);
    }
    
    for (int row = 0; row < ALIEN_ROWS_COUNT; ++row)
        for (int column = 0; column < ALIEN_COMLUMNS_COUNT; ++column) {
            const Alien alien = sinvaders->aliens.aliens[row][column];

            SDL_Rect src, dest;
            
            if (alien.type != ALIEN_DYING && alien.type != ALIEN_DEAD) {
                src = AlienTypeToRect(alien.type, alien.frame, &sinvaders->rmap);
                assert(src.w != -1 && src.h != -1);
                dest = (SDL_Rect){
                    .x = alien.pos.x,
                    .y = alien.pos.y,
                    .w = src.w,
                    .h = src.h
                };
                RenderApp(app, sinvaders->sprite_sheet, &src, &dest);
            } else if (alien.type != ALIEN_DEAD) {
                src = GetFromRectMap(&sinvaders->rmap, "explosion");
                assert(src.w != -1 && src.h != -1);
                dest = (SDL_Rect){
                    .x = alien.pos.x,
                    .y = alien.pos.y,
                    .w = src.w,
                    .h = src.h
                };
                RenderApp(app, sinvaders->sprite_sheet, &src, &dest);
            }
        }
    

    for (int i = 0; i < BARRIERS_COUNT; ++i) {
        DrawBarrier(&sinvaders->barriers.barriers[i], app->renderer);
    }

    for (int i = 0; i < LAZER_COUNT; ++i) {
        const Lazer lazer = sinvaders->lazers.lazers[i];
        if (lazer.type == LAZER_NONE) {
            continue;
        }
        char buffer[100];
        strcpy(buffer, LazerTypeToStr(lazer.type));
        char suffix[] = "_0";
        suffix[1] += ((char)lazer.frame);
        strcat(buffer, suffix);
        SDL_Rect src = GetFromRectMap(&sinvaders->rmap, buffer);
        assert(src.w != -1 && src.h != -1);
        SDL_Rect dest = {
            .x = lazer.pos.x,
            .y = lazer.pos.y,
            .w = src.w,
            .h = src.h
        };
        RenderApp(app, sinvaders->sprite_sheet, &src, &dest);
    }

    {
        SDL_Rect src = GetFromRectMap(&sinvaders->rmap, "player");
        assert(src.w != -1 && src.h != -1);
        SDL_Rect dest = {
            .x = sinvaders->player.pos.x,
            .y = sinvaders->player.pos.y,
            .w = src.w,
            .h = src.h
        };
        RenderApp(app, sinvaders->sprite_sheet, &src, &dest);
    }

} 

bool SpacePressed(const SDL_Event *event) {
    assert(event);
    return event->type == SDL_KEYDOWN && event->key.keysym.scancode == SDL_SCANCODE_SPACE;
}
void UpdateSpaceInvaders(SpaceInvaders *sinvaders, App *app, const EventArray *event_arr, uint32_t delta_time_ms) {
    assert(sinvaders);
    assert(app);
    assert(event_arr);
    
    UpdatePlayer(&sinvaders->player, delta_time_ms);
    UpdateAliens(&sinvaders->aliens, delta_time_ms);
    UpdateLazers(&sinvaders->lazers, delta_time_ms);

    if (QueryEventArray(event_arr, NULL, SpacePressed)) {
        Lazer lazer = {
            .type = LAZER_SLIDE,
            .pos = (vec2i){ sinvaders->player.pos.x + 8, sinvaders->player.pos.y - 8 },
            .frame = 0,
            .direction = 0
        };
        AddLazer(&sinvaders->lazers, lazer);
    }
    
    if (rand() < RAND_MAX / 10) {
        const int delta_x = RAND_MAX / ALIEN_COMLUMNS_COUNT;
        int column = (rand() / delta_x);
        if (column == ALIEN_COMLUMNS_COUNT) {
            --column;
        }
        for (int row = ALIEN_ROWS_COUNT - 1; row >= 0; --row) {
            if (sinvaders->aliens.aliens[row][column].type != ALIEN_DYING && sinvaders->aliens.aliens[row][column].type != ALIEN_DEAD) {
                const Alien alien = sinvaders->aliens.aliens[row][column];
                Lazer lazer = {
                    .type = (LazerType)(rand() % 4),
                    .pos = (vec2i){ alien.pos.x + 4, alien.pos.y + 8 },
                    .frame = 0,
                    .direction = 1
                };
                AddLazer(&sinvaders->lazers, lazer);
                break;
            }
        }
    }

    if (sinvaders->ship.isMoving) {
        sinvaders->ship.pos.x += 1;
    }
    if (!sinvaders->ship.isMoving && rand() < RAND_MAX / 500) {
        sinvaders->ship.isMoving = true;
    }
    if (sinvaders->ship.pos.x > 256) {
        sinvaders->ship = CreateShip();
    }

    FOR_I(lazer_index, LAZER_COUNT) {
        SDL_Rect ship_rect = GetFromRectMap(&sinvaders->rmap, "ship");
        ship_rect.x = sinvaders->ship.pos.x;
        ship_rect.y = sinvaders->ship.pos.y;
        //Ship *ship = &sinvaders->ship;
        Lazer *lazer = &sinvaders->lazers.lazers[lazer_index];

        SDL_Rect lazer_rect = {
            .x = lazer->pos.x,
            .y = lazer->pos.y,
            .w = 3,
            .h = 8
        };
        if (SDL_HasIntersection(&ship_rect, &lazer_rect)) {
            lazer->type = LAZER_NONE;
            sinvaders->ship = CreateShip();
            sinvaders->points += 1000;
        }
    }

    FOR_I(lazer_index, LAZER_COUNT) {
        FOR_I(barrier_index, BARRIERS_COUNT) {
            Lazer *lazer = &sinvaders->lazers.lazers[lazer_index];
            Barrier *barrier = &sinvaders->barriers.barriers[barrier_index];
            if (BarrierHasCollision(barrier, lazer, NULL)) {
                lazer->type = LAZER_NONE;
                // Handle barrier lazer intersection
                BarrierHandleColision(barrier, lazer);
            }
        }
    }
    
    {
        Player *player = &sinvaders->player;
        if (player->invincibility_ms == 0)
            FOR_I(lazer_index, LAZER_COUNT) {
                Lazer *lazer = &sinvaders->lazers.lazers[lazer_index];
                SDL_Rect lazer_rect = { lazer->pos.x, lazer->pos.y, LAZER_WIDTH, LAZER_HEIGHT };
                Player *player = &sinvaders->player;
                SDL_Rect player_rect = GetFromRectMap(&sinvaders->rmap, "player");
                player_rect.x = player->pos.x;
                player_rect.y = player->pos.y;
                if (SDL_HasIntersection(&lazer_rect, &player_rect)) {
                    //*player = CreatePlayer();
                    player->invincibility_ms = 500;
                    --sinvaders->lives;
                    if (sinvaders->lives <= 0) {
                        SDL_Event event;
                        SDL_zero(event);
                        event.type = GAMEOVER_EVENT;
                        SDL_PushEvent(&event);
                    }
                    lazer->type = LAZER_NONE;
                }
            }
    }

    FOR_I(lazer_index, LAZER_COUNT)
        FOR_I(row, ALIEN_ROWS_COUNT) 
            FOR_I(column, ALIEN_COMLUMNS_COUNT) {
                Alien *alien = &sinvaders->aliens.aliens[row][column];
                Lazer *lazer = &sinvaders->lazers.lazers[lazer_index];

                if (lazer->type == LAZER_NONE || alien->type == ALIEN_DYING || alien->type == ALIEN_DEAD) {
                    continue;
                }

                SDL_Rect lazer_rect = {
                    .x = lazer->pos.x,
                    .y = lazer->pos.y,
                    .w = 3,
                    .h = 8
                };
                SDL_Rect alien_rect = {
                    .x = alien->pos.x,
                    .y = alien->pos.y,
                    .w = 16,
                    .h = 8
                };

                if (SDL_HasIntersection(&lazer_rect, &alien_rect)) {
                    lazer->type = LAZER_NONE;
                    sinvaders->points += alien->type * 100;
                    alien->type = ALIEN_DYING;
                }
            }
}