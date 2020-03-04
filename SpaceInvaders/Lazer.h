#pragma once

#include "../Engine/vec2i.h"

typedef enum LazerType LazerType;
enum LazerType {
    LAZER_ZIG,
    LAZER_SLIDE,
    LAZER_BARB,
    LAZER_NONE
};

const char *LazerTypeToStr(LazerType lazer_type);

typedef struct Lazer Lazer;
struct Lazer {
    LazerType type : 2;
    uint8_t frame : 2;
    uint8_t direction : 1;
    vec2i pos;
};

#define LAZER_COUNT 20
#define LAZER_WIDTH 3
#define LAZER_HEIGHT 8

#define LAZER_UP 0
#define LAZER_DOWN 1

typedef struct Lazers Lazers;
struct Lazers {
    Lazer lazers[LAZER_COUNT];
    uint32_t ms_elasped;
};

Lazers CreateLazers();
void UpdateLazers(Lazers *, const uint32_t delta_time_ms);
void AddLazer(Lazers *, Lazer);

void CreateLazer(Lazer);
