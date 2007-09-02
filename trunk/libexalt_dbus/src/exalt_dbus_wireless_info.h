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

int exalt_dbus_wirelessinfo_get_quality(exalt_dbus_conn* conn, char* eth, char* essid);
int exalt_dbus_wirelessinfo_get_encryption(exalt_dbus_conn* conn, char* eth, char* essid);
int exalt_dbus_wirelessinfo_get_signallvl(exalt_dbus_conn* conn, char* eth, char* essid);
int exalt_dbus_wirelessinfo_get_noiselvl(exalt_dbus_conn* conn, char* eth, char* essid);
char* exalt_dbus_wirelessinfo_get_addr(exalt_dbus_conn* conn, char* eth, char* essid);
char* exalt_dbus_wirelessinfo_get_protocol(exalt_dbus_conn* conn, char* eth, char* essid);
char* exalt_dbus_wirelessinfo_get_mode(exalt_dbus_conn* conn, char* eth, char* essid);
char* exalt_dbus_wirelessinfo_get_channel(exalt_dbus_conn* conn, char* eth, char* essid);
char* exalt_dbus_wirelessinfo_get_bitrates(exalt_dbus_conn* conn, char* eth, char* essid);


char* exalt_dbus_wirelessinfo_get_default_passwd(exalt_dbus_conn* conn, char* eth, char* essid);
char* exalt_dbus_wirelessinfo_get_default_ip(exalt_dbus_conn* conn, char* eth, char* essid);
char* exalt_dbus_wirelessinfo_get_default_netmask(exalt_dbus_conn* conn, char* eth, char* essid);
char* exalt_dbus_wirelessinfo_get_default_gateway(exalt_dbus_conn* conn, char* eth, char* essid);
int exalt_dbus_wirelessinfo_get_default_passwd_mode(exalt_dbus_conn* conn, char* eth, char* essid);
int exalt_dbus_wirelessinfo_get_default_security_mode(exalt_dbus_conn* conn, char* eth, char* essid);
int exalt_dbus_wirelessinfo_get_default_mode(exalt_dbus_conn* conn, char* eth, char* essid);
int exalt_dbus_wirelessinfo_is_default_dhcp(exalt_dbus_conn* conn, char* eth, char* essid);
#endif   /* ----- #ifndef EXALT_DBUS_WIRELESS_INFO_INC  ----- */

