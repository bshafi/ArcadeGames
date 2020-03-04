#include "../Engine/vec2i.h"

STRUCT (Ship) {
    vec2i pos;
    bool isMoving;
};


Ship CreateShip();