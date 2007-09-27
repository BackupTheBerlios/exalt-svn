/**  @file Exalt_Wireless.h */
#ifndef WIRELESS_H
#define WIRELESS_H

/**
 * @defgroup Exalt_Wireless
 * @brief The Exalt_Wireless contains all informations about your wireless card as name, essid, list of scanning networks ...
 * @{
 */

/**
 * @brief informations about a wireless card
 * @structinfo
 */
typedef struct Exalt_Wireless Exalt_Wireless;

/** No encryption password mode */
#define EXALT_WIRELESS_ENCRYPTION_NONE 0
/** WEP ASCII encryption plain text */
#define EXALT_WIRELESS_ENCRYPTION_WEP_ASCII 1
/** WEP Hexadecimal encryption mode */
#define EXALT_WIRELESS_ENCRYPTION_WEP_HEXA 2
/** WPA PSK CCMP (ASCII mode) */
#define EXALT_WIRELESS_ENCRYPTION_WPA_PSK_CCMP_ASCII 3
/** WPA_PSK TKIP (ASCII mode) */
#define EXALT_WIRELESS_ENCRYPTION_WPA_PSK_TKIP_ASCII 4
/** WPA2 PSK CCMP (ASCII mode) */
#define EXALT_WIRELESS_ENCRYPTION_WPA2_PSK_CCMP_ASCII 5
/** WPA2_PSK TKIP (ASCII mode) */
#define EXALT_WIRELESS_ENCRYPTION_WPA2_PSK_TKIP_ASCII 6

/** Ad-Hoc connection mode */
#define EXALT_WIRELESS_MODE_ADHOC 1
/** Managed connection mode */
#define EXALT_WIRELESS_MODE_MANAGED 2

/** Open security mode */
#define EXALT_WIRELESS_SECURITY_OPEN 0
/** Shared security mode */
#define EXALT_WIRELESS_SECURITY_SHARED 1


#define EXALT_WIRELESS_SCAN_UPDATE_TIME 5


#include "libexalt.h"
#include "exalt_wireless_info.h"
#include <Ecore_Data.h>
#include <Ecore.h>
#include <stdio.h>


Exalt_Wireless* exalt_wireless_create(Exalt_Ethernet* eth);
void exalt_wireless_free(Exalt_Wireless* w);

char* exalt_wireless_get_driver(Exalt_Wireless* w);

void exalt_wireless_scan_start(Exalt_Ethernet* eth);
void exalt_wireless_scan_stop(Exalt_Ethernet* eth);
int exalt_wireless_scan_wait(Exalt_Ethernet* eth);

Exalt_Ethernet* exalt_wireless_get_ethernet(Exalt_Wireless* w);
Ecore_List* exalt_wireless_get_networks_list(Exalt_Wireless* w);
Exalt_Ethernet* exalt_wireless_get_eth(Exalt_Wireless* w);

void exalt_set_button_state(Exalt_Wireless* w);
short exalt_wireless_radiobutton_ison(Exalt_Wireless* w);



Exalt_Wireless_Info* exalt_wireless_get_networkinfo(Exalt_Wireless* w, int nb);
Exalt_Wireless_Info* exalt_wireless_get_networkinfo_by_essid(Exalt_Wireless* w,const char *essid);


int exalt_wireless_apply_conf(Exalt_Wireless *w);

void exalt_wireless_printf(Exalt_Wireless *w);
void exalt_wireless_printf_scan(Exalt_Wireless *w);


char* exalt_wireless_get_essid(Exalt_Wireless* w);

void exalt_wireless_set_new_passwd_mode(Exalt_Wireless* w,int passwd_mode);
void exalt_wireless_set_new_mode(Exalt_Wireless* w,int mode);
void exalt_wireless_set_new_security_mode(Exalt_Wireless* w,int security_mode);
void exalt_wireless_set_new_passwd(Exalt_Wireless* w,const char* passwd);
void exalt_wireless_set_new_essid(Exalt_Wireless* w,const char* essid);

int exalt_wireless_get_new_passwd_mode(Exalt_Wireless* w);
int exalt_wireless_get_new_mode(Exalt_Wireless* w);
int exalt_wireless_get_new_security_mode(Exalt_Wireless* w);
const char* exalt_wireless_get_new_passwd(Exalt_Wireless* w);
const char* exalt_wireless_get_new_essid(Exalt_Wireless* w);

/** @} */

#endif

