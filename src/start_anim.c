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
            game_won = 13;    /* Goronu Dies */
    } else if (sprite_no == 1 && !sprite_ptrs[sprite_no]) {
        if (room == 7) {    /* boss of room 7: also heart container $99 */
            game_won = 13;    /* Goronu Dies */
            mark_experience(33);
            if (!check_experience(0x99)) {
                mark_experience(0x99);
                heart_bonus = 1;
                anim_restores_life = 1;
            }
        } else if (room == 28) {
            game_won = 26;    /* Harlequin Dies */
            mark_experience(34);
        } else if (room == 60)
            game_won = 26;    /* Harlequin Dies */
        else if (room == 40) {
            game_won = 34;    /* Militron's Defeat */
            mark_experience(38);
        } else if (room == 66)
            game_won = 34;    /* Militron's Defeat */
        else if (room == 54) {
            game_won = 44;    /* Glutko Dies */
            mark_experience(42);
            mark_experience(48);
        } else if (room == 56)
            game_won = 44;    /* Glutko Dies */
        else if (room == 61) {
            game_won = 48;    /* Lupay Dies */
            mark_experience(43);
        } else if (room == 69) {
            game_won = 39;    /* Ganon's Defeat */
            mark_experience(36);
        }
    } else if (room == 17 && (sprite_no == 1 || sprite_no == 2)) {    /* first visit: heart $97; later: refill life and canteen */
        if (!check_experience(0x97)) {
            mark_experience(0x97);
            heart_bonus = 1;
            anim_restores_life = 1;
            game_won = 19;    /* Fairies Life Heart */
        } else {
            get_water_of_life();
            anim_restores_life = 1;
            game_won = 20;    /* Fairies Water of Life */
        }
    } else if (sprite_no == 1) {
        if (room == 11) {
            if (kind == 6) {    /* hit by a Firestones shot (projectile type 6) */
                game_won = 3;    /* Hamish's Water of Life */
                if (have_water)
                    water_used = 1;
                anim_restores_life = 1;
                if (!anim_delay)
                    anim_delay = 2;
            } else
                game_won = 2;    /* Hamish Freezing */
        } else if (room == 13) {
            if (item_counts[10])    /* Book of Koridai */
                game_won = 5;    /* Aypo Reads the Book */
            else
                game_won = 4;    /* Aypo's Offer */
        } else if (room == 1) {
            if (sword_level2 || sword_level3)    /* room 1 gives sword level 2 once */
                game_won = 7;    /* Anutu Cheers Link On */
            else {
                game_won = 6;    /* Anutu's Power Sword */
                sword_level2 = 1;
                anim_restores_life = 1;
            }
        } else if (room == 3)
            game_won = 8;    /* Fish Lady's Secret */
        else if (room == 22) {
            if (item_counts[13]) {    /* Necklace traded for the Water of Life */
                game_won = 16;    /* Alora's Canteen */
                item_counts[13] = 0;
                have_water = 1;
            } else if (have_water)
                game_won = 1;    /* Alora's Missing Necklace (Short) */
            else
                game_won = 15;    /* Alora's Missing Necklace (Whole) */
        } else if (room == 24) {
            if (item_counts[0])    /* room 24 gives the Lantern */
                game_won = 18;    /* Horgam's Advice */
            else {
                game_won = 17;    /* Horgam's Lantern */
                item_counts[0] = 1;
            }
        } else if (room == 20) {
            if (item_counts[14]) {    /* Grapple Berries traded for the Power Glove (9) */
                item_counts[14] = 0;
                item_counts[9] = 1;
                game_won = 22;    /* Zorga's Power Glove */
            } else if (item_counts[9])
                game_won = 14;    /* Zorga's Magic (Short) */
            else
                game_won = 21;    /* Zorga's Magic (Whole) */
        } else if (room == 36) {
            if (item_counts[16]) {    /* Crystal of Reflection traded for the shield */
                game_won = 31;    /* Suprena and the Shield */
                item_counts[16] = 0;
                shield_level = 1;
                kill_sprite_noflash(1);
            } else
                game_won = 30;    /* Suprena's Challenge */
        } else if (room == 42) {
            if (check_experience(45))
                game_won = 36;    /* Clora (Short) */
            else
                game_won = 35;    /* Clora (Whole) */
            if (!check_experience(0x98)) {    /* room 42: heart container $98 once */
                mark_experience(0x98);
                heart_bonus = 1;
                anim_restores_life = 1;
            }
        } else if (room == 43)
            game_won = 37;    /* Kulvan In Chains */
        else if (room == 52)
            game_won = 42;    /* Droolik's Advice */
        else if (room == 30) {
            if (item_counts[18]) {    /* Ice Crystal traded for the Magic Lantern (1) */
                game_won = 28;    /* Odranoel's Lantern */
                item_counts[18] = 0;
                item_counts[1] = 1;
                kill_sprite_noflash(1);
            } else
                game_won = 27;    /* Odranoel's Experiment */
        } else if (room == 63) {
            game_won = 49;    /* Gwonam's Lantern */
            item_counts[15] = 0;    /* Crystal of Vision -> Lantern of Vision (2), unchecked */
            item_counts[2] = 1;
            kill_sprite_noflash(1);
        } else if (room == 4)
            game_won = 9;    /* Morshu's Store */
        else if (room == 6)
            game_won = 11;    /* Gwonam on Goronu */
        else if (room == 25)
            game_won = 23;    /* Gwonam on Harlequin */
        else if (room == 32)
            game_won = 29;    /* Gwonam on Zelda's Capture */
        else if (room == 37)
            game_won = 32;    /* Gwonam on Militron */
        else if (room == 53)
            game_won = 41;    /* Gwonam on Glutko */
        else if (room == 49) {
            game_won = 40;    /* Goronu's Revenge */
            kill_sprite_noflash(1);
            if (sprite_asleep[2]) {
                sprite_asleep[2] = 0;
                sprite_ptrs[2] = (char *)1;
            }
        } else if (room == 57)
            game_won = 45;    /* Gwonam on Lupay */
        else if (room == 70)
            game_won = 57;    /* Ending 1 */
    } else if (sprite_no == 2 && room == 43) {    /* Fire Diamond -> sword level 3 */
        game_won = 38;    /* Kulvan Tempers The Sword */
        sword_level3 = 1;
        item_counts[17] = 0;
        anim_restores_life = 1;
        kill_sprite_noflash(1);
    }
    if (game_won != -1 && kind == 25)    /* killed by a bomb: wait 10 frames */
        anim_delay = 10;
}
