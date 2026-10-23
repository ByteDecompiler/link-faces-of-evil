/* open_up_area: opens a place on the world map for map piece `area` (11-22): marks it open in areas[],
   and with show set, scrolls the world map to its label first (set_new_map). */

#include "game.h"

void open_up_area(long area, long show)
{
    unsigned char order[12];

    order[11] = 8;
    order[10] = 7;
    order[9] = 1;
    order[8] = 2;
    order[7] = 4;
    order[6] = 11;
    order[5] = 13;
    order[4] = 5;
    order[3] = 6;
    order[2] = 10;
    order[1] = 14;
    order[0] = 3;
    if (areas[order[area - 11]] != 1 && show)  /* map piece 11-22 -> place (areas[], labels[]) */
        set_new_map(labels[order[area - 11]].hot_x, labels[order[area - 11]].hot_y,
                    labels[order[area - 11]].hot_w, labels[order[area - 11]].hot_h);
    areas[order[area - 11]] = 1;
}
