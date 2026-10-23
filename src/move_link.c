/* move_link: moves Link one frame from the stick (menu_event: 8 = up, 1 = up-right, 2 = right,
   ... clockwise): rope, big jump, thrust, jump, fall, ladder, slope slide, crouch and walk. */

#include "game.h"

void move_link(void)
{
    long i;

    ++jump_wait_timer;
    ++crouch_wait_timer;
    link_moved = 0;
    handle_knockback();
    if (rope_out)
        using_rope();
    else if (big_jump_active && jump_rising)
        big_jump_up();
    else if (big_jump_active && jump_falling)
        big_jump_down();
    else {
        ++link_idle_timer;
        if (link_idle_timer == 5) {    /* idle 5 frames: back to the standing/crouching pose */
            if (link_crouching) {
                go_find_hero_action(9);
                link_anim_step = 0;
            } else
                go_find_hero_action(0);
            anim_frame_idx = 0;
            cur_sprite = sprite_frames[hero_actions[hero_action].frames[anim_frame_idx]];
        } else if (link_thrusting) {
            link_idle_timer = 0;
            sword_thrust();
            link_moved = 1;
            link_anim_step = 0;
        } else if (jump_rising) {
            link_anim_step = 0;
            link_idle_timer = 0;
            fix_jump_stick((struct stick *)&menu_event);
            jump_up((struct motion *)&menu_event);
        } else if (jump_falling) {
            link_anim_step = 0;
            link_idle_timer = 0;
            fix_jump_stick((struct stick *)&menu_event);
            menu_event = ((struct motion *)&menu_event)->next;
            if (menu_event == 2 || menu_event == 1) {
                link_dir = 2;
                link_sprite_x = link_screen_x - 8;
                check_map_while_jump_descent(1);
            } else if (menu_event == 6 || menu_event == 7) {
                link_dir = 6;
                link_sprite_x = link_screen_x - 9;
                check_map_while_jump_descent(-1);
            } else
                check_map_while_jump_descent(0);
            if (jump_falling) {
                jump_step -= 1;
                if (jump_step == -1)
                    jump_falling = 0;
            }
            if (!jump_falling) {
                get_sprite_block(link_x, link_y, &jump_info);
                if (!jump_info_ground && !jump_info.left && !jump_info.right) {
                    go_find_hero_action(0);
                    anim_frame_idx = 0;
                    link_moved = 1;
                }
            }
        } else if (climbing_blk48) {    /* on a ladder (block 48) */
            climb(12, 13, (long)&jump_info.climb_a, (struct climb_dir *)&menu_event);
            link_idle_timer = 0;
        } else if (climbing_blk25) {    /* on a ladder (block 25) */
            climb(11, 14, (long)&jump_info.climb_b, (struct climb_dir *)&menu_event);
            link_idle_timer = 0;
        } else {
            get_sprite_block(link_x, link_y, &jump_info);
            if (!jump_info.floor && !jump_info_solid && !jump_info.deadly && !jump_info.climb_b && !jump_info.climb_a
                    && !jump_info.track) {
                ++fall_timer;
                if (fall_timer == 3) {    /* falling pose after 3 frames */
                    go_find_hero_action(4);
                    anim_frame_idx = 1;
                }
                for (i = 0; i <= 2; ++i) {    /* falls up to 3 cells per frame */
                    get_sprite_block(link_x, link_y, &jump_info);
                    if (!jump_info.floor && !jump_info_solid && !jump_info.deadly && !jump_info.climb_b && !jump_info.climb_a
                            && !jump_info.track) {
                        ++link_y;
                        ++link_screen_y;
                        ++link_sprite_y;
                    }
                }
                link_moved = 1;
                link_anim_step = 0;
                link_idle_timer = 0;
            } else if (old_get_sprite_block(link_x, link_y) == 0x56) {    /* slope: slides right */
                ++fall_timer;
                if (fall_timer == 3) {
                    go_find_hero_action(4);
                    anim_frame_idx = 1;
                }
                slide(1, (long)&jump_info.right);
                link_idle_timer = 0;
            } else if (old_get_sprite_block(link_x, link_y) == 0x25) {    /* slope: slides left */
                ++fall_timer;
                if (fall_timer == 3) {
                    go_find_hero_action(4);
                    anim_frame_idx = 1;
                }
                slide(-1, (long)&jump_info.left);
                link_idle_timer = 0;
            } else if (link_crouching) {
                if (menu_event == 3 || menu_event == 4 || menu_event == 5) {
                    link_crouching = 0;
                    check_which_downward_motion((struct motion *)&menu_event);
                    if (!link_crouching)
                        menu_event = 0;
                }
                if (menu_event == 2 || menu_event == 3) {
                    go_find_hero_action(9);
                    link_dir = 2;
                    link_sprite_x = link_screen_x - 8;
                    check_map_while_walking(1);
                } else if (menu_event == 6 || menu_event == 5) {
                    go_find_hero_action(9);
                    link_dir = 6;
                    link_sprite_x = link_screen_x - 9;
                    check_map_while_walking(-1);
                } else if (menu_event == 8 || menu_event == 1 || menu_event == 7) {
                    go_find_hero_action(0);
                    anim_frame_idx = 0;
                    jump_wait_timer = 0;
                    link_crouching = 0;
                    link_moved = 1;
                }
            } else {
                jump_falling = 0;
                fall_timer = 0;
                if (menu_event == 2) {
                    link_dir = 2;
                    link_sprite_x = link_screen_x - 8;
                    go_find_hero_action(2);
                    check_map_while_walking(1);
                } else if (menu_event == 6) {
                    link_dir = 6;
                    link_sprite_x = link_screen_x - 9;
                    go_find_hero_action(2);
                    check_map_while_walking(-1);
                } else if (menu_event == 8 || menu_event == 1 || menu_event == 7)
                    check_which_upward_motion((struct motion *)&menu_event);
                else if (menu_event == 4 || menu_event == 3 || menu_event == 5) {
                    check_which_downward_motion((struct motion *)&menu_event);
                    if (link_crouching) {
                        go_find_hero_action(9);
                        anim_frame_idx = 0;
                        link_moved = 1;
                        link_anim_step = 0;
                    }
                    link_idle_timer = 0;
                }
            }
        }
    }
    if (link_moved) {
        if (link_anim_step)
            ++anim_frame_idx;
        link_anim_step = 1;
        if (hero_actions[hero_action].frames[anim_frame_idx] == 999)
            anim_frame_idx = 0;
        cur_sprite = sprite_frames[hero_actions[hero_action].frames[anim_frame_idx]];
    }
    if (jump_info.track)
        block_ptrs[0]->track = old_get_sprite_block(sprite_pos[0][2], sprite_pos[0][3] - 1);
}
