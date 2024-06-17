
// #------------------------------#
// |          includes            |
// #------------------------------#
// c includes
#include <stdio.h>

// my includes
#include "server/ake.h"

// #------------------------------#
// |           macros             |
// #------------------------------#

// #------------------------------#
// | global variables definitions |
// #------------------------------#

// #------------------------------#
// | static variables definitions |
// #------------------------------#

// #------------------------------#
// | static functions declarations|
// #------------------------------#

// #------------------------------#
// | global function definitions  |
// #------------------------------#

ClientMsg* ake_handle_msg(const ServerMsg* msg_p)
{
    printf("session: %zu", msg_p->token);
    return NULL;
}

// #------------------------------#
// | static functions definitions |
// #------------------------------#
