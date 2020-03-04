#include <assert.h>
#include <stdio.h>

#include <SDL2/SDL.h>
#include <SDL2/SDL_image.h>


#include "App.h"
#include "EventArray.h"

EventArray CreateEventArray() {

    return (EventArray) {
        .event = NULL,
        .n = 0
    };
}

void PollAllEvents(EventArray *event_array) {
    assert(event_array != NULL);

    SDL_PumpEvents();

    int num_events = SDL_PeepEvents(NULL, 0, SDL_PEEKEVENT, SDL_FIRSTEVENT, SDL_LASTEVENT);
    assert(num_events >= 0);

    event_array->event = realloc(event_array->event, sizeof(SDL_Event) * num_events);
    event_array->n = num_events;
    assert(event_array->event != NULL);

    SDL_PeepEvents(event_array->event, event_array->n, SDL_GETEVENT, SDL_FIRSTEVENT, SDL_LASTEVENT);
//    SDL_Event event;
//    for (int i = 0; i < event_array->n && SDL_PollEvent(&event); ++i) {
//        event_array->event[i] = event;
//    }
}
void PollEvents(EventArray *event_array, uint32_t min_type, uint32_t max_type) {
    SDL_PumpEvents();
    int num_events = SDL_PeepEvents(NULL, 0, SDL_PEEKEVENT, min_type, max_type);
    assert(num_events >= 0);

    event_array->event = realloc(event_array->event, sizeof(SDL_Event) * num_events);
    event_array->n = num_events;
//    assert(event_array->n != 0 && event_array->event == NULL);

    SDL_PeepEvents(event_array->event, event_array->n, SDL_GETEVENT, min_type, max_type);
}

void DestroyEventArray(EventArray *event_array) {
    assert(event_array != NULL);

    if (event_array->event != NULL) {
        free(event_array->event);
    }
    event_array->event = NULL;
    event_array->n = 0;
}

bool QueryEventArray(const EventArray *event_arr, SDL_Event *event, bool(*query)(const SDL_Event*)) {
    assert(event_arr != NULL);
    assert(query != NULL);

    for (int i = 0; i < event_arr->n; ++i) {
        if (query(&event_arr->event[i])) {
            if (event != NULL) {
                *event = event_arr->event[i];
            }
            return true;
        }
    }
    return false;
}

bool ShouldQuit(const SDL_Event *event) {
    assert(event);

    return event->type == SDL_QUIT;
}
bool QPressed(const SDL_Event *event) {
    assert(event);
    
    return event->type == SDL_KEYDOWN && event->key.keysym.scancode == SDL_SCANCODE_Q;
}
bool GameIsOver(const SDL_Event *event) {
    assert(event);

    return event->type == GAMEOVER_EVENT;
}