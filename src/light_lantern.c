/* light_lantern: lights a dark room: the light is largest with the Lantern of Vision (item 2), half that
   with the Magic Lantern (item 1), a quarter with the plain lantern. */

#include "game.h"

void light_lantern(void)
{
    if (item_counts[2]) {             /* Lantern of Vision */
        lantern_size = 0x500;
        lantern_pos = 0x520 - lantern_pos;
    } else if (item_counts[1]) {      /* Magic Lantern */
        lantern_size = 0x280;
        lantern_pos = 0x2a0 - lantern_pos;
    } else {
        lantern_size = 0x140;
        lantern_pos = 0x160 - lantern_pos;
    }
}
