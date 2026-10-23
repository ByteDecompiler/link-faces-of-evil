/* init_enemies: sets up the room's enemies (sprites 1-20) from their records: skips
   one-time enemies already beaten, copies speed, lives and sleep settings, links their graphics
   frames, and runs their type's init routine. Then fixes two rooms up. */

#include "game.h"

void init_enemies(void)
{
    long base;
    long id;
    register struct enemy *enemy_rec;
    register long j;
    register long k;
    long i;
    struct enemy_gfx *gfx_hdr;
    unsigned short color_word;

    enemy_rec = (struct enemy *)enemy_data;
    for (i = 1; i < 21; ++i) {
        if (enemy_rec->present) {
            if ((enemy_rec->kind == 1 || enemy_rec->kind == 2) && check_experience(enemy_rec->experience)) {  /* one-time enemy already beaten */
                sprite_drawn[i] = 0;
                sprite_ptrs[i] = 0;
                enemy_rec = (struct enemy *)((char *)enemy_rec + 0xb0L);
                continue;
            }
            macros[i]->kind = enemy_rec->type;
            id = enemy_rec->id;
            macros[i]->enemy_id = id;
            macros[i]->layer = enemy_rec->layer;
            macros[i]->step_delay = enemy_rec->step_delay;
            if ((unsigned char)macros[i]->step_delay < 1)
                macros[i]->step_delay = 1;
            if ((unsigned char)macros[i]->step_delay > 8)
                macros[i]->step_delay = 8;
            macros[i]->alt_step_delay = 1;
            macros[i]->sleep_timer = 0;
            macros[i]->wake_pending = 0;
            macros[i]->lives = enemy_rec->lives;
            macros[i]->respawned = 0;
            macros[i]->in_wake_box = 0;
            if (!enemy_rec->first_wake_mode && enemy_rec->first_sleep > 0 || enemy_rec->first_wake_mode == 2) {
                macros[i]->sleep_timer = enemy_rec->first_sleep;
                sprite_asleep[i] = 1;
            } else if (enemy_rec->first_wake_mode == 1)
                sprite_asleep[i] = 1;
            else {
                sprite_ptrs[i] = (char *)1;
                sprite_drawn[i] = 0;
            }
            for (k = 0; k < 4; ++k)
                if (enemy_types[k].id == id)
                    break;
            sprite_gfx[i] = k + 1;
            gfx_hdr = enemy_types[k].gfx;
            base = (long)gfx_hdr + 0x9c;
            for (j = 0; j < 24; ++j) {
                sprite_frames[i * 24 + j] = base + gfx_hdr->frames[j];
                color_word = gfx_hdr->colors[j];
                ((char *)frame_color_hi)[i * 24 + j] = color_word >> 8;
                ((char *)frame_color_lo)[i * 24 + j] = color_word & 0xff;
            }
            main_init(i, (struct macro_def *)enemy_rec);
            sprite_drawn_frame[i] = sprite_frame_no[i];
            (&saved_link_x)[i] = (&link_sprite_x)[i] - pf_x;
            (&saved_link_y)[i] = (&link_sprite_y)[i] - pf_y;
        }
        enemy_rec = (struct enemy *)((char *)enemy_rec + 0xb0L);
    }
    if (music_paused && room_number == 5) {  /* help demo in room 5: remove sprites 1, 2 and 4 */
        sprite_drawn[1] = 0;
        sprite_asleep[1] = 0;
        sprite_ptrs[1] = 0;
        sprite_drawn[4] = 0;
        sprite_asleep[4] = 0;
        sprite_ptrs[4] = 0;
        sprite_drawn[2] = 0;
        sprite_asleep[2] = 0;
        sprite_ptrs[2] = 0;
    }
    if (room_number == 49 && check_experience(40) && sprite_asleep[2]) {  /* room 49 after experience 40: sprite 2 starts awake */
        sprite_asleep[2] = 0;
        sprite_ptrs[2] = (char *)1;
    }
}
