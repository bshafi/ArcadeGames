#include <assert.h>
#include <stdlib.h>
#include <string.h>
#include <stdio.h>

#include "Alien.h"

#define FIRST_ALIEN_POS (vec2i){ 0, 0 }

const char *AlienTypeToStr(AlienType type) {
    switch (type) {
        case ALIEN_0:
            return "alien0";
        case ALIEN_1:
            return "alien1";
        case ALIEN_2:
            return "alien2";
        case ALIEN_DYING:
            return NULL;
        case ALIEN_DEAD:
            return NULL;
    }
    return NULL;
}

SDL_Rect AlienTypeToRect(AlienType type, uint32_t frame, const RectMap *rmap) {
    assert(rmap);
    char buffer[100];
    strcpy(buffer, AlienTypeToStr(type));
    char suffix[] = "_0";
    suffix[1] += ((char)frame);
    strcat(buffer, suffix);
    return GetFromRectMap(rmap, buffer);
}

Aliens CreateAliens() {
    Aliens aliens;
    for (int row = 0; row < ALIEN_ROWS_COUNT; ++row)
        for (int column = 0; column < ALIEN_COMLUMNS_COUNT; ++column) {
            

            Alien alien;
            if (0 == row) {
                alien.type = ALIEN_0;
            } else if (1 <= row && row <= 2) {
                alien.type = ALIEN_1;
            } else if (3 <= row && row <= 4) {
                alien.type = ALIEN_2;
            }
            alien.pos = (vec2i) {
                .x = 40 + (16 * column),
                .y = 40 + (16 * row)
            };
            alien.frame = 0;
            aliens.aliens[row][column] = alien;
        }
        
    aliens.ms_elasped = 0;
    aliens.curr_alien = FIRST_ALIEN_POS;
    aliens.pos = 0;
    aliens.direction = ALIEN_MOVE_RIGHT;
    return aliens;
}

vec2i NextAlien(const vec2i curr_alien) {
    vec2i next_alien = {
        .x = curr_alien.x + 1,
        .y = curr_alien.y
    };
    if (next_alien.x >= ALIEN_ROWS_COUNT) {
        next_alien.x = 0;
        ++next_alien.y;
    }
    if (next_alien.y >= ALIEN_COMLUMNS_COUNT) {
        next_alien.y = 0;
    }
    return next_alien;
}

void UpdateAliens(Aliens *aliens, uint32_t delta_time_ms) {
    assert(aliens);

#define MAX 30
#define MIN -MAX

    Alien *alien = &aliens->aliens[aliens->curr_alien.x][aliens->curr_alien.y];
    if (alien->type == ALIEN_DYING) {
        if (alien->frame == 1) {
            alien->type = ALIEN_DEAD;
        } else if (alien->frame == 0) {
            alien->frame = 1;
        }
    } else {
        alien->frame = alien->frame == 0 ? 1 : 0;
    }
    const vec2i next_alien = NextAlien(aliens->curr_alien);
    switch (aliens->direction) {
        case ALIEN_MOVE_DOWN_LEFT:
            alien->pos.y += 8;
            if (next_alien.x == 0 && next_alien.y == 0) {
                aliens->direction = ALIEN_MOVE_RIGHT;
            }
            break;
        case ALIEN_MOVE_DOWN_RIGHT:
            alien->pos.y += 8;
            if (next_alien.x == 0 && next_alien.y == 0) {
                aliens->direction = ALIEN_MOVE_LEFT;
            }
            break;
        case ALIEN_MOVE_LEFT:
            alien->pos.x += -1;
            if (next_alien.x == 0 && next_alien.y == 0) {
                aliens->pos += -1;
            }
            if (aliens->pos < MIN) {
                aliens->direction = ALIEN_MOVE_DOWN_LEFT;
            }
            break;
        case ALIEN_MOVE_RIGHT:
            alien->pos.x += 1;
            if (next_alien.x == 0 && next_alien.y == 0) {
                aliens->pos += 1;
            }
            if (aliens->pos > MAX) {
                aliens->direction = ALIEN_MOVE_DOWN_RIGHT;
            }
            break;
    }

    aliens->curr_alien = next_alien;
#undef MIN
#undef MAX
}