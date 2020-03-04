#include <SDL2/SDL.h>

#include "Engine/App.h"
#include "Engine/EventArray.h"

#include "SpaceInvaders/SpaceInvaders.h"

#define FPS 30
#define MS_PER_FRAME 1000 / FPS

int main() {
    App app = CreateApp("Space Invaders", 502, 448);
    SetAppCamera(&app, 256, 224);

    SpaceInvaders sinvaders = CreateSpaceInvaders(&app);

    uint32_t ms_elasped = 0;

    EventArray events = CreateEventArray();
    bool game_over = false;
    while (true) {
        SDL_PumpEvents();
        uint32_t delta_time_ms = SDL_GetTicks() - ms_elasped;
        if (delta_time_ms < MS_PER_FRAME) {
            SDL_Delay(MS_PER_FRAME - delta_time_ms);
            delta_time_ms = MS_PER_FRAME;
        }
        ms_elasped += delta_time_ms;

        PollEvents(&events, SDL_FIRSTEVENT, SDL_LASTEVENT);
        if (QueryEventArray(&events, NULL, ShouldQuit)) {
            break;
        }
        if (QueryEventArray(&events, NULL, QPressed)) {
            TakeScreenShot(&app);
        }
        if (QueryEventArray(&events, NULL, GameIsOver)) {
            game_over = true;
        }
        UpdateSpaceInvaders(&sinvaders, &app, &events, delta_time_ms);        

        SDL_SetRenderDrawColor(app.renderer, 0, 0, 0, 255);
        SDL_RenderClear(app.renderer);

        RenderSpaceInvaders(&sinvaders, &app);

        SDL_RenderPresent(app.renderer);
        if (game_over) {
            SDL_SetRenderDrawColor(app.renderer, 0, 0, 0, 255);
            SDL_RenderClear(app.renderer);
            SDL_Delay(1000);
            DestroySpaceInvaders(&sinvaders);
            sinvaders = CreateSpaceInvaders(&app);
            game_over = false;
        }
    }

    DestroySpaceInvaders(&sinvaders);
    DestroyApp(&app);
}