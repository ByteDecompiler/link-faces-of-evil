/* add_fake_ropes: after ropes are bought in the shop, puts a placeholder rope tool (kind 26, state 126)
   at cell (77, 32) with a 16-frame timer; when it runs out update_tools calls add_ropes_to_buy. */

#include "game.h"

void add_fake_ropes(void)
{
    add_to_playfield_tools(1, 26, 77, 32, 26, 16);
}
