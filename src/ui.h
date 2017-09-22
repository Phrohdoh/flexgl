#ifndef _FLEXGL_UI_
#define _FLEXGL_UI_

#include <stdint.h>

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
};

struct widget* widget_new(float x, float y, float w, float h);
void widget_free(struct widget *w);
float widget_get_width(struct widget *w);
float widget_get_height(struct widget *w);
float widget_get_pos_x(struct widget *w);
float widget_get_pos_y(struct widget *w);
uint32_t widget_sizeof(void);

#endif /* _FLEXGL_UI_ */
