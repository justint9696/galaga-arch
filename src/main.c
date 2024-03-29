#include "common/util.h"
#include "game/buttons.h"
#include "game/game.h"
#include "gfx/window.h"

#include <SDL2/SDL_events.h>

static void poll_events(Game *game) {
    SDL_Event event;
    while (SDL_PollEvent(&event)) {
        switch (event.type) {
            case SDL_QUIT:
                game_quit(game);
                window_destroy();
                exit(0);
                break;
            case SDL_KEYDOWN:
            case SDL_KEYUP:
                buttons_update(event);
                break;
            default: break;
        }
    }
}

int main(int argc, char *argv[]) {
    window_init(GAME_TITLE);

    Game game;
    game_init(&game);
    LOG("%s initialized.\n", GAME_TITLE);

    while (game_is_running(&game)) {
        renderer_clear();
        game_update(&game);
        renderer_present();

        poll_events(&game);
    }

    game_destroy(&game);
    window_destroy();

    return 0;
}
