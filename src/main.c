#include "inc/app.h"
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
				switch (event.key.keysym.scancode) {
				case SDL_SCANCODE_UP:
					buttons |= BUTTON_UP;
					break;
				case SDL_SCANCODE_DOWN:
					buttons |= BUTTON_DOWN;
					break;
				case SDL_SCANCODE_LEFT:
					buttons |= BUTTON_LEFT;
					break;
				case SDL_SCANCODE_RIGHT:
					buttons |= BUTTON_RIGHT;
					break;
				case SDL_SCANCODE_SPACE:
					buttons |= BUTTON_SPACE;
					break;
				default:
					break;
				}
				break;

			case SDL_KEYUP:
				switch (event.key.keysym.scancode) {
				case SDL_SCANCODE_UP:
					buttons &= ~BUTTON_UP;
					break;
				case SDL_SCANCODE_DOWN:
					buttons &= ~BUTTON_DOWN;
					break;
				case SDL_SCANCODE_LEFT:
					buttons &= ~BUTTON_LEFT;
					break;
				case SDL_SCANCODE_RIGHT:
					buttons &= ~BUTTON_RIGHT;
					break;
				case SDL_SCANCODE_SPACE:
					buttons &= ~BUTTON_SPACE;
					break;
				default:
					break;
				}
				break;

			default:
				break;
			}
		}

		Game_Main(buttons);
	}

    printf("Game Over.\n");

	return 0;
}
