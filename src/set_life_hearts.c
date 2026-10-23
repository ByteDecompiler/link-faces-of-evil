/* set_life_hearts: sets Link's heart containers: 3, plus one for each of the three heart experiences
   ($97-$99) found. */

#include "game.h"

void set_life_hearts(void)
{
    max_hearts = 3;
    if (check_experience(0x97))
        max_hearts += 1;
    if (check_experience(0x98))
        max_hearts += 1;
    if (check_experience(0x99))
        max_hearts += 1;
}
