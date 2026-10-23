/* update_death_pose: advances Link's dying animation one frame; when it reaches its end, death_done is set. */

#include "game.h"

void update_death_pose(void)
{
    if (link_anim_step)
        anim_frame_idx += 1;
    link_anim_step = 1;
    if (link_thrusting && hero_actions[hero_action].frames[anim_frame_idx] == 999) {
        anim_frame_idx = 0;
        death_done = 1;
    }
    cur_sprite = sprite_frames[hero_actions[hero_action].frames[anim_frame_idx]];
}
