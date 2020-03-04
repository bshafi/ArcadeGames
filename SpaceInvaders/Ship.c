#include "Ship.h"


Ship CreateShip() {
    return (Ship){
        .pos = (vec2i){ .x = -20, .y = 30 },
        .isMoving = false
    };
}