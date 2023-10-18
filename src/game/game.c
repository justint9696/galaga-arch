#include "gfx/hud.h"
#include "gfx/renderer.h"
#include "gfx/stars.h"

#include "common/util.h"

#include "entity/enemy.h"
#include "entity/player.h"

#include "entity/logic/formation.h"

#include "game/buttons.h"
#include "game/game.h"
#include "game/time.h"

#include <SDL2/SDL.h>

static inline void _Game_Tick(Game *self) {
    Hud_AddText("Level time: %lis", ((Time_Passed(self->fps.start) / 1000)));
}

bool Game_IsRunning(Game *self) {
    return (Player_IsAlive(&self->world.player));
}

void Game_Init(Game *self) {
    // prepare game
    memset(self, 0, sizeof(Game));

    // prepare time
    Time_Init();
    FPS_Init(&self->fps);

    // prepare entities
    World_Init(&self->world, self->fps.start);

    // prepare stage
    Stage_Init(&self->stage, 0);

    // prepare gfx
    Hud_Init();
    Stars_Init();
}

void Game_Main(Game *self) {
    // frame start
    Frame_Start(&self->fps);

    // prepare renderer
    Renderer_Prepare();

    // update gfx
    Stars_Update(self->fps.delta);

    // update stage
    Stage_Update(&self->stage, &self->world, self->fps.ticks);

    // update entities
    World_Update(&self->world, &self->buttons, self->fps.ticks, self->fps.delta);

    // toggle debug info
    if (self->buttons.current & BUTTON_F12 && Time_Passed(self->tick) > BUTTON_DELAY) {
        self->tick = self->fps.ticks;
        self->flags.debug ^= 1;
    }

    // draw hud
    Hud_Draw(self->flags.debug);

    // render screen
    Renderer_Update();

    // frame end
    Frame_End(&self->fps);

    // frame delay
    FPS_Limit(&self->fps);

    // draw fps
    Hud_DrawFPS(&self->fps);
}

void Game_Destroy(Game *self) {
    Hud_Destroy();
    World_Destroy(&self->world);
    Stage_Destroy(&self->stage);
    Formation_Destroy(&self->world.formation);
}
