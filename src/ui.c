#include <stdlib.h>
#include <stdio.h>
#include "../includes/flex.h"

typedef struct flex_item flex_item;

typedef struct widget {
    flex_item *_flex_item;
} widget;

widget* ui_widget_new(float x, float y, float w, float h) {
    flex_item *itm;
    itm = flex_item_new();

    flex_item_set_left(itm, x);
    flex_item_set_top(itm, y);
    flex_item_set_width(itm, w);
    flex_item_set_height(itm, h);

    flex_layout(itm);

    widget *ret = malloc(sizeof(widget));
    ret->_flex_item = itm;

    return ret;
}

void ui_widget_free(widget *w) {
    flex_item_free(w->_flex_item);
    free(w);
}

float ui_widget_get_width(widget *w) {
    float ret = flex_item_get_width(w->_flex_item);
    return ret;
}

float ui_widget_get_height(widget *w) {
    float ret = flex_item_get_height(w->_flex_item);
    return ret;
}

float ui_widget_get_pos_x(widget *w) {
    float ret = flex_item_get_left(w->_flex_item);
    return ret;
}

float ui_widget_get_pos_y(widget *w) {
    float ret = flex_item_get_top(w->_flex_item);
    return ret;
}
