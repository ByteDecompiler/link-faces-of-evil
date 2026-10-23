/* check_which_upward_motion: handles the stick pushed up: Link starts climbing if he stands at a
   climbable column (actions 12 / 11), otherwise he starts a jump, if there's no ceiling above and at
   least 10 frames have passed since the last jump. */

#include "game.h"

void check_which_upward_motion(struct motion *move_info)
{
    check_for_climb(-1);
    if (climbing_blk48 || climbing_blk25) {
        if (climbing_blk48)
            go_find_hero_action(12);  /* climb a block-48 column */
        if (climbing_blk25)
            go_find_hero_action(11);  /* climb a block-25 column */
        move_info->next = 0;
        old_link_dir = link_dir;
        link_dir = 2;
        link_sprite_x = link_screen_x - 8;
        anim_frame_idx = 0;
        --link_y;
        --link_screen_y;
        --link_sprite_y;
        link_moved = 1;
    } else {
        get_sprite_block(link_x, link_y - 1, &jump_info);
        if (!jump_info_solid) {
            if ((unsigned char)jump_wait_timer < 10)  /* last jump under 10 frames ago: no jump */
                jump_info_solid = 1;
            else {
                jump_wait_timer = 10;
                go_find_hero_action(4);  /* action 4: jump */
                anim_frame_idx = 0;
                link_moved = 1;
                link_anim_step = 0;
                jump_rising = 1;
                jump_step = 0;
                move_info->next = move_info->dir;
                move_info->prev = move_info->dir;
            }
        }
    }
}
