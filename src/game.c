#include "include/game.h"
#include "include/entity.h"
#include "include/player.h"
#include "include/render.h"

#include <SDL2/SDL_render.h>

void Game_Init() {
    Player_Init();
}

void Game_Main(int buttons) {
    Renderer_Prepare();
    Player_Update(buttons);
    Entity_UpdateAll();
    Renderer_Update();
}