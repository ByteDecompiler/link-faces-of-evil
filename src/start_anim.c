/* start_anim: picks the cutscene (game_won = animation number) for an enemy or character that was
   killed or hit (kind = projectile type, 99 = sword) in room, and applies its reward: heart
   containers, sword levels, shield, Water of Life, item trades, experience bits.
   An old-style definition, so that the char parameter is loaded into its register first
   (the prototype in game.h says long, which a char parameter promotes to) */

#include "game.h"

void start_anim(room, sprite_no, kind)
long room;
long sprite_no;
char kind;
{
    if (macros[sprite_no]->enemy_id != 1 && kind != 99    /* projectiles only count if the record's byte $50 >= 2 */
            && ((unsigned char *)(enemy_data + (sprite_no - 1) * 0xb0 + 0x50))[0] < 2)
        return;
    if (sprite_no == 2 && !sprite_ptrs[sprite_no]) {
        if (room == 49)    /* second character gone in room 49 */
            game_won = 13;
    } else if (sprite_no == 1 && !sprite_ptrs[sprite_no]) {
        if (room == 7) {    /* boss of room 7: also heart container $99 */
            game_won = 13;
            mark_experience(33);
            if (!check_experience(0x99)) {
                mark_experience(0x99);
                heart_bonus = 1;
                anim_restores_life = 1;
            }
        } else if (room == 28) {
            game_won = 26;
            mark_experience(34);
        } else if (room == 60)
            game_won = 26;
        else if (room == 40) {
            game_won = 34;
            mark_experience(38);
        } else if (room == 66)
            game_won = 34;
        else if (room == 54) {
            game_won = 44;
            mark_experience(42);
            mark_experience(48);
        } else if (room == 56)
            game_won = 44;
        else if (room == 61) {
            game_won = 48;
            mark_experience(43);
        } else if (room == 69) {
            game_won = 39;
            mark_experience(36);
        }
    } else if (room == 17 && (sprite_no == 1 || sprite_no == 2)) {    /* first visit: heart $97; later: refill life and canteen */
        if (!check_experience(0x97)) {
            mark_experience(0x97);
            heart_bonus = 1;
            anim_restores_life = 1;
            game_won = 19;
        } else {
            get_water_of_life();
            anim_restores_life = 1;
            game_won = 20;
        }
    } else if (sprite_no == 1) {
        if (room == 11) {
            if (kind == 6) {    /* hit by a Firestones shot (projectile type 6) */
                game_won = 3;
                if (have_water)
                    water_used = 1;
                anim_restores_life = 1;
                if (!anim_delay)
                    anim_delay = 2;
            } else
                game_won = 2;
        } else if (room == 13) {
            if (item_counts[10])    /* Book of Koridai */
                game_won = 5;
            else
                game_won = 4;
        } else if (room == 1) {
            if (sword_level2 || sword_level3)    /* room 1 gives sword level 2 once */
                game_won = 7;
            else {
                game_won = 6;
                sword_level2 = 1;
                anim_restores_life = 1;
            }
        } else if (room == 3)
            game_won = 8;
        else if (room == 22) {
            if (item_counts[13]) {    /* Necklace traded for the Water of Life */
                game_won = 16;
                item_counts[13] = 0;
                have_water = 1;
            } else if (have_water)
                game_won = 1;
            else
                game_won = 15;
        } else if (room == 24) {
            if (item_counts[0])    /* room 24 gives the Lantern */
                game_won = 18;
            else {
                game_won = 17;
                item_counts[0] = 1;
            }
        } else if (room == 20) {
            if (item_counts[14]) {    /* Grapple Berries traded for the Power Glove (9) */
                item_counts[14] = 0;
                item_counts[9] = 1;
                game_won = 22;
            } else if (item_counts[9])
                game_won = 14;
            else
                game_won = 21;
        } else if (room == 36) {
            if (item_counts[16]) {    /* Crystal of Reflection traded for the shield */
                game_won = 31;
                item_counts[16] = 0;
                shield_level = 1;
                kill_sprite_noflash(1);
            } else
                game_won = 30;
        } else if (room == 42) {
            if (check_experience(45))
                game_won = 36;
            else
                game_won = 35;
            if (!check_experience(0x98)) {    /* room 42: heart container $98 once */
                mark_experience(0x98);
                heart_bonus = 1;
                anim_restores_life = 1;
            }
        } else if (room == 43)
            game_won = 37;
        else if (room == 52)
            game_won = 42;
        else if (room == 30) {
            if (item_counts[18]) {    /* Ice Crystal traded for the Magic Lantern (1) */
                game_won = 28;
                item_counts[18] = 0;
                item_counts[1] = 1;
                kill_sprite_noflash(1);
            } else
                game_won = 27;
        } else if (room == 63) {
            game_won = 49;
            item_counts[15] = 0;    /* Crystal of Vision -> Lantern of Vision (2), unchecked */
            item_counts[2] = 1;
            kill_sprite_noflash(1);
        } else if (room == 4)
            game_won = 9;
        else if (room == 6)
            game_won = 11;
        else if (room == 25)
            game_won = 23;
        else if (room == 32)
            game_won = 29;
        else if (room == 37)
            game_won = 32;
        else if (room == 53)
            game_won = 41;
        else if (room == 49) {
            game_won = 40;
            kill_sprite_noflash(1);
            if (sprite_asleep[2]) {
                sprite_asleep[2] = 0;
                sprite_ptrs[2] = (char *)1;
            }
        } else if (room == 57)
            game_won = 45;
        else if (room == 70)
            game_won = 57;
    } else if (sprite_no == 2 && room == 43) {    /* Fire Diamond -> sword level 3 */
        game_won = 38;
        sword_level3 = 1;
        item_counts[17] = 0;
        anim_restores_life = 1;
        kill_sprite_noflash(1);
    }
    if (game_won != -1 && kind == 25)    /* killed by a bomb: wait 10 frames */
        anim_delay = 10;
}
