/* using_rope: one frame of the rope: 1 = thrown up (3 cells a frame, to the ceiling or screen top),
   2 = it has caught (Link starts climbing), 4 = climbing (1 cell a frame),
   5 = climbing onto the top (3 a frame), 3 = the rope comes back down, 6 = done. */

#include "game.h"

void using_rope(void)
{
    long step;

    switch ((unsigned char)rope_out) {
    case 0:
        break;
    case 1:
        rope_top -= 3;
        if (pf_y + 1 >= rope_top) {
            rope_top = pf_y + 1;
            rope_out = 3;
        }
        if (check_for_rope_path(rope_x, rope_top + 2))
            rope_top += 2;
        else if (check_for_rope_path(rope_x, rope_top + 1))
            rope_top += 1;
        else
            check_for_rope_path(rope_x, rope_top);
        break;
    case 2:
        --rope_count;
        go_find_hero_action(11);
        anim_frame_idx = 0;
        rope_out = 4;
        link_moved = 1;
        break;
    case 3:
        if (rope_top >= rope_bottom) {
            rope_out = 6;
            link_dir = old_link_dir;
            go_find_hero_action(0);
            anim_frame_idx = 0;
            cur_sprite = sprite_frames[hero_actions[hero_action].frames[anim_frame_idx]];
        } else {
            rope_top += 3;
            if (rope_top >= rope_bottom) {
                rope_top = rope_bottom;
                rope_out = 6;
                link_dir = old_link_dir;
                go_find_hero_action(0);
                anim_frame_idx = 0;
                cur_sprite = sprite_frames[hero_actions[hero_action].frames[anim_frame_idx]];
            }
        }
        break;
    case 4:
        if (link_y - 11 <= rope_top) {
            link_y -= 3;
            link_screen_y -= 3;
            link_sprite_y -= 3;
            rope_bottom -= 3;
            link_moved = 1;
            rope_out = 5;
            go_find_hero_action(14);
            anim_frame_idx = 0;
        } else {
            link_y -= 1;
            link_screen_y -= 1;
            link_sprite_y -= 1;
            rope_bottom -= 1;
            if (rope_anim_toggle)
                link_anim_step = 1;
            else
                link_anim_step = 0;
            rope_anim_toggle = 1 - rope_anim_toggle;
        }
        link_moved = 1;
        break;
    case 5:
        if (link_y <= rope_top) {
            rope_out = 6;
            link_dir = old_link_dir;
            go_find_hero_action(0);
            anim_frame_idx = 0;
        } else if (link_y - rope_top < 3) {
            step = link_y - rope_top;
            link_y -= step;
            link_screen_y -= step;
            link_sprite_y -= step;
            rope_bottom -= step;
        } else {
            link_y -= 3;
            link_screen_y -= 3;
            link_sprite_y -= 3;
            rope_bottom -= 3;
        }
        link_moved = 1;
        break;
    case 6:
        rope_out = 0;
        break;
    }
}
