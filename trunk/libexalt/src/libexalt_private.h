/*
 * =====================================================================================
 *
 *       Filename:  libexalt_private.h
 *
 *    Description:  private functions
 *
 *        Version:  1.0
 *        Created:  09/03/2007 09:00:35 PM CEST
 *       Revision:  none
 *       Compiler:  gcc
 *
 *         Author:   (Watchwolf), Atton Jonathan <watchwolf@watchwolf.fr>
 *        Company:
 *
 * =====================================================================================
 */

#ifndef  LIBEXALT_PRIVATE_INC
#define  LIBEXALT_PRIVATE_INC

#include "libexalt.h"
#include <string.h>
#include "nettools/proc.h"
#include "iw/iwlib.h"
#include "wpa_supplicant/wpa_ctrl.h"
#include "exalt_command.h"
#include "exalt_regexp.h"
#include <Eet.h>

struct Exalt_Ethernets
{
    Ecore_List* ethernets;

    int we_version;

    short admin;
    short is_launch ;
    E_DBus_Connection *dbus_conn;
    Ecore_Fd_Handler *rtlink_watch;

    Exalt_Eth_Cb eth_cb;
    void * eth_cb_user_data;

    Exalt_Wifi_Scan_Cb wireless_scan_cb;
    void* wireless_scan_cb_user_data;
};




char *str_remove (const char *s, const char *ct);
void print_error(const char* type, const char* file, int line,const char* fct, const char* msg, ...);
char* exalt_addr_hexa_to_dec(const char* addr);
short exalt_ioctl(void* argp, int request);

int _exalt_rtlink_essid_change(Exalt_Wireless *w);


Eet_Data_Descriptor * exalt_eth_edd_new(Eet_Data_Descriptor *edd_w);
Eet_Data_Descriptor * exalt_wireless_edd_new();
Eet_Data_Descriptor * exalt_wirelessinfo_edd_new();

#endif   /* ----- #ifndef LIBEXALT_PRIVATE_INC  ----- */

