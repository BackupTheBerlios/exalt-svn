/*
 * =====================================================================================
 *
 *       Filename:  main.c
 *
 *    Description:  test the exalt daemon
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

#include "main.h"

int main(int argc, char** argv)
{
    Ecore_List *interfaces;
    char* interface;

    exalt_dbus_conn* conn;
    exalt_dbus_init();
    conn = exalt_dbus_connect();
    if(!conn)
    {
        fprintf(stderr,"main(): not connected!\n");
        return -1;
    }

    interfaces = exalt_dbus_eth_get_list(conn);
    if(!interfaces)
        return -1;

    printf("INTERFACES LIST: \n\n");
    ecore_list_first_goto(interfaces);
    while( (interface = ecore_list_next(interfaces)))
    {
        printf("name     : %s\n",interface);
        printf("wireless?: %d\n",exalt_dbus_eth_is_wireless(conn,interface));
        printf("link?    : %d\n",exalt_dbus_eth_is_link(conn,interface));
        printf("ip       : %s\n",exalt_dbus_eth_get_ip(conn,interface));
        printf("mask     : %s\n",exalt_dbus_eth_get_netmask(conn,interface));
        printf("gateway  : %s\n",exalt_dbus_eth_get_gateway(conn,interface));
        printf("\n");
    }

    //ecore_main_loop_begin();
    ecore_list_destroy(interfaces);
    exalt_dbus_shutdown();
    return 1;
}
