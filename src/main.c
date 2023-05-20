#include "inc/app.h"
#include "inc/buttons.h"
#include "inc/game.h"
#include "inc/player.h"

#include <SDL2/SDL_events.h>
#include <SDL2/SDL_render.h>
#include <SDL2/SDL_scancode.h>

int main(int argc, char *argv[]) {
	int buttons = 0;

	initSDL();

	Game_Init();
	printf("%s Initialized.\n", APP_TITLE);

	SDL_Event event;
	while (Game_IsRunning()) {
		while (SDL_PollEvent(&event)) {
			switch (event.type) {
			case SDL_QUIT:
				printf("%s Terminated.\n", APP_TITLE);
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

		Game_Main();
	}

	printf("Game Over.\n");

	return 0;
}
