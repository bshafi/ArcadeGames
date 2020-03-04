#pragma once

#include "../Engine/vec2i.h"

#define PLAYER_STR "player"

typedef struct Player Player;
struct Player {
    vec2f pos;
    uint32_t invincibility_ms;
};

Player CreatePlayer();
void MovePlayer(Player *player, float amount);
void UpdatePlayer(Player *player, const uint32_t delta_time_ms);