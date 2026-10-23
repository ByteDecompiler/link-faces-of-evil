/* start_boss_first_anim: when Link enters a boss room (boss_flag set), queues the boss's introduction
   cutscene for that room (game_won holds the pending animation number). */

#include "game.h"

void start_boss_first_anim(long room_no)
{
    if (boss_flag) {
        if (room_no == 7)
            game_won = 12;
        else if (room_no == 28)
            game_won = 25;
        else if (room_no == 40)
            game_won = 33;
        else if (room_no == 54)
            game_won = 43;
        else if (room_no == 60)
            game_won = 46;
        else if (room_no == 61)
            game_won = 47;
        else if (room_no == 66)
            game_won = 50;
        else if (room_no == 69)
            game_won = 24;
    }
}
