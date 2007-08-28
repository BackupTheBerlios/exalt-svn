/*
 * =====================================================================================
 *
 *       Filename:  exalt_daemon.c
 *
 *    Description:  defines globals funtions
 *
 *        Version:  1.0
 *        Created:  08/28/2007 04:27:45 PM CEST
 *       Revision:  none
 *       Compiler:  gcc
 *
 *         Author:   (Watchwolf), Atton Jonathan <watchwolf@watchwolf.fr>
 *        Company:
 *
 * =====================================================================================
 */

#include "exalt_daemon.h"

int main(int argc, char** argv)
{
    printf("INIT EXALT_DAEMON\n");
    e_dbus_init();
    ecore_init();

    exalt_eth_init();
    exalt_main();

    ecore_main_loop_begin();

    e_dbus_shutdown();
    ecore_shutdown();
    return 1;
}
