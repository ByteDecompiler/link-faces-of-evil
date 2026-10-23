/* init_big_jump: starts a big jump with the Winged Helmet (item 3): Link's action 4. */

#include "game.h"

void init_big_jump(void)
{
    go_find_hero_action(4);
    anim_frame_idx = 0;
    cur_sprite = sprite_frames[hero_actions[hero_action].frames[anim_frame_idx]];
    link_moved = 1;
    link_anim_step = 0;
    jump_rising = 1;
    jump_step = 0;
    link_thrusting = 0;
    big_jump_active = 1;
    link_idle_timer = 0;
}
