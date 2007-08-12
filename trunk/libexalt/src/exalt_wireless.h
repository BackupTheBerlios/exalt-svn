/**  @file exalt_wireless.h */
#ifndef WIRELESS_H
#define WIRELESS_H

#include <stdio.h>

/**
 * @defgroup Exalt_wireless
 * @brief The exalt_wireless contains all informations about your wireless card as name, essid, list of scanning networks ...
 * @{
 */



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

typedef struct exalt_wireless exalt_wireless;

#include "libexalt.h"
#include "exalt_wireless_info.h"
#include <Ecore_Data.h>
#include <Ecore.h>
#include "iwlib.h"
#include "wpa_ctrl.h"

/**
 * @brief informations about a wireless card
 * @structinfo
 */
struct exalt_wireless
{
	exalt_ethernet* eth;
	char* new_essid;
	char* new_passwd;
	int new_passwd_mode;
	int new_mode;
        int new_security_mode;

        Ecore_List* networks;

        char* _save_essid;

	//use for scanning
	Ecore_Timer* scan_cb_timer;
	wireless_scan_head *context;
	int scan_fd;
};


exalt_wireless* exalt_wireless_create(exalt_ethernet* eth);
void exalt_wireless_free(exalt_wireless* w);
void exalt_wireless_scan_execute(exalt_ethernet * eth);
void exalt_wireless_scan_load(exalt_ethernet* eth);
int exalt_wireless_scan(void* data);


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

void exalt_wireless_printf(exalt_wireless w);
void exalt_wireless_printf_scan(exalt_wireless w);


void exalt_wireless_scan_free(wireless_scan **w);

struct wpa_ctrl * exalt_wpa_open_connection(const char *ifname);
int exalt_wpa_ctrl_command(struct wpa_ctrl *ctrl, char *cmd);





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

int exalt_rtlink_essid_change(exalt_wireless *w);

/** @} */

#endif

