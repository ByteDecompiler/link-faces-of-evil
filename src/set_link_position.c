/* set_link_position: moves Link dx cells sideways. */

#include "game.h"

void set_link_position(long dx)
{
    link_x += dx;
    link_screen_x += dx;
    link_sprite_x += dx;
    link_moved = 1;
}
