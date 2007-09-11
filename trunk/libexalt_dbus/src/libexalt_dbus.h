/*
 * =====================================================================================
 *
 *       Filename:  libexalt_dbus.h
 *
 *    Description:  include all headers and define generals functions
 *
 *        Version:  0.0.1
 *        Created:  08/28/2007 04:15:01 PM CEST
 *       Revision:  none
 *       Compiler:  gcc
 *
 *         Author:   (Watchwolf), Atton Jonathan <watchwolf@watchwolf.fr
 *        Company:
 *
 * =====================================================================================
 */

#ifndef  LIBEXALT_DBUS_INC
#define  LIBEXALT_DBUS_INC

typedef struct _exalt_dbus_conn exalt_dbus_conn;
typedef struct _exalt_dbus_notify_data exalt_dbus_notify_data;
typedef void (exalt_notify_cb) (char* eth, int action, void* user_data);

#include <E_DBus.h>
#include <Ecore_Data.h>
#include <string.h>
#include "define.h"
#include "exalt_dbus_ethernet.h"
#include "exalt_dbus_wireless.h"
#include "exalt_dbus_wireless_info.h"

struct _exalt_dbus_conn
{
    E_DBus_Connection *e_conn;
    DBusConnection* conn;
    exalt_dbus_notify_data* notify;
};

struct _exalt_dbus_notify_data
{
    exalt_notify_cb* cb;
    void* user_data;
};

void exalt_dbus_init();
void exalt_dbus_shutdown();
exalt_dbus_conn*  exalt_dbus_connect();
void exalt_dbus_notify_set(exalt_dbus_conn* conn, exalt_notify_cb* cb, void* user_data);

#endif   /* ----- #ifndef LIBEXALT_DBUS_INC  ----- */

