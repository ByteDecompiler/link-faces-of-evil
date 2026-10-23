/* leave_item: drops the item an enemy leaves when it dies (enemy record +$48) at (x, y). Kinds 1-8 are
   temporary pickups (rubies, Snowballs, Firestones) that vanish after 80 frames; kinds 9-22 are
   permanent items, placed at their tool_spots entry with their experience bit marked. */

#include "game.h"

void leave_item(long kind, long x, long y)
{
    long id;
    long item;
    long temporary;

    id = -1;
    switch (kind) {
    case 1:  /* 1 ruby */
        item = 2;
        temporary = 1;
        break;
    case 2:  /* 5 rubies */
        item = 3;
        temporary = 1;
        break;
    case 3:  /* 10 rubies */
        item = 4;
        temporary = 1;
        break;
    case 4:  /* 1 or 5 rubies */
        item = randomize() % 2 ? 2 : 3;
        temporary = 1;
        break;
    case 5:  /* 5 or 10 rubies */
        item = randomize() % 2 ? 3 : 4;
        temporary = 1;
        break;
    case 6:  /* 10 rubies (1 in 5), else 1 or 5 */
        item = randomize() % 5 == 1 ? 4 : (randomize() % 2 ? 2 : 3);
        temporary = 1;
        break;
    case 7:  /* a Snowball */
        item = 11;
        temporary = 1;
        break;
    case 8:  /* a Firestone */
        item = 12;
        temporary = 1;
        break;
    case 9:  /* tool spot 48: Winged Helmet */
        id = 0x30;
        item = 14;
        temporary = 0;
        mark_experience(0x9a);
        break;
    case 10:  /* tool spot 49: Bell */
        id = 0x31;
        item = 13;
        temporary = 0;
        mark_experience(0x9b);
        break;
    case 11:  /* tool spot 50: Necklace */
        id = 0x32;
        item = 16;
        temporary = 0;
        mark_experience(0x9c);
        break;
    case 12:  /* tool spot 51: Crystal of Reflection */
        id = 0x33;
        item = 19;
        temporary = 0;
        mark_experience(0x9d);
        break;
    case 13:  /* tool spot 52: Fire Diamond */
        id = 0x34;
        item = 20;
        temporary = 0;
        mark_experience(0x9e);
        break;
    case 14:  /* tool spot 53: Crystal of Vision */
        id = 0x35;
        item = 21;
        temporary = 0;
        mark_experience(0x9f);
        break;
    case 15:  /* tool spot 45: Book of Koridai */
        id = 0x2d;
        item = 15;
        temporary = 0;
        mark_experience(0xa0);
        break;
    case 17:  /* tool spots 5-10: experiences $85-$8a when picked up (?) */
        id = 5;
        item = 5;
        temporary = 0;
        mark_experience(0xa1);
        break;
    case 18:
        id = 6;
        item = 5;
        temporary = 0;
        mark_experience(0xa2);
        break;
    case 19:
        id = 7;
        item = 5;
        temporary = 0;
        mark_experience(0xa3);
        break;
    case 20:
        id = 8;
        item = 5;
        temporary = 0;
        mark_experience(0xa4);
        break;
    case 21:
        id = 9;
        item = 5;
        temporary = 0;
        mark_experience(0xa5);
        break;
    case 22:
        id = 10;
        item = 5;
        temporary = 0;
        mark_experience(0xa6);
        x = 0x70;
        y = 21;
        break;
    default:
        item = 4;
        temporary = 1;
        break;
    case 16:  /* leaves item and temporary unset */
        break;
    }
    if (!temporary)
        tool_spots[id].room = room_number;
    add_to_playfield_tools(temporary, id == -1 ? item : id, x + 7, y + 7, item, temporary == 0 ? 0 : 0x50);  /* temporary drops vanish after $50 frames */
}
