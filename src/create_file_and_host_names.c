/* create_file_and_host_names: puts the save slot digit into the NVRAM file name ("/nvr/link<n>.nvri") and
   into the host name ("Faces of Evil <n>"). */

#include "game.h"

char file_name[16];
char host_name[16];

void create_file_and_host_names(long slot_number)
{
    file_name[9] = slot_number + '0';  /* "/nvr/link<n>.nvri" */
    host_name[14] = slot_number + '0';
}
