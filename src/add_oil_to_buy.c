/* add_oil_to_buy: puts the lamp oil (tool kind 7) up for sale in the shop room (room 4) at cell
   (47, 32); buying it costs 5 rubies (sprite_hits_tool). */

#include "game.h"

void add_oil_to_buy(void)
{
    add_to_playfield_tools(1, 7, 47, 32, 7, 0);    /* for sale (kind 1), tool kind 7 = lamp oil */
}
