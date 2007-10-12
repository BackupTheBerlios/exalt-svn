/*
 * =====================================================================================
 *
 *       Filename:  cb_wirelessinfo.h
 *
 *    Description:  All cb about a wireless network
 *
 *        Version:  1.0
 *        Created:  09/01/2007 12:07:53 PM CEST
 *       Revision:  none
 *       Compiler:  gcc
 *
 *         Author:   (Watchwolf), Atton Jonathan <watchwolf@watchwolf.fr>
 *        Company:
 *
 * =====================================================================================
 */

#ifndef  CB_WIRELESSINFO_INC
#define  CB_WIRELESSINFO_INC

#include "daemon.h"

DBusMessage * dbus_cb_wirelessinfo_get_quality(E_DBus_Object *obj, DBusMessage *msg);
DBusMessage * dbus_cb_wirelessinfo_get_addr(E_DBus_Object *obj, DBusMessage *msg);
DBusMessage * dbus_cb_wirelessinfo_get_protocol(E_DBus_Object *obj, DBusMessage *msg);
DBusMessage * dbus_cb_wirelessinfo_get_mode(E_DBus_Object *obj, DBusMessage *msg);
DBusMessage * dbus_cb_wirelessinfo_get_channel(E_DBus_Object *obj, DBusMessage *msg);
DBusMessage * dbus_cb_wirelessinfo_get_bitrates(E_DBus_Object *obj, DBusMessage *msg);
DBusMessage * dbus_cb_wirelessinfo_get_encryption(E_DBus_Object *obj, DBusMessage *msg);
DBusMessage * dbus_cb_wirelessinfo_get_signallvl(E_DBus_Object *obj, DBusMessage *msg);
DBusMessage * dbus_cb_wirelessinfo_get_noiselvl(E_DBus_Object *obj, DBusMessage *msg);


DBusMessage * dbus_cb_wirelessinfo_get_default_conn(E_DBus_Object *obj, DBusMessage *msg);

#endif   /* ----- #ifndef CB_WIRELESSINFO_INC  ----- */

