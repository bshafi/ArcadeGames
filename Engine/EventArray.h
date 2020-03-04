#pragma once

#include <stdint.h>
#include <stdbool.h>

typedef struct EventArray EventArray;

union SDL_Event;

struct EventArray {
    SDL_Event *event;
    size_t n;
};

EventArray CreateEventArray();
void PollAllEvents(EventArray *event_array);
void PollEvents(EventArray *event_array, uint32_t min_type, uint32_t max_type);
void DestroyEventArray(EventArray *event_array);
// returns the first true or returns false
bool QueryEventArray(const EventArray *event_arr, SDL_Event *event, bool(*query)(const SDL_Event*));

bool ShouldQuit(const SDL_Event *event);
bool QPressed(const SDL_Event *event);
bool GameIsOver(const SDL_Event *event);