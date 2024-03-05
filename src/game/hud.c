#include "game/hud.h"
#include "game/buttons.h"
#include "game/game.h"

#include <assert.h>

#define CENTER_X(_width) ((SCREEN_WIDTH - _width) / 2.f)
#define CENTER_Y(_height) ((SCREEN_HEIGHT - _height) / 2.f)

static void prepare_idle_ui(Game *self) {
    ui_init(&self->ui);
    ui_set_title(&self->ui, GAME_TITLE);
    ui_add(&self->ui, "Start Game", game_resume);
    ui_add(&self->ui, "Quit Game", game_quit);
}

static void prepare_paused_ui(Game *self) {
    ui_init(&self->ui);
    ui_set_title(&self->ui, GAME_TITLE);
    ui_add(&self->ui, "Resume Game", game_resume);
    ui_add(&self->ui, "Restart Game", game_restart);
    ui_add(&self->ui, "Quit Game", game_quit);
}

static void prepare_game_over_ui(Game *self) {
    ui_init(&self->ui);
    ui_set_title(&self->ui, "Game Over");
    ui_add(&self->ui, "Play Again", game_restart);
    ui_add(&self->ui, "Quit Game", game_quit);
}

static void update_text(Game *self) {
    uint32_t width = font_width(self->ui.title, FONT_NORMAL);
    uint32_t height = font_height(FONT_NORMAL);

    draw_text(self->ui.title, VEC2(CENTER_X(width), HUD_Y + height + 5), COLOR_WHITE, FONT_NORMAL, DEPTH_UI);

    UI_Item *item;
    size_t size = self->ui.size;
    for (size_t i = 0; i < size; i++) {
        item = &self->ui.items[i]; 
        width = font_width(item->title, FONT_SMALL);
        if (ui_item_selected(&self->ui, item))
            draw_text(item->title, VEC2(CENTER_X(width), HUD_Y - (HUD_DIST * i)), COLOR_RED, FONT_SMALL, DEPTH_UI);
        else
            draw_text(item->title, VEC2(CENTER_X(width), HUD_Y - (HUD_DIST * i)), COLOR_WHITE, FONT_SMALL, DEPTH_UI);
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

void game_hud_update(Game *self) {
    switch (self->state) {
        case G_IDLE:
        case G_PAUSED:
        case G_DEAD:
            update_text(self);
            monitor_input(self);
            break;
        default: break;
    }
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
        default: break;
    }
}
