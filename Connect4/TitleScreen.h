#pragma once
#include "../Engine/App.h"
#include "../Engine/EventArray.h"
#include "../Engine/Scene.h"
#include "../Engine/Button.h"

struct SDL_Renderer;
struct SDL_Texture;

typedef struct TitleScreenScene TitleScreenScene;

struct TitleScreenScene {
    Button button;
};

void CreateTitleScreenScene(TitleScreenScene *tss, App *app);
void DestroyTitleScreenScene(TitleScreenScene *tss);

void CreateTitleScreen(Scene *scene, App *app);
void UpdateTitleScreen(Scene *scene, const EventArray *event, uint32_t delta_time_ms);
void RenderTitleScreen(Scene *scene, App *renderer);
void DestroyTitleScreen(Scene *scene);

extern const SceneFnTable TITLE_SCREEN_SCENE_TABLE;