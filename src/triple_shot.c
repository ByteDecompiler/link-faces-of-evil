/* triple_shot: fires three projectiles in a fan (item 5): one straight ahead at 4 cells a
   frame and two at 3.5 cells a frame drifting half a cell up and down. A wall within 10 cells makes
   it fizzle. */

#include "game.h"

void triple_shot(long type, long shot_damage)
{
    long dy;
    long y;
    long wall_x;
    long x;

    dy = 0;
    if (link_crouching)
        dy = 2;
    else if (jump_rising)
        dy = -4;
    y = link_sprite_y + 8 + pf_y + dy;
    if (link_dir == 2) {
        x = link_sprite_x + 4 + pf_x;
        if (thrust_lunge)
            x += 4;
        wall_x = check_map_row(x, y, 11);
        if (wall_x < x + 10)
            init_miss(wall_x - pf_x, y - pf_y);
        else {
            init_projectile(type, wall_x - 3, y - 2, 28, 4, 0, 1, 0, 0, 0, shot_damage);
            init_projectile(type, wall_x - 3, y - 2, 32, 0, 0, 1, 0, 0, 0, shot_damage);
            init_projectile(type, wall_x - 3, y - 2, 28, -4, 0, 1, 0, 0, 0, shot_damage);
        }
    } else {
        x = link_sprite_x + 13 + pf_x;
        if (thrust_lunge)
            x -= 4;
        wall_x = check_map_row(x, y, -11);
        if (wall_x > x - 10)
            init_miss(wall_x - pf_x, y - pf_y);
        else {
            init_projectile(type, wall_x - 3, y - 2, -28, 4, 0, 1, 0, 0, 0, shot_damage);
            init_projectile(type, wall_x - 3, y - 2, -32, 0, 0, 1, 0, 0, 0, shot_damage);
            init_projectile(type, wall_x - 3, y - 2, -28, -4, 0, 1, 0, 0, 0, shot_damage);
        }
    }
}
