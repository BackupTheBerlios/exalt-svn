/*
 * =====================================================================================
 *
 *       Filename:  exalt_dbus_wireless_info.h
 *
 *    Description:  All functions about a wireless network
 *
 *        Version:  1.0
 *        Created:  08/31/2007 08:58:01 PM CEST
 *       Revision:  none
 *       Compiler:  gcc
 *
 *         Author:   (Watchwolf), Atton Jonathan <watchwolf@watchwolf.fr>
 *        Company:
 *
 * =====================================================================================
 */

#ifndef  EXALT_DBUS_WIRELESS_INFO_INC
#define  EXALT_DBUS_WIRELESS_INFO_INC

#include "libexalt_dbus.h"

int exalt_dbus_wirelessinfo_get_quality(const exalt_dbus_conn* conn, const char* eth, const char* essid);
int exalt_dbus_wirelessinfo_get_encryption(const exalt_dbus_conn* conn, const char* eth, const char* essid);
int exalt_dbus_wirelessinfo_get_signallvl(const exalt_dbus_conn* conn, const char* eth, const char* essid);
int exalt_dbus_wirelessinfo_get_noiselvl(const exalt_dbus_conn* conn, const char* eth, const char* essid);
char* exalt_dbus_wirelessinfo_get_addr(const exalt_dbus_conn* conn, const char* eth, const char* essid);
char* exalt_dbus_wirelessinfo_get_protocol(const exalt_dbus_conn* conn, const char* eth, const char* essid);
char* exalt_dbus_wirelessinfo_get_mode(const exalt_dbus_conn* conn, const char* eth, const char* essid);
char* exalt_dbus_wirelessinfo_get_channel(const exalt_dbus_conn* conn, const char* eth, const char* essid);
char* exalt_dbus_wirelessinfo_get_bitrates(const exalt_dbus_conn* conn, const char* eth, const char* essid);


char* exalt_dbus_wirelessinfo_get_default_passwd(const exalt_dbus_conn* conn, const char* eth, const char* essid);
char* exalt_dbus_wirelessinfo_get_default_ip(const exalt_dbus_conn* conn, const char* eth, const char* essid);
char* exalt_dbus_wirelessinfo_get_default_netmask(const exalt_dbus_conn* conn, const char* eth, const char* essid);
char* exalt_dbus_wirelessinfo_get_default_gateway(const exalt_dbus_conn* conn, const char* eth, const char* essid);
int exalt_dbus_wirelessinfo_get_default_passwd_mode(const exalt_dbus_conn* conn, const char* eth, const char* essid);
int exalt_dbus_wirelessinfo_get_default_security_mode(const exalt_dbus_conn* conn, const char* eth, const char* essid);
int exalt_dbus_wirelessinfo_get_default_mode(const exalt_dbus_conn* conn, const char* eth, const char* essid);
int exalt_dbus_wirelessinfo_is_default_dhcp(const exalt_dbus_conn* conn, const char* eth, const char* essid);
#endif   /* ----- #ifndef EXALT_DBUS_WIRELESS_INFO_INC  ----- */

