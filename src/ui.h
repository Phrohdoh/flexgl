#ifndef _FLEXGL_UI_
#define _FLEXGL_UI_

struct widget;

struct widget* ui_widget_new(float x, float y, float w, float h);
void ui_widget_free(struct widget *w);

float ui_widget_get_width(struct widget *w);
float ui_widget_get_height(struct widget *w);
float ui_widget_get_pos_x(struct widget *w);
float ui_widget_get_pos_y(struct widget *w);

#endif /* _FLEXGL_UI_ */
