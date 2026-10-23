/* move_type_5_or_6_1_block: moves a riding platform (macro kind 6) one cell along its track (block 12) or
   marker blocks (block 20, the f1d flag), going up or down with it. Returns 0 if it moved, 1 if no track
   was found. */

#include "game.h"

long move_type_5_or_6_1_block(long sprite_idx, long dx)
{
    char *f1d_flag;
    char *f1d_flag2;

    if (macros[sprite_idx]->kind == 6) {
        f1d_flag = &block_ptrs[sprite_idx]->f1d;
        f1d_flag2 = &block_ptrs[sprite_idx]->f1d;
    }
    get_sprite_block(sprite_pos[sprite_idx][2] + dx, sprite_pos[sprite_idx][3] - 1, block_ptrs[sprite_idx]);
    if (block_ptrs[sprite_idx]->track) {
        sprite_pos[sprite_idx][2] += dx;
        (&link_sprite_x)[sprite_idx] += dx;
        --sprite_pos[sprite_idx][3];
        --(&link_sprite_y)[sprite_idx];
        return 0;
    }
    if (*f1d_flag || *f1d_flag2) {
        do {
            --sprite_pos[sprite_idx][3];
            --(&link_sprite_y)[sprite_idx];
            get_sprite_block(sprite_pos[sprite_idx][2] + dx, sprite_pos[sprite_idx][3] - 1, block_ptrs[sprite_idx]);
        } while (*f1d_flag || *f1d_flag2);
        sprite_pos[sprite_idx][2] += dx;
        (&link_sprite_x)[sprite_idx] += dx;
        return 0;
    }
    get_sprite_block(sprite_pos[sprite_idx][2] + dx, sprite_pos[sprite_idx][3], block_ptrs[sprite_idx]);
    if (block_ptrs[sprite_idx]->track || *f1d_flag || *f1d_flag2) {
        sprite_pos[sprite_idx][2] += dx;
        (&link_sprite_x)[sprite_idx] += dx;
        return 0;
    }
    get_sprite_block(sprite_pos[sprite_idx][2] + dx, sprite_pos[sprite_idx][3] + 1, block_ptrs[sprite_idx]);
    if (block_ptrs[sprite_idx]->track) {
        sprite_pos[sprite_idx][2] += dx;
        (&link_sprite_x)[sprite_idx] += dx;
        ++sprite_pos[sprite_idx][3];
        ++(&link_sprite_y)[sprite_idx];
        return 0;
    }
    if (*f1d_flag || *f1d_flag2) {
        do {
            ++sprite_pos[sprite_idx][3];
            ++(&link_sprite_y)[sprite_idx];
            get_sprite_block(sprite_pos[sprite_idx][2] + dx, sprite_pos[sprite_idx][3] + 1, block_ptrs[sprite_idx]);
        } while (*f1d_flag || *f1d_flag2);
        sprite_pos[sprite_idx][2] += dx;
        (&link_sprite_x)[sprite_idx] += dx;
        return 0;
    }
    get_sprite_block(sprite_pos[sprite_idx][2], sprite_pos[sprite_idx][3] + 1, block_ptrs[sprite_idx]);
    if (block_ptrs[sprite_idx]->track) {
        ++sprite_pos[sprite_idx][3];
        ++(&link_sprite_y)[sprite_idx];
        return 0;
    }
    if (*f1d_flag || *f1d_flag2) {
        do {
            ++sprite_pos[sprite_idx][3];
            ++(&link_sprite_y)[sprite_idx];
            get_sprite_block(sprite_pos[sprite_idx][2], sprite_pos[sprite_idx][3] + 1, block_ptrs[sprite_idx]);
        } while (*f1d_flag || *f1d_flag2);
        return 0;
    }
    return 1;
}
