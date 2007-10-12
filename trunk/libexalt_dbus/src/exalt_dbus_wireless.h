/*
 * =====================================================================================
 *
 *       Filename:  exalt_dbus_wireless.h
 *
 *    Description: All functions about a wireless device
 *
 *        Version:  1.0
 *        Created:  08/31/2007 12:34:00 PM CEST
 *       Revision:  none
 *       Compiler:  gcc
 *
 *         Author:   (Watchwolf), Atton Jonathan <watchwolf@watchwolf.fr>
 *        Company:
 *
 * =====================================================================================
 */

#ifndef  EXALT_DBUS_WIRELESS_INC
#define  EXALT_DBUS_WIRELESS_INC

#include <libexalt_dbus.h>

Ecore_List* exalt_dbus_wireless_scan_wait(const exalt_dbus_conn* conn, const char* eth);
char* exalt_dbus_wireless_get_essid(const exalt_dbus_conn* conn, const char* eth);
void exalt_dbus_wireless_scan_start(const exalt_dbus_conn* conn, const char* eth);
void exalt_dbus_wireless_scan_stop(const exalt_dbus_conn* conn, const char* eth);

#endif   /* ----- #ifndef EXALT_DBUS_WIRELESS_INC  ----- */

