/* draw_other_items: draws the quest items Link has found (items 13-18) on the subscreen,
   in two rows of three. */

#include "game.h"

long item_icons[25];

void draw_other_items(long where)
{
    long i;
    long x;
    long y;

    x = SELECTED_ICON_X;
    y = 0xac;
    for (i = 13; i <= 18; ++i)
        if (item_counts[i] > 0) {
            if (i < 16)
                draw_rle_icon(where, (i - 13) * QUEST_ITEM_STEP * 13 / 6 + x, y, item_icons[i]);
            else
                draw_rle_icon(where, (i - 16) * QUEST_ITEM_STEP * 13 / 6 + x, y + 29, item_icons[i]);
        }
}
