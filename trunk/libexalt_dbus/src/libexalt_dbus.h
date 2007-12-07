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

#include <E_DBus.h>
#include <Ecore_Data.h>
#include <Ecore_Data.h>
#include <string.h>
#include <libexalt.h>

typedef struct _exalt_dbus_conn exalt_dbus_conn;
typedef struct _exalt_dbus_notify_data exalt_dbus_notify_data;
typedef struct _exalt_dbus_notify_conn_applied_data exalt_dbus_notify_conn_applied_data;
typedef struct _exalt_dbus_scan_notify_data exalt_dbus_scan_notify_data;

typedef void (exalt_notify_cb) (char* eth, Exalt_Enum_Action action, void* user_data);
typedef void (exalt_scan_notify_cb) (char* eth, Ecore_List* new_networks, Ecore_List *old_networks, void* user_data);
typedef void (exalt_notify_conn_applied_cb) (char* interface, void* user_data);

#include "define.h"
#include "exalt_dbus_ethernet.h"
#include "exalt_dbus_wireless.h"
#include "exalt_dbus_wireless_info.h"
#include "exalt_dbus_dns.h"

void exalt_dbus_init();
void exalt_dbus_free(exalt_dbus_conn** conn);
void exalt_dbus_shutdown();
exalt_dbus_conn*  exalt_dbus_connect();
void exalt_dbus_notify_set(exalt_dbus_conn* conn, exalt_notify_cb* cb, void* user_data);
void exalt_dbus_scan_notify_set(exalt_dbus_conn* conn, exalt_scan_notify_cb* cb, void* user_data);

int exalt_dbus_wpasupplicant_is_support(const exalt_dbus_conn* conn);
int exalt_dbus_dhcp_is_support(const exalt_dbus_conn* conn);


#endif   /* ----- #ifndef LIBEXALT_DBUS_INC  ----- */

