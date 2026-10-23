/* init_type_6: sets up a type-6 sprite, a moving platform Link can ride, from its definition: start state
   (2 right, else left), pause time, then makes its block row solid (init_5_or_6). A definition with no
   kind leaves the sprite empty. */

#include "game.h"

void init_type_6(long sprite_idx, struct macro_def6 *def)
{
    register struct macro_def6 *def_ptr;

    def_ptr = def;
    if (!def_ptr->kind) {
        sprite_ptrs[sprite_idx] = 0;
        sprite_drawn[sprite_idx] = 0;
        sprite_asleep[sprite_idx] = 0;
        return;
    }
    macros[sprite_idx]->state = def_ptr->state;
    macros[sprite_idx]->lo = def_ptr->pause_time;
    macros[sprite_idx]->hi = 0;
    ((struct macro6 *)macros[sprite_idx])->moving = 0;
    if (macros[sprite_idx]->state == 2) {
        sprite_draw_xs[sprite_idx] = sprite_pos[sprite_idx][2] - 8;
        ((struct macro6 *)macros[sprite_idx])->frame_offset = 0;
    } else {
        sprite_draw_xs[sprite_idx] = sprite_pos[sprite_idx][2] - 9;
        ((struct macro6 *)macros[sprite_idx])->frame_offset = 3;
    }
    init_5_or_6(sprite_idx, def_ptr->kind, 2, 1, 6);  /* solid block 3 under the platform, 6 cells wide */
}
