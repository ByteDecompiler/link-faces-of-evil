/* add_ropes_to_buy: puts the ropes for sale on the shop floor (room 4): a tool of kind 8 at (77, 32) cells.
   Touching it buys 3 ropes for 10 rubies (sprite_hits_tool, buy_item). */

#include "game.h"

void add_ropes_to_buy(void)
{
    add_to_playfield_tools(1, 8, 77, 32, 8, 0);
}
