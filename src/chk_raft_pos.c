/* chk_raft_pos: on entering a room, moves the raft (type-6 platform) to the side Link
   came from: the arguments are the new room and the room just left. */

#include "game.h"

void chk_raft_pos(long area, char room)
{
    if (area == 15 && room == 16)
        place_raft(0x92, 0x33, 6);
    else if (area == 14 && room == 15)
        place_raft(0x98, 0x28, 6);
    else if (area == 31 && room == 32)
        place_raft(0xc4, 0x35, 6);
    else if (area == 64 && room == 65)
        place_raft(0xc4, 0x29, 6);
    else if (area == 65 && room == 66)
        place_raft(0xdd, 0x28, 2);
}
