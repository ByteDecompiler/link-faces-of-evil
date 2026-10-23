/* death_loop_bottom: clears the death screen's number box (pixels 200-264 x 88-144 of plane A, colour 1),
   saves Link's sprite position and erases the death count before it is redrawn with deaths. */

#include "game.h"

void death_loop_bottom(void)
{
    draw_filled_rect(DEATH_ERASE_X, 88, DEATH_ERASE_X_END, 144, plane_a_mem + death_y, 1);
    saved_link_y = link_sprite_y;
    saved_link_x = link_sprite_x;
    erase_death_number(death_y);
    deaths_shown = deaths;
}
