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

char* exalt_dbus_eth_get_ip(const exalt_dbus_conn* conn,const char* eth);
char* exalt_dbus_eth_get_netmask(const exalt_dbus_conn* conn, const char* eth);
char* exalt_dbus_eth_get_gateway(const exalt_dbus_conn* conn, const char* eth);
Ecore_List* exalt_dbus_eth_get_list(const exalt_dbus_conn* conn);
int exalt_dbus_eth_is_wireless(const exalt_dbus_conn* conn, const char* eth);
int exalt_dbus_eth_is_link(const exalt_dbus_conn* conn, const char* eth);
int exalt_dbus_eth_is_up(const exalt_dbus_conn* conn, const char* eth);
int exalt_dbus_eth_is_dhcp(const exalt_dbus_conn* conn, const char* eth);


int exalt_dbus_eth_up(const exalt_dbus_conn* conn, const char* eth);
int exalt_dbus_eth_down(const exalt_dbus_conn* conn, const char* eth);

int exalt_dbus_eth_apply_conn(exalt_dbus_conn* conn, const char* eth,Exalt_Connection*c);
void _exalt_dbus_notify_conn_applied(void *data, DBusMessage *msg);

#endif   /* ----- #ifndef EXALT_DBUS_ETHERNET_INC  ----- */

