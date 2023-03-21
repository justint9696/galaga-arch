#include <stdio.h>

#include "app.h"

int main(int argc, char *argv[]) {
	initSDL();

	printf("%s Initialized.\n", APP_TITLE);

	SDL_Event event;
	while (1) {
		drawRect(0, 0, 15, 15, 0x00000000);

		while (SDL_PollEvent(&event)) {
			switch (event.type) {
			case SDL_QUIT:
				printf("%s Terminated.\n", APP_TITLE);
				exit(0);
				break;
			default:
				break;
			}
		}
	}

	return 0;
}
