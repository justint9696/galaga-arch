#include "gfx/app.h"

#include "game/buttons.h"
#include "game/game.h"

#include "entity/player.h"

#include <SDL2/SDL_events.h>
#include <SDL2/SDL_render.h>
#include <SDL2/SDL_scancode.h>

void Poll_Events() {
    SDL_Event event;
    while (SDL_PollEvent(&event)) {
        switch (event.type) {
            case SDL_QUIT:
                printf("%s Terminated.\n", GAME_TITLE);
                exit(0);
                break;

            case SDL_KEYDOWN:
            case SDL_KEYUP:
                Buttons_Update(event);
                break;

            default:
                break;
        }
    }
}

int main(int argc, char *argv[]) {
    initSDL();

    Game game;
    Game_Init(&game);
    printf("%s Initialized.\n", GAME_TITLE);

    while (Game_IsRunning(&game)) {
        Poll_Events();
        Game_Main(&game);
    }

    printf("Game Over.\n");

    return 0;
}
