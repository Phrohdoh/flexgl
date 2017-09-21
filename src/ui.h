#ifndef _FLEXGL_UI_
#define _FLEXGL_UI_

struct widget;
typedef struct widget widget;

widget* ui_widget_new(float x, float y, float w, float h);
void ui_widget_free(widget *w);

float ui_widget_get_width(widget *w);
float ui_widget_get_height(widget *w);
float ui_widget_get_pos_x(widget *w);
float ui_widget_get_pos_y(widget *w);

#endif /* _FLEXGL_UI_ */
