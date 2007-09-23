/*
 * =====================================================================================
 *
 *       Filename:  cb_wireless.h
 *
 *    Description:  All functions about a wireless interface
 *
 *        Version:  1.0
 *        Created:  09/01/2007 12:06:19 PM CEST
 *       Revision:  none
 *       Compiler:  gcc
 *
 *         Author:   (Watchwolf), Atton Jonathan <watchwolf@watchwolf.fr>
 *        Company:
 *
 * =====================================================================================
 */

#ifndef  CB_WIRELESS_INC
#define  CB_WIRELESS_INC

#include "daemon.h"

DBusMessage * dbus_cb_wireless_scan_wait(E_DBus_Object *obj, DBusMessage *msg);
DBusMessage * dbus_cb_wireless_get_essid(E_DBus_Object *obj, DBusMessage *msg);

DBusMessage * dbus_cb_wireless_scan_start(E_DBus_Object *obj __UNUSED__, DBusMessage *msg);
DBusMessage * dbus_cb_wireless_scan_stop(E_DBus_Object *obj __UNUSED__, DBusMessage *msg);


DBusMessage * dbus_cb_eth_set_new_essid(E_DBus_Object *obj __UNUSED__, DBusMessage *msg);
DBusMessage * dbus_cb_eth_set_new_passwd(E_DBus_Object *obj __UNUSED__, DBusMessage *msg);
DBusMessage * dbus_cb_eth_set_new_passwd_mode(E_DBus_Object *obj __UNUSED__, DBusMessage *msg);
DBusMessage * dbus_cb_eth_set_new_mode(E_DBus_Object *obj __UNUSED__, DBusMessage *msg);
DBusMessage * dbus_cb_eth_set_new_security_mode(E_DBus_Object *obj __UNUSED__, DBusMessage *msg);
#endif   /* ----- #ifndef CB_WIRELESS_INC  ----- */

