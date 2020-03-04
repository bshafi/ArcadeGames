#include <assert.h>
#include <stdio.h>

#include <SDL2/SDL.h>
#include <SDL2/SDL_image.h>

#include "Player.h"

#define PLAYER_MAX (256 - 33)
#define PLAYER_MIN (33)
#define PLAYER_START_POS (vec2f){ 33, 192 }

Player CreatePlayer() {
    return (Player){
        .pos = PLAYER_START_POS,
        .invincibility_ms = 300
    };
}
void MovePlayer(Player *player, float amount) {
    assert(player);
    if (
        (amount < 0 && PLAYER_MIN < player->pos.x) ||
        (amount > 0 && PLAYER_MAX > player->pos.x)
    ) {
        player->pos.x += amount;
    }
}
void UpdatePlayer(Player *player, const uint32_t delta_time_ms) {
    assert(player);

    const uint8_t *keyboard = SDL_GetKeyboardState(NULL);

    if (keyboard[SDL_SCANCODE_LEFT]) {
        MovePlayer(player, delta_time_ms * -0.1f);
    } else if (keyboard[SDL_SCANCODE_RIGHT]) {
        MovePlayer(player, delta_time_ms * 0.1f);
    }

    if (player->invincibility_ms > delta_time_ms) {
        player->invincibility_ms -= delta_time_ms;
    } else {
        player->invincibility_ms = 0;
    }
}