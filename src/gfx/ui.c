#include "gfx/ui.h"

#include <assert.h>
#include <string.h>

void ui_init(UI *self) {
    memset(self, 0, sizeof(UI));
}

void ui_add(UI *self, const char *title, game_f function) {
    assert(self->size < UI_MAX);
    uint32_t index = self->size++;
    UI_Item *item = &self->items[index];

    size_t size = strnlen(title, STR_LEN_MAX);
    strncpy(item->title, title, size);

    item->function = function;
    item->index = index;
}

int32_t ui_scroll(UI *self, int32_t pos) {
    self->pos += pos;
    if (self->pos < 0) self->pos = self->size - 1;
    else if ((uint32_t)(self->pos) >= self->size) self->pos = 0;
    return self->pos;
}

game_f ui_select(UI *self) {
    return self->items[self->pos].function;
}

bool ui_item_selected(const UI *self, const UI_Item *item) {
    return ((uint32_t)(self->pos) == item->index);
}
