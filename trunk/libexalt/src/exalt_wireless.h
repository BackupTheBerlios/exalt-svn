/**  @file exalt_wireless.h */
#ifndef WIRELESS_H
#define WIRELESS_H

/**
 * @defgroup Exalt_Wireless
 * @brief The exalt_wireless contains all informations about your wireless card as name, essid, list of scanning networks ...
 * @{
 */

/**
 * @brief informations about a wireless card
 * @structinfo
 */
typedef struct exalt_wireless exalt_wireless;

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


/** when a new wireless network appears */
#define EXALT_WIRELESS_SCAN_CB_ACTION_NEW 1
/** when a new wireless network disappears */
#define EXALT_WIRELESS_SCAN_CB_ACTION_REMOVE 2


#define EXALT_WIRELESS_SCAN_UPDATE_TIME 10


#include "libexalt.h"
#include "exalt_wireless_info.h"
#include <Ecore_Data.h>
#include <Ecore.h>
#include <stdio.h>


exalt_wireless* exalt_wireless_create(exalt_ethernet* eth);
void exalt_wireless_free(exalt_wireless* w);

char* exalt_wireless_get_driver(exalt_wireless* w);

void exalt_wireless_scan_start(exalt_ethernet* eth);
void exalt_wireless_scan_stop(exalt_ethernet* eth);
int exalt_wireless_scan_wait(exalt_ethernet* eth);

exalt_ethernet* exalt_wireless_get_ethernet(exalt_wireless* w);
Ecore_List* exalt_wireless_get_networks_list(exalt_wireless* w);
exalt_ethernet* exalt_wireless_get_eth(exalt_wireless* w);

void exalt_set_button_state(exalt_wireless* w);
short exalt_wireless_radiobutton_ison(exalt_wireless* w);



exalt_wireless_info* exalt_wireless_get_networkinfo(exalt_wireless* w, int nb);
exalt_wireless_info* exalt_wireless_get_networkinfo_by_essid(exalt_wireless* w,char *essid);


int exalt_wireless_apply_conf(exalt_wireless *w);

void exalt_wireless_printf(exalt_wireless *w);
void exalt_wireless_printf_scan(exalt_wireless *w);


char* exalt_wireless_get_essid(exalt_wireless* w);

void exalt_wireless_set_new_passwd_mode(exalt_wireless* w,int passwd_mode);
void exalt_wireless_set_new_mode(exalt_wireless* w,int mode);
void exalt_wireless_set_new_security_mode(exalt_wireless* w,int security_mode);
void exalt_wireless_set_new_passwd(exalt_wireless* w,const char* passwd);
void exalt_wireless_set_new_essid(exalt_wireless* w,const char* essid);

int exalt_wireless_get_new_passwd_mode(exalt_wireless* w);
int exalt_wireless_get_new_mode(exalt_wireless* w);
int exalt_wireless_get_new_security_mode(exalt_wireless* w);
char* exalt_wireless_get_new_passwd(exalt_wireless* w);
char* exalt_wireless_get_new_essid(exalt_wireless* w);

/** @} */

#endif

