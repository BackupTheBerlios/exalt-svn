/*
 * =====================================================================================
 *
 *       Filename:  daemon.h
 *
 *    Description:  define globals functions
 *
 *        Version:  1.0
 *        Created:  08/28/2007 04:26:56 PM CEST
 *       Revision:  none
 *       Compiler:  gcc
 *
 *         Author:   (Watchwolf), Atton Jonathan <watchwolf@watchwolf.fr>
 *        Company:
 *
 * =====================================================================================
 */

#ifndef  DAEMON_INC
#define  DAEMON_INC

#include <stdio.h>
#include <libexalt.h>

#include "define.h"
#include "cb_ethernet.h"
#include "cb_wireless.h"
#include "cb_wirelessinfo.h"
#include "cb_dns.h"

void print_error(const char* type, const char*file, int line, const char* fct, const char* msg, ...);

int main(int argc, char** argv);
int setup(E_DBus_Connection *conn);
Exalt_Ethernet* dbus_get_eth(DBusMessage* msg);
Exalt_Wireless_Info* dbus_get_wirelessinfo(DBusMessage* msg);
Exalt_Wireless_Info* get_wirelessinfo(Exalt_Ethernet* eth, char* essid);
void eth_cb(Exalt_Ethernet* eth, int action, void* data);
void wireless_scan_cb(Exalt_Ethernet* eth, Ecore_List* new_networks, Ecore_List* old_networks, void* data);

DBusMessage * dbus_cb_is_address(E_DBus_Object *obj __UNUSED__, DBusMessage *msg);
DBusMessage * dbus_cb_is_essid(E_DBus_Object *obj __UNUSED__, DBusMessage *msg);
DBusMessage * dbus_cb_is_passwd(E_DBus_Object *obj __UNUSED__, DBusMessage *msg);
DBusMessage * dbus_cb_wpasupplicant_is_support(E_DBus_Object *obj __UNUSED__, DBusMessage *msg);
DBusMessage * dbus_cb_dhcp_is_support(E_DBus_Object *obj __UNUSED__, DBusMessage *msg);

#endif   /* ----- #ifndef DAEMON_INC  ----- */


