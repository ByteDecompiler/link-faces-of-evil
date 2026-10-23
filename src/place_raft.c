/* place_raft: moves the room's raft (the first type-6 sprite) to (x, y) facing state
   (2 right, 6 left): restores the map blocks under its old spot and marks its new spot as
   platform (block 3 (?)), 6 cells wide. */

#include "game.h"

void place_raft(long x, long y, long state)
{
    long i;

    for (i = 0; i < 21; ++i)
        if (i && sprite_ptrs[i] && macros[i]->kind == 6) {
            restore_sprite_blocks(sprite_pos[i][2] - 2, sprite_pos[i][3] - 1, 6, &((char *)path_blocks)[i * 18]);
            macros[i]->state = state;
            sprite_pos[i][2] = x;
            sprite_pos[i][3] = y;
            if (state == 2) {
                sprite_draw_xs[i] = sprite_pos[i][2] - 8;
                ((struct macro_b *)macros[i])->alt2_macro = 0;
            } else {
                sprite_draw_xs[i] = sprite_pos[i][2] - 9;
                ((struct macro_b *)macros[i])->alt2_macro = 3;
            }
            sprite_draw_ys[i] = sprite_pos[i][3] - 14;
            write_sprite_blocks(sprite_pos[i][2] - 2, sprite_pos[i][3] - 1, 6, &((char *)path_blocks)[i * 18], 3);
            break;
        }
}
