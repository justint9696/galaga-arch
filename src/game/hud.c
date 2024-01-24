#include "game/hud.h"
#include "game/buttons.h"
#include "game/game.h"

#include <assert.h>

#define CENTER_X(_width) ((SCREEN_WIDTH - _width) / 2.f)
#define CENTER_Y(_height) ((SCREEN_HEIGHT - _height) / 2.f)

static void prepare_idle_ui(Game *self) {
    ui_init(&self->ui);
    ui_add(&self->ui, "Start Game", game_resume);
    ui_add(&self->ui, "Quit Game", game_quit);
}

static void prepare_paused_ui(Game *self) {
    ui_init(&self->ui);
    ui_add(&self->ui, "Resume Game", game_resume);
    ui_add(&self->ui, "Restart Game", game_restart);
    ui_add(&self->ui, "Quit Game", game_quit);
}

static void prepare_game_over_ui(Game *self) {
    ui_init(&self->ui);
    ui_add(&self->ui, "Play Again", game_restart);
    ui_add(&self->ui, "Quit Game", game_quit);
}

static void update_text(Game *self) {
    uint32_t width;
    uint32_t height = font_height(FONT_NORMAL);

    switch (self->state) {
        case G_DEAD:
            width = font_width("Game Over", FONT_NORMAL);
            hud_add_text("Game Over", CENTER_X(width), HUD_Y + height + 5, COLOR_WHITE, FONT_NORMAL);
            break;
        default: 
            width = font_width(GAME_TITLE, FONT_NORMAL);
            hud_add_text(GAME_TITLE, CENTER_X(width), HUD_Y + height + 5, COLOR_WHITE, FONT_NORMAL);
            break;
    }

    UI_Item *item;
    size_t size = self->ui.size;
    for (size_t i = 0; i < size; i++) {
        item = &self->ui.items[i]; 
        width = font_width(item->title, FONT_SMALL);
        if (ui_item_selected(&self->ui, item))
            hud_add_text(item->title, CENTER_X(width), HUD_Y - (HUD_DIST * i), COLOR_RED, FONT_SMALL);
        else
            hud_add_text(item->title, CENTER_X(width), HUD_Y - (HUD_DIST * i), COLOR_WHITE, FONT_SMALL);
    }
}

static void monitor_input(Game *self) {
    if (button_pressed(SDL_SCANCODE_UP, true)) {
        ui_scroll(&self->ui, -1);
    }
    if (button_pressed(SDL_SCANCODE_DOWN, true)) {
        ui_scroll(&self->ui, 1);
    }
    if (button_pressed(SDL_SCANCODE_RETURN, true)) {
        game_f function = ui_select(&self->ui);
        assert(function);
        function(self);
    }
}

void game_hud_init(Game *self) {
    hud_init();
}

void game_hud_update(Game *self) {
    switch (self->state) {
        case G_IDLE:
        case G_PAUSED:
        case G_DEAD:
            update_text(self);
            monitor_input(self);
            break;
        default:
            break;
    }

    hud_update();
}

void game_hud_reset(Game *self) {
    switch (self->state) {
        case G_IDLE:
            prepare_idle_ui(self);
            break;
        case G_PAUSED:
            prepare_paused_ui(self);
            break;
        case G_DEAD:
            prepare_game_over_ui(self);
            break;
        default:
            break;
    }
}
