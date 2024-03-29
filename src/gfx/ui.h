#ifndef _GFX_UI_H_
#define _GFX_UI_H_

#include "gfx/renderer.h"

#include <stdbool.h>
#include <stdint.h>

#define UI_MAX 32

struct Game;
typedef void(*game_f)(struct Game *);

typedef struct {
    uint32_t index;
    char title[STR_LEN_MAX];
    game_f function;
} UIComponent;

typedef struct {
    size_t size;
    int32_t pos;
    UIComponent items[UI_MAX]; 
    char title[STR_LEN_MAX];
} UI;

void ui_init(UI *);
void ui_add(UI *, const char *title, game_f);
void ui_set_title(UI *, const char *title);
int32_t ui_scroll(UI *, int32_t);
game_f ui_select(UI *);
bool ui_item_selected(const UI *, const UIComponent *);

#endif
