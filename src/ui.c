#include <stdlib.h>
#include <stdio.h>

#include "../includes/flex.h"
typedef struct flex_item flex_item;

struct vertex {
    float x;
    float y;
};

typedef struct vertex vertex;

struct rgba {
    float r;
    float g;
    float b;
    float a;
};

typedef struct rgba rgba;

struct widget {
    flex_item *_flex_item;
    vertex tl;
    vertex tr;
    vertex br;
    vertex bl;
    rgba tl_rgba;
    rgba tr_rgba;
    rgba br_rgba;
    rgba bl_rgba;
};

typedef struct widget widget;

widget* widget_new(float x, float y, float w, float h, rgba color) {
    widget *ret = malloc(sizeof(widget));
    if (ret == NULL) {
        return NULL;
    }

    flex_item *itm = flex_item_new();
    if (itm == NULL) {
        return NULL;
    }

    flex_item_set_left(itm, x);
    flex_item_set_top(itm, y);
    flex_item_set_width(itm, w);
    flex_item_set_height(itm, h);
    flex_layout(itm);

    ret->_flex_item = itm;
    ret->tl = (vertex){ x,     y     };
    ret->tr = (vertex){ x + w, y     };
    ret->br = (vertex){ x + w, y + h };
    ret->bl = (vertex){ x,     y + h };
    ret->tl_rgba = color;
    ret->tr_rgba = color;
    ret->br_rgba = color;
    ret->bl_rgba = color;

    return ret;
}

void widget_free(widget *w) {
    if (w != NULL) {
        flex_item_free(w->_flex_item);
        free(w);
    }
}

float widget_get_width(widget *w) {
    float ret = flex_item_get_width(w->_flex_item);
    return ret;
}

float widget_get_height(widget *w) {
    float ret = flex_item_get_height(w->_flex_item);
    return ret;
}

float widget_get_pos_x(widget *w) {
    float ret = flex_item_get_left(w->_flex_item);
    return ret;
}

float widget_get_pos_y(widget *w) {
    float ret = flex_item_get_top(w->_flex_item);
    return ret;
}

uint32_t widget_sizeof(void) {
    return sizeof(widget);
}

uint32_t vertex_sizeof(void) {
    return sizeof(vertex);
}
