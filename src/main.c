#include "gfx/app.h"

#include "game/buttons.h"
#include "game/game.h"

#include <SDL2/SDL_events.h>

void Poll_Events(Game *game) {
    SDL_Event event;
    while (SDL_PollEvent(&event)) {
        switch (event.type) {
            case SDL_QUIT:
                printf("%s Terminated.\n", GAME_TITLE);
                Game_Destroy(game);
                exit(0);
                break;
            case SDL_KEYDOWN:
            case SDL_KEYUP:
                Buttons_Update(&game->buttons, event);
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
        Poll_Events(&game);
        Game_Main(&game);
    }

    Game_Destroy(&game);
    printf("Game Over.\n");

    return 0;
}
