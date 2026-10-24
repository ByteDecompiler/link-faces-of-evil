/* start_boss_first_anim: when Link enters a boss room (boss_flag set), queues the boss's introduction
   cutscene for that room (game_won holds the pending animation number). */

#include "game.h"

void start_boss_first_anim(long room_no)
{
    if (boss_flag) {
        if (room_no == 7)
            game_won = 12;    /* Goronu Taunts Link */
        else if (room_no == 28)
            game_won = 25;    /* Harlequin Taunts Link */
        else if (room_no == 40)
            game_won = 33;    /* Militron Taunts Link */
        else if (room_no == 54)
            game_won = 43;    /* Glutko Attacks */
        else if (room_no == 60)
            game_won = 46;    /* Harlequin's Revenge */
        else if (room_no == 61)
            game_won = 47;    /* Lupay Taunts Link */
        else if (room_no == 66)
            game_won = 50;    /* Militron's Revenge */
        else if (room_no == 69)
            game_won = 24;    /* Ganon's Offer */
    }
}
