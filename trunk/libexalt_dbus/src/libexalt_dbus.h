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

#include <E_DBus.h>
#include <Ecore_Data.h>
#include <string.h>
#include "define.h"
#include "exalt_dbus_ethernet.h"

struct _exalt_dbus_conn
{
    E_DBus_Connection *e_conn;
    DBusConnection* conn;
};

void exalt_dbus_init();
void exalt_dbus_shutdown();
exalt_dbus_conn*  exalt_dbus_connect();
char* exalt_dbus_response_string(DBusMessage *msg);
Ecore_List* exalt_dbus_response_strings(DBusMessage *msg);
void exalt_dbus_string_free(void* data);
int exalt_dbus_response_boolean(DBusMessage *msg);

#endif   /* ----- #ifndef LIBEXALT_DBUS_INC  ----- */

