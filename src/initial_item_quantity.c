/* initial_item_quantity: sets the item counts for a new game (initial_counts) and clears all experience
   bits. */

#include "game.h"

unsigned char initial_counts[27];

void initial_item_quantity(void)
{
    long i;

    for (i = 0; i <= 26; ++i)
        item_counts[i] = initial_counts[i];
    for (i = 0; i <= 32; ++i)
        experiences[i] = 0;
}
