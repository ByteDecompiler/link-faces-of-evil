/* set_link_position_and_dir: moves Link dx cells sideways and faces him that way. */

#include "game.h"

void set_link_position_and_dir(long dx)
{
    link_x += dx;
    link_screen_x += dx;
    link_moved = 1;
    if (dx < 0) {
        link_dir = 6;
        link_sprite_x = link_screen_x - 9;
    } else {
        link_dir = 2;
        link_sprite_x = link_screen_x - 8;
    }
}
