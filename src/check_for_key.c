/* check_for_key: may Link go through this locked door? door is the door's block code (79, 84 or 94),
   room the room number (see can_open_door). Each locked door needs its key, experience $80-$8a (picked
   up from tool spots 0-10); returns 0 if the key is missing, else 1. */

#include "game.h"

unsigned char check_for_key(long door, long room)
{
    if (room == 11 && door == 79 && !check_experience(0x80))
        return 0;
    if (room == 0 && door == 84 && !check_experience(0x81))
        return 0;
    if (room == 21 && door == 94 && !check_experience(0x82))
        return 0;
    if (room == 18 && door == 84 && !check_experience(0x83))
        return 0;
    if (room == 41 && door == 94 && !check_experience(0x84))
        return 0;
    if (room == 28 && door == 79 && !check_experience(0x85))
        return 0;
    if (room == 29 && door == 94 && !check_experience(0x85))
        return 0;
    if (room == 46 && door == 79 && !check_experience(0x86))
        return 0;
    if (room == 51 && door == 84 && !check_experience(0x87))
        return 0;
    if (room == 59 && door == 94 && !check_experience(0x88))
        return 0;
    if (room == 69 && door == 94 && !check_experience(0x89))
        return 0;
    return !(room == 0 && door == 79 && !check_experience(0x8a));
}
