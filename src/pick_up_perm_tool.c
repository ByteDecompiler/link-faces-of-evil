/* pick_up_perm_tool: Link touches a permanent item (tool spot index tool): keys (0-10), exits back to
   the world map (11-39; 11-22 also open a new place on the map), Water of Life (40-44), or the quest
   items (45-53), several of which then make an exit appear. Returns 1 to remove the item, 0 to keep it. */

#include "game.h"

char pick_up_perm_tool(unsigned char tool)
{
    switch (tool) {
    case 0: case 1: case 2: case 3: case 4: case 5: case 6: case 7: case 8: case 9: case 10:
        /* a key: experience $80-$8a */
        mark_experience(tool - (char)0x80);
        break;
    case 11: case 12: case 13: case 14: case 15: case 16: case 17: case 18: case 19: case 20:
    case 21: case 22:
        /* an exit that also opens a new place on the world map */
        open_up_area(tool, 1);
        mark_experience(tool + 0x80);
        demo_started = 1;
        return 0;
    case 23: case 24: case 25: case 26: case 27: case 28: case 29: case 30: case 31: case 32:
    case 33: case 34: case 35: case 36: case 37: case 38: case 39:
        /* an exit back to the world map */
        demo_started = 1;
        return 0;
    case 40: case 41: case 42: case 43: case 44:
        /* Water of Life: full life */
        get_water_of_life();
        return 0;
    case 45:                        /* Book of Koridai */
        item_counts[10] = 1;
        tell_what_got(10);
        mark_experience(0xac);
        tool_spots[tool].room = room_number;
        add_to_playfield_tools(0, 25, 0x44, 0x1b, 22, 0); /* the exit (tool spot 25) appears */
        break;
    case 46:                        /* Grapple Berries */
        item_counts[14] = 1;
        tell_what_got(14);
        break;
    case 47:                        /* Ice Crystal */
        item_counts[18] = 1;
        tell_what_got(18);
        break;
    case 48:                        /* Winged Helmet */
        item_counts[3] = 1;
        tell_what_got(3);
        mark_experience(0xa9);
        tool_spots[tool].room = room_number;
        add_to_playfield_tools(0, 0x13, 0x4b, 0x6b, 22, 0);
        break;
    case 49:                        /* Bell */
        item_counts[4] = 1;
        tell_what_got(4);
        mark_experience(0xa7);
        tool_spots[tool].room = room_number;
        add_to_playfield_tools(0, 13, 0x50, 0x27, 22, 0);
        break;
    case 50:                        /* Necklace */
        item_counts[13] = 1;
        tell_what_got(13);
        break;
    case 51:                        /* Crystal of Reflection */
        item_counts[16] = 1;
        tell_what_got(16);
        mark_experience(0xaa);
        tool_spots[tool].room = room_number;
        add_to_playfield_tools(0, 0x15, 0x40, 0x2d, 22, 0);
        break;
    case 52:                        /* Fire Diamond */
        item_counts[17] = 1;
        tell_what_got(17);
        mark_experience(0xa8);
        tool_spots[tool].room = room_number;
        add_to_playfield_tools(0, 0x11, 0x45, 0x8f, 22, 0);
        break;
    case 53:                        /* Crystal of Vision */
        item_counts[15] = 1;
        tell_what_got(15);
        mark_experience(0xab);
        tool_spots[tool].room = room_number;
        add_to_playfield_tools(0, 22, 0x39, 0x2c, 22, 0);
        break;
    }
    return 1;
}
