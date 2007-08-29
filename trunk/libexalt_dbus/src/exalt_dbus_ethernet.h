/*
 * =====================================================================================
 *
 *       Filename:  exalt_dbus_ethernet.h
 *
 *    Description:  All functions about a ethernet device (no wireless)
 *
 *        Version:  1.0
 *        Created:  08/29/2007 02:19:50 PM CEST
 *       Revision:  none
 *       Compiler:  gcc
 *
 *         Author:   (Watchwolf), Atton Jonathan <watchwolf@watchwolf.fr>
 *        Company:
 *
 * =====================================================================================
 */

#ifndef  EXALT_DBUS_ETHERNET_INC
#define  EXALT_DBUS_ETHERNET_INC

#include "libexalt_dbus.h"
#include "define.h"

char* exalt_dbus_eth_get_ip(exalt_dbus_conn* conn, char* eth);
char* exalt_dbus_eth_get_netmask(exalt_dbus_conn* conn, char* eth);
char* exalt_dbus_eth_get_gateway(exalt_dbus_conn* conn, char* eth);
Ecore_List* exalt_dbus_eth_get_list(exalt_dbus_conn* conn);
int exalt_dbus_eth_is_wireless(exalt_dbus_conn* conn, char* eth);
int exalt_dbus_eth_is_link(exalt_dbus_conn* conn, char* eth);

#endif   /* ----- #ifndef EXALT_DBUS_ETHERNET_INC  ----- */

