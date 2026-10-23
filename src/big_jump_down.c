/* big_jump_down: one frame of the falling half of a big jump (item 3): moves Link down and forward in his
   facing direction, steps jump_step down to -1, then ends the big jump; if Link has landed on a block
   (not empty space, not a left/right wall block) he goes back to the stand pose. */

#include "game.h"

extern char jump_falling;
extern struct block_info jump_info;
void check_map_while_jump_descent(long dir);

void big_jump_down(void)
{
    link_anim_step = 0;
    link_idle_timer = 0;
    link_dir == 2 ? check_map_while_jump_descent(1) : check_map_while_jump_descent(-1);
    if (jump_falling) {
        jump_step -= 1;
        if (jump_step == -1)
            jump_falling = 0;
    }
    if (!jump_falling) {
        big_jump_active = 0;
        get_sprite_block(link_x, link_y, &jump_info);
        if (!jump_info.ground && !jump_info.left && !jump_info.right) {
            go_find_hero_action(0);
            anim_frame_idx = 0;
            link_moved = 1;
        }
    }
}
