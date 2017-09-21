#include <stdio.h>
#include "../includes/flex.h"

// I don't want to type 'struct' over and over.
typedef struct flex_item flex_item;

void ui_create_square(float size) {
    flex_item *itm;
    itm = flex_item_new();

    flex_item_set_width(itm, size);
    flex_item_set_height(itm, size);

    float w = flex_item_get_width(itm);
    float h = flex_item_get_height(itm);

    flex_layout(itm);

    printf("%0.2f x %0.2f\n", w, h);

    flex_item_free(itm);
}
