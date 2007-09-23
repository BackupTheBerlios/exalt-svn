/*
 * =====================================================================================
 *
 *       Filename:  define.h
 *
 *    Description:  define some variables
 *
 *        Version:  1.0
 *        Created:  08/29/2007 01:40:41 PM CEST
 *       Revision:  none
 *       Compiler:  gcc
 *
 *         Author:   (Watchwolf), Atton Jonathan <watchwolf@watchwolf.fr>
 *        Company:
 *
 * =====================================================================================
 */

#ifndef  DEFINE_INC
#define  DEFINE_INC


#define EXALTD_SERVICE "org.e.Exalt"
#define EXALTD_INTERFACE_READ "org.e.Exalt.Read"
#define EXALTD_INTERFACE_WRITE "org.e.Exalt.Write"
#define EXALTD_PATH "/org/e/Exalt"

#define exalt_dbus_read_call_new(member) dbus_message_new_method_call(EXALTD_SERVICE,EXALTD_PATH, EXALTD_INTERFACE_READ, member)
#define exalt_dbus_write_call_new(member) dbus_message_new_method_call(EXALTD_SERVICE,EXALTD_PATH, EXALTD_INTERFACE_WRITE, member)

#define EXALT_DBUS_FREE(a) (a?free(a):NULL)

#define EXALT_DBUS_STATIC 0
#define EXALT_DBUS_DHCP 1


/** when we load the device list */
#define EXALT_ETH_CB_ACTION_NEW 0
/** when we have a new card */
#define EXALT_ETH_CB_ACTION_ADD 1
/** when we have a remove card */
#define EXALT_ETH_CB_ACTION_REMOVE 2
/** when a known card is up */
#define EXALT_ETH_CB_ACTION_UP 3
/** when a known card is down */
#define EXALT_ETH_CB_ACTION_DOWN 4
/** when a card is link */
#define EXALT_ETH_CB_ACTION_LINK 5
/** when a card is unlink */
#define EXALT_ETH_CB_ACTION_UNLINK 6
/** when an essid change */
#define EXALT_WIRELESS_CB_ACTION_ESSIDCHANGE 7

/** when we have a new address */
#define EXALT_ETH_CB_ACTION_ADDRESS_NEW 9
/** when we have a new netmask */
#define EXALT_ETH_CB_ACTION_NETMASK_NEW 10
/** when we have a new gateway */
#define EXALT_ETH_CB_ACTION_GATEWAY_NEW 11

/** No encryption password mode */
#define EXALT_DBUS_WIRELESS_ENCRYPTION_NONE 0
/** WEP ASCII encryption plain text */
#define EXALT_DBUS_WIRELESS_ENCRYPTION_WEP_ASCII 1
/** WEP Hexadecimal encryption mode */
#define EXALT_DBUS_WIRELESS_ENCRYPTION_WEP_HEXA 2
/** WPA PSK CCMP (ASCII mode) */
#define EXALT_DBUS_WIRELESS_ENCRYPTION_WPA_PSK_CCMP_ASCII 3
/** WPA_PSK TKIP (ASCII mode) */
#define EXALT_DBUS_WIRELESS_ENCRYPTION_WPA_PSK_TKIP_ASCII 4
/** WPA2 PSK CCMP (ASCII mode) */
#define EXALT_DBUS_WIRELESS_ENCRYPTION_WPA2_PSK_CCMP_ASCII 5
/** WPA2_PSK TKIP (ASCII mode) */
#define EXALT_DBUS_WIRELESS_ENCRYPTION_WPA2_PSK_TKIP_ASCII 6

/** Ad-Hoc connection mode */
#define EXALT_DBUS_WIRELESS_MODE_ADHOC 1
/** Managed connection mode */
#define EXALT_DBUS_WIRELESS_MODE_MANAGED 2

/** Open security mode */
#define EXALT_DBUS_WIRELESS_SECURITY_OPEN 0
/** Shared security mode */
#define EXALT_DBUS_WIRELESS_SECURITY_SHARED 1


#endif   /* ----- #ifndef DEFINE_INC  ----- */

