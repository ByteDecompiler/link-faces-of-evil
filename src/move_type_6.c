/* move_type_6: moves a moving platform one block along its track per frame, carrying Link if he
   rides it. At the end of the track it turns round and waits (lo frames); a platform with no
   wait left starts again once Link stands on it. */

#include "game.h"

short riding_box[4];
long sprite_macros[21];
long check_riding_box(short *box, long riding, long sprite_no);
long move_type_5_or_6_1_block(long sprite_no, long dx);
extern long was_riding;            /* -$1b16 */

void move_type_6(long sprite_no)
{
    long moved;
    long old_x;
    long riding;
    long old_y;
    char update_riding;
    unsigned short code;

    update_riding = 1;
    if (rope_out)
        riding = 0;
    else
        riding = check_riding_box(riding_box, 1, sprite_no);
    if (riding)
        link_riding = 1;
    if (((struct macro_b *)macros[sprite_no])->frame_offset) {
        restore_sprite_blocks(sprite_pos[sprite_no][2] - 2, sprite_pos[sprite_no][3] - 1, 6, &((char *)path_blocks)[sprite_no * 18]);
        old_x = sprite_pos[sprite_no][2];
        old_y = sprite_pos[sprite_no][3];
        if (macros[sprite_no]->state == 6)
            moved = move_type_5_or_6_1_block(sprite_no, -1);
        else
            moved = move_type_5_or_6_1_block(sprite_no, 1);
        if (riding) {
            if (block_ptrs[sprite_no]->track)
                jump_info.track = old_get_sprite_block(sprite_pos[sprite_no][2], sprite_pos[sprite_no][3] - 1);
            link_y += sprite_pos[sprite_no][3] - old_y;
            link_screen_y += sprite_pos[sprite_no][3] - old_y;
            link_sprite_y += sprite_pos[sprite_no][3] - old_y;
            link_x += sprite_pos[sprite_no][2] - old_x;
            if (moved) {
                if (macros[sprite_no]->state == 2) {
                    if (sprite_pos[sprite_no][2] - 2 != link_x)
                        --link_x;
                } else if (sprite_pos[sprite_no][2] + 3 != link_x)
                    ++link_x;
            }
            if (!was_riding) {
                if (old_get_sprite_block(link_x, link_y) != 1)
                    link_dir = macros[sprite_no]->state;
                else
                    update_riding = 0;
            }
            link_screen_x = link_x - pf_x;
            link_sprite_x = link_screen_x - (link_dir == 6 ? 9 : 8);
            link_moved = 1;
        }
        if (moved) {
            ((struct macro_b *)macros[sprite_no])->frame_offset = 0;
            macros[sprite_no]->hi = macros[sprite_no]->lo;
            if (macros[sprite_no]->state == 2) {
                (&link_sprite_x)[sprite_no] = sprite_pos[sprite_no][2] - 9;
                macros[sprite_no]->state = 6;
            } else {
                (&link_sprite_x)[sprite_no] = sprite_pos[sprite_no][2] - 8;
                macros[sprite_no]->state = 2;
            }
        }
        write_sprite_blocks(sprite_pos[sprite_no][2] - 2, sprite_pos[sprite_no][3] - 1, 6, &((char *)path_blocks)[sprite_no * 18], 3);
    } else if (macros[sprite_no]->hi) {
        if (--macros[sprite_no]->hi == 0) {
            ((struct macro_b *)macros[sprite_no])->frame_offset = 1;
            was_riding = 0;
            update_riding = 0;
        }
    } else if (riding)
        ((struct macro_b *)macros[sprite_no])->frame_offset = 1;
    ++(&anim_frame_idx)[sprite_no];
    while ((code = *(unsigned short *)(sprite_macros[sprite_no] + (&anim_frame_idx)[sprite_no] * 2)) == 0xffff)
        (&anim_frame_idx)[sprite_no] = 0;
    get_new_macro_image(sprite_no, ((struct macro_b *)macros[sprite_no])->alt2_macro, code);
    if (update_riding)
        was_riding = riding;
}
