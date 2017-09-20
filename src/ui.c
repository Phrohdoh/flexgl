#include <stdio.h>
#include "../includes/flex.h"

void ui_do_thing(void) {
    struct flex_item *itm;
    itm = flex_item_new();
    printf("%p\n", itm);
    flex_item_free(itm);
}
