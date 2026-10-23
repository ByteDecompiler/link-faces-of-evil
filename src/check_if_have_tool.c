/* check_if_have_tool: whether the tool spot tool_id should NOT be placed in its room: it was
   already picked up (experience $80+), it has not been made available yet (experiences $a1-$ac
   not set), or Link already holds (or has traded in) the item it gives. */

#include "game.h"

long check_if_have_tool(long tool_id)
{
    return tool_id == 0 && check_experience(0x80) || tool_id == 1 && check_experience(0x81)    /* spots 0-10: picked up ($80-$8a) */
        || tool_id == 2 && check_experience(0x82) || tool_id == 3 && check_experience(0x83)
        || tool_id == 4 && check_experience(0x84)
        || tool_id == 5 && (check_experience(0x85) || !check_experience(0xa1))
        || tool_id == 6 && (check_experience(0x86) || !check_experience(0xa2))
        || tool_id == 7 && (check_experience(0x87) || !check_experience(0xa3))
        || tool_id == 8 && (check_experience(0x88) || !check_experience(0xa4))
        || tool_id == 9 && (check_experience(0x89) || !check_experience(0xa5))
        || tool_id == 10 && (check_experience(0x8a) || !check_experience(0xa6))
        || tool_id == 13 && !check_experience(0xa7) || tool_id == 17 && !check_experience(0xa8)
        || tool_id == 19 && !check_experience(0xa9) || tool_id == 21 && !check_experience(0xaa)
        || tool_id == 22 && !check_experience(0xab) || tool_id == 25 && !check_experience(0xac)
        || tool_id == 48 && (item_counts[3] || !check_experience(0x9a))
        || tool_id == 49 && (item_counts[4] || !check_experience(0x9b))
        || tool_id == 50 && (item_counts[13] || have_water || !check_experience(0x9c))    /* Necklace */
        || tool_id == 51 && (item_counts[16] || shield_level || !check_experience(0x9d))    /* Crystal of Reflection */
        || tool_id == 52 && (item_counts[17] || sword_level3 || !check_experience(0x9e))    /* Fire Diamond */
        || tool_id == 53 && (item_counts[15] || item_counts[2] || !check_experience(0x9f))    /* Crystal of Vision */
        || tool_id == 45 && (item_counts[10] || !check_experience(0xa0))    /* Book of Koridai */
        || tool_id == 47 && (item_counts[18] || item_counts[1])    /* Ice Crystal */
        || tool_id == 46 && (item_counts[14] || item_counts[9]);    /* Grapple Berries */
}
