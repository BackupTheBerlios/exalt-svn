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

void notify_cb(char* interface, int action, void* user_data)
{
    exalt_dbus_conn* conn;
    conn = (exalt_dbus_conn*)user_data;

    printf("\nNOTIFY: %s: ",interface);

    switch(action)
    {
        case EXALT_ETH_CB_ACTION_NEW:
            printf("New\n");
            break;
        case EXALT_ETH_CB_ACTION_ADD:
            printf("New\n");
            break;
        case EXALT_ETH_CB_ACTION_REMOVE:
            printf("Removed\n");
            break;
        case EXALT_ETH_CB_ACTION_UP:
            printf("Up\n");
            break;
        case EXALT_ETH_CB_ACTION_DOWN:
            printf("Down\n");
            break;
        case EXALT_ETH_CB_ACTION_LINK:
            printf("Link\n");
            break;
        case EXALT_ETH_CB_ACTION_UNLINK:
            printf("Unlink\n");
            break;
        case EXALT_ETH_CB_ACTION_ADDRESS_NEW:
            printf("New ip: %s\n", exalt_dbus_eth_get_ip(conn,interface));
            break;
        case EXALT_ETH_CB_ACTION_NETMASK_NEW:
            printf("New netmask: %s\n",exalt_dbus_eth_get_netmask(conn,interface));
            break;
        case EXALT_ETH_CB_ACTION_GATEWAY_NEW:
            printf("New netmask: %s\n",exalt_dbus_eth_get_gateway(conn,interface));
            break;
        case EXALT_WIRELESS_CB_ACTION_ESSIDCHANGE:
            printf("New essid\n");
            break;
    }
}

int main(int argc, char** argv)
{
    Ecore_List *interfaces;
    Ecore_List* networks;
    Ecore_List* dnss;
    char* interface;
    char* essid;
    char* dns;

    exalt_dbus_conn* conn;
    exalt_dbus_init();
    conn = exalt_dbus_connect();
    if(!conn)
    {
        fprintf(stderr,"main(): not connected!\n");
        return -1;
    }

    exalt_dbus_notify_set(conn,NULL,NULL);
    exalt_dbus_notify_set(conn,notify_cb,conn);

    dnss = exalt_dbus_dns_get_list(conn);
    ecore_list_first_goto(dnss);
    printf("DNS LIST: \n\n");
    while ((dns =ecore_list_next(dnss)))
        printf("%s\n", dns);

    interfaces = exalt_dbus_eth_get_list(conn);
    if(!interfaces)
        return -1;

    printf("\n\nINTERFACES LIST: \n\n");
    ecore_list_first_goto(interfaces);
    if(ecore_list_empty_is(interfaces))
        printf("no interfaces\n");
    else
        while( (interface = ecore_list_next(interfaces)))
        {
            printf("name     : %s\n",interface);
            printf("wireless?: %d\n",exalt_dbus_eth_is_wireless(conn,interface));
            if(exalt_dbus_eth_is_wireless(conn,interface) >0)
            {
                printf("\tcurrent essid: %s\n", exalt_dbus_wireless_get_essid(conn,interface));
                networks = exalt_dbus_wireless_scan_wait(conn, interface);
                ecore_list_first_goto(networks);
                if(ecore_list_empty_is(networks))
                    printf("\t no networks\n");
                while ( (essid = ecore_list_next(networks)))
                {
                    printf("\tEssid     : %s\n", essid);
                    printf("\t\tQuality     : %d%%\n", exalt_dbus_wirelessinfo_get_quality(conn,interface,essid));
                    printf("\t\tEncryption  : %d\n", exalt_dbus_wirelessinfo_get_encryption(conn,interface,essid));
                    printf("\t\tSignal lvl  : %d\n", exalt_dbus_wirelessinfo_get_signallvl(conn,interface,essid));
                    printf("\t\tNoise lvl   : %d\n", exalt_dbus_wirelessinfo_get_noiselvl(conn,interface,essid));
                    printf("\t\tAddr        : %s\n", exalt_dbus_wirelessinfo_get_addr(conn,interface,essid));
                    printf("\t\tProtocol    : %s\n", exalt_dbus_wirelessinfo_get_protocol(conn,interface,essid));
                    printf("\t\tMode        : %s\n", exalt_dbus_wirelessinfo_get_mode(conn,interface,essid));
                    printf("\t\tChannel     : %s\n", exalt_dbus_wirelessinfo_get_channel(conn,interface,essid));
                    printf("\t\tBit rates   : %s\n", exalt_dbus_wirelessinfo_get_bitrates(conn,interface,essid));

                    if( exalt_dbus_wirelessinfo_is_default_dhcp(conn,interface,essid))
                        printf("\t\tdhcp mode\n");
                    else
                    {
                        printf("\t\tstatic mode\n");
                        printf("\t\tAddress     : %s\n", exalt_dbus_wirelessinfo_get_default_ip(conn,interface,essid));
                        printf("\t\tNetmask     : %s\n", exalt_dbus_wirelessinfo_get_default_netmask(conn,interface,essid));
                        printf("\t\tGateway     : %s\n", exalt_dbus_wirelessinfo_get_default_gateway(conn,interface,essid));
                    }
                    printf("\t\tPasswd mode : %d\n", exalt_dbus_wirelessinfo_get_default_passwd_mode(conn,interface,essid));
                    printf("\t\tPasswd      : %s\n", exalt_dbus_wirelessinfo_get_default_passwd(conn,interface,essid));
                    printf("\t\tSecurity    : %d\n", exalt_dbus_wirelessinfo_get_default_security_mode(conn,interface,essid));
                    printf("\t\tMode        : %d\n", exalt_dbus_wirelessinfo_get_default_mode(conn,interface,essid));
                }
            }
            printf("link?    : %d\n",exalt_dbus_eth_is_link(conn,interface));
            printf("up?      : %d\n",exalt_dbus_eth_is_up(conn, interface));
            printf("ip       : %s\n",exalt_dbus_eth_get_ip(conn,interface));
            printf("mask     : %s\n",exalt_dbus_eth_get_netmask(conn,interface));
            printf("gateway  : %s\n",exalt_dbus_eth_get_gateway(conn,interface));
            printf("\n");
        }

    ecore_main_loop_begin();
    ecore_list_destroy(interfaces);
    ecore_list_destroy(dnss);
    exalt_dbus_shutdown();
    return 1;
}
