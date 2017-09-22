#ifndef _FLEXGL_UI_
#define _FLEXGL_UI_

#include <stdint.h>

struct rgba {
    // These values must be normalized between 0.0 and 1.0
    float r;
    float g;
    float b;
    float a;
};

struct vertex {
    float x;
    float y;
};

uint32_t vertex_sizeof(void);

struct widget {
    struct flex_item *_flex_item;
    struct vertex tl;
    struct vertex tr;
    struct vertex br;
    struct vertex bl;

    // TODO: Don't store duplicate color data.
    // Consider instancing.
    struct rgba tl_rgba;
    struct rgba tr_rgba;
    struct rgba br_rgba;
    struct rgba bl_rgba;
};

struct widget* widget_new(float x, float y, float w, float h, struct rgba color);
void widget_free(struct widget *w);
float widget_get_width(struct widget *w);
float widget_get_height(struct widget *w);
float widget_get_pos_x(struct widget *w);
float widget_get_pos_y(struct widget *w);
uint32_t widget_sizeof(void);

#endif /* _FLEXGL_UI_ */
