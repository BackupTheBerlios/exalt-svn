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
};

struct _exalt_dbus_notify_data
{
    exalt_dbus_conn* conn;
    exalt_notify_cb* cb;
    void* user_data;
};

void exalt_dbus_init();
void exalt_dbus_shutdown();
exalt_dbus_conn*  exalt_dbus_connect();
char* exalt_dbus_response_string(DBusMessage *msg);
Ecore_List* exalt_dbus_response_strings(DBusMessage *msg);
void exalt_dbus_string_free(void* data);
int exalt_dbus_response_boolean(DBusMessage *msg);
int exalt_dbus_response_integer(DBusMessage *msg);
void exalt_dbus_notify_set(exalt_dbus_conn* conn, exalt_notify_cb* cb, void* user_data);


void print_error(char* type, char* file, int line,const char* fct, char* msg);

#endif   /* ----- #ifndef LIBEXALT_DBUS_INC  ----- */

