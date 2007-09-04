/**  @file exalt_wireless_info.h */
#ifndef WIRELESS_INFO_H
#define WIRELESS_INFO_H

#include <stdio.h>
/**
 * @defgroup Exalt_Wireless_Info
 * @brief The exalt_wireless_info contains all informations about a wireless networks (essid, protocol ...)
 * @{
 */

typedef struct exalt_wireless_info exalt_wireless_info;
#define EXALT_WIRELESS_INFO(x) (exalt_wireless_info*)x

#include "exalt_wireless.h"
#include "libexalt.h"

exalt_wireless_info* exalt_wirelessinfo_create(exalt_wireless* w);
void exalt_wirelessinfo_free(void* data);

exalt_wireless* exalt_wirelessinfo_get_wireless(exalt_wireless_info* w);
void exalt_wirelessinfo_set_address(exalt_wireless_info* w, char* address);
void exalt_wirelessinfo_set_essid(exalt_wireless_info* w, char* essid);
void exalt_wirelessinfo_set_essid(exalt_wireless_info* w, char* essid);
void exalt_wirelessinfo_set_mode(exalt_wireless_info* w, char* mode);
void exalt_wirelessinfo_set_protocol(exalt_wireless_info* w, char* protocol);
void exalt_wirelessinfo_set_channel(exalt_wireless_info* w, char* channel);
void exalt_wirelessinfo_set_encryption(exalt_wireless_info* w,int encryption);
void exalt_wirelessinfo_set_bitrates(exalt_wireless_info* w, char* bit_rates);
void exalt_wirelessinfo_set_quality(exalt_wireless_info* w, int quality);
void exalt_wirelessinfo_set_signalvl(exalt_wireless_info* w, int signal_lvl);
void exalt_wirelessinfo_set_noiselvl(exalt_wireless_info* w, int noise_lvl);
void exalt_wirelessinfo_set_scanok(exalt_wireless_info *w,short ok);
void exalt_wirelessinfo_set_known(exalt_wireless_info* w,short known);

char* exalt_wirelessinfo_get_addr(exalt_wireless_info* w);
char* exalt_wirelessinfo_get_essid(exalt_wireless_info* w);
char* exalt_wirelessinfo_get_protocol(exalt_wireless_info* w);
char* exalt_wirelessinfo_get_mode(exalt_wireless_info* w);
char* exalt_wirelessinfo_get_channel(exalt_wireless_info* w);
int exalt_wirelessinfo_get_encryption(exalt_wireless_info* w);
char* exalt_wirelessinfo_get_bitrates(exalt_wireless_info* w);
int exalt_wirelessinfo_get_quality(exalt_wireless_info* w);
int exalt_wirelessinfo_get_signallvl(exalt_wireless_info* w);
int exalt_wirelessinfo_get_noiselvl(exalt_wireless_info* w);

int exalt_wirelessinfo_is_known(exalt_wireless_info* wi);
int exalt_wirelessinfo_is_scan(exalt_wireless_info* wi);






char* exalt_wirelessinfo_get_default_passwd(exalt_wireless_info* wi);
void exalt_wirelessinfo_set_default_passwd(exalt_wireless_info* wi,const char* passwd);
int exalt_wirelessinfo_get_default_passwd_mode(exalt_wireless_info* wi);
void exalt_wirelessinfo_set_default_passwd_mode(exalt_wireless_info* wi,int passwd_mode);
int exalt_wirelessinfo_get_default_security_mode(exalt_wireless_info* wi);
void exalt_wirelessinfo_set_default_security_mode(exalt_wireless_info* wi,int security_mode);
int exalt_wirelessinfo_get_default_mode(exalt_wireless_info* wi);
void exalt_wirelessinfo_set_default_mode(exalt_wireless_info* wi,int mode);

char* exalt_wirelessinfo_get_default_ip(exalt_wireless_info* wi);
char* exalt_wirelessinfo_get_default_netmask(exalt_wireless_info* wi);
char* exalt_wirelessinfo_get_default_gateway(exalt_wireless_info* wi);
int exalt_wirelessinfo_set_default_ip(exalt_wireless_info* wi, const char* ip);
int exalt_wirelessinfo_set_default_netmask(exalt_wireless_info* wi, const char* netmask);
int exalt_wirelessinfo_set_default_gateway(exalt_wireless_info* wi, const char* gateway);

short exalt_wirelessinfo_is_default_dhcp(exalt_wireless_info* wi);
int exalt_wirelessinfo_set_default_dhcp(exalt_wireless_info* wi, short dhcp);


/** @} */

#endif

