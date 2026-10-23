/* straight_shot: fires a projectile straight ahead from Link's sword (the sword beam):
   4 cells a frame. A wall right in front makes it fizzle instead. */

#include "game.h"

void straight_shot(long type, long shot_damage)
{
    long dy;
    long y;
    long x;

    dy = 0;
    if (link_crouching)
        dy = 2;
    else if (jump_rising)
        dy = -4;
    y = link_sprite_y + 8 + pf_y + dy;
    if (link_dir == 2) {
        x = check_map_row(link_sprite_x + 4 + pf_x, y, 11);
        if (x < link_sprite_x + 14 + pf_x)
            init_miss(x - pf_x, y - pf_y);
        else
            init_projectile(type, x - 3, y - 2, 32, 0, 0, 1, 0, 0, 0, shot_damage);
    } else {
        x = check_map_row(link_sprite_x + 13 + pf_x, y, -11);
        if (x > link_sprite_x + 3 + pf_x)
            init_miss(x - pf_x, y - pf_y);
        else
            init_projectile(type, x - 3, y - 2, -32, 0, 0, 1, 0, 0, 0, shot_damage);
    }
}
