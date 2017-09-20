#include <stdio.h>
#include "../includes/flex.h"

// I don't want to type 'struct' over and over.
typedef struct flex_item flex_item;

void ui_do_thing(void) {
    flex_item *itm;
    itm = flex_item_new();
    printf("%p\n", itm);
    flex_item_free(itm);
}
