#pragma once

#include "../Engine/vec2i.h"
#include "../Engine/RectMap.h"

typedef enum AlienType AlienType;
enum AlienType {
    ALIEN_0,
    ALIEN_1,
    ALIEN_2,
    ALIEN_DYING,
    ALIEN_DEAD
};

typedef enum AlienMove AlienMove;
enum AlienMove {
    ALIEN_MOVE_DOWN_LEFT,
    ALIEN_MOVE_DOWN_RIGHT,
    ALIEN_MOVE_LEFT,
    ALIEN_MOVE_RIGHT
};

const char *AlienTypeToStr(AlienType type);

typedef struct Alien Alien;
struct Alien {
    AlienType type;
    vec2i pos;
    uint8_t frame;
};

#define ALIEN_ROWS_COUNT 5
#define ALIEN_COMLUMNS_COUNT 11

typedef struct Aliens Aliens;
struct Aliens {
    Alien aliens[ALIEN_ROWS_COUNT][ALIEN_COMLUMNS_COUNT];
    uint32_t ms_elasped;
    vec2i curr_alien;
    int32_t pos;
    AlienMove direction;
};

Aliens CreateAliens();
void UpdateAliens(Aliens *, uint32_t delta_time_ms);
SDL_Rect AlienTypeToRect(AlienType type, uint32_t frame, const RectMap *rmap);