#include "game/buttons.h"
#include "game/game.h"
#include "gfx/window.h"

#include <SDL2/SDL_events.h>
#include <stdio.h>

void poll_events(Game *game) {
    SDL_Event event;
    while (SDL_PollEvent(&event)) {
        switch (event.type) {
            case SDL_QUIT:
                game_destroy(game);
                window_destroy();
                printf("%s terminated.\n", GAME_TITLE);
                exit(0);
                break;
            case SDL_KEYDOWN:
            case SDL_KEYUP:
                buttons_update(event);
                break;
            default:
                break;
        }
    }
}

int main(int argc, char *argv[]) {
    window_init(GAME_TITLE);

    Game game;
    game_init(&game);
    printf("%s initialized.\n", GAME_TITLE);

    while (game.state != G_QUIT) {
        renderer_prepare();
        game_update(&game);
        renderer_present();

        poll_events(&game);
    }

    game_destroy(&game);
    window_destroy();

    printf("Game Over.\n");
    return 0;
}
