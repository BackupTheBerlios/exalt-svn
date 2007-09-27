/**  @file Exalt_Wireless_Info.h */
#ifndef WIRELESS_INFO_H
#define WIRELESS_INFO_H

#include <stdio.h>
/**
 * @defgroup Exalt_Wireless_Info
 * @brief The Exalt_Wireless_Info contains all informations about a wireless networks (essid, protocol ...)
 * @{
 */

/**
 * @brief informations about a wireless network
 * @structinfo
 */
typedef struct Exalt_Wireless_Info Exalt_Wireless_Info;

#define Exalt_Wireless_Info(x) (Exalt_Wireless_Info*)x

#include "exalt_wireless.h"
#include "libexalt.h"

Exalt_Wireless_Info* exalt_wirelessinfo_create(Exalt_Wireless* w);
void exalt_wirelessinfo_free(void* data);

Exalt_Wireless* exalt_wirelessinfo_get_wireless(Exalt_Wireless_Info* w);
void exalt_wirelessinfo_set_address(Exalt_Wireless_Info* w, const char* address);
void exalt_wirelessinfo_set_essid(Exalt_Wireless_Info* w, const char* essid);
void exalt_wirelessinfo_set_mode(Exalt_Wireless_Info* w, const char* mode);
void exalt_wirelessinfo_set_protocol(Exalt_Wireless_Info* w, const char* protocol);
void exalt_wirelessinfo_set_channel(Exalt_Wireless_Info* w, const char* channel);
void exalt_wirelessinfo_set_encryption(Exalt_Wireless_Info* w,int encryption);
void exalt_wirelessinfo_set_bitrates(Exalt_Wireless_Info* w, const char* bit_rates);
void exalt_wirelessinfo_set_quality(Exalt_Wireless_Info* w, int quality);
void exalt_wirelessinfo_set_signalvl(Exalt_Wireless_Info* w, int signal_lvl);
void exalt_wirelessinfo_set_noiselvl(Exalt_Wireless_Info* w, int noise_lvl);
void exalt_wirelessinfo_set_scanok(Exalt_Wireless_Info *w,short ok);
void exalt_wirelessinfo_set_known(Exalt_Wireless_Info* w,short known);

const char* exalt_wirelessinfo_get_addr(Exalt_Wireless_Info* w);
const char* exalt_wirelessinfo_get_essid(Exalt_Wireless_Info* w);
const char* exalt_wirelessinfo_get_protocol(Exalt_Wireless_Info* w);
const char* exalt_wirelessinfo_get_mode(Exalt_Wireless_Info* w);
const char* exalt_wirelessinfo_get_channel(Exalt_Wireless_Info* w);
int exalt_wirelessinfo_get_encryption(Exalt_Wireless_Info* w);
const char* exalt_wirelessinfo_get_bitrates(Exalt_Wireless_Info* w);
int exalt_wirelessinfo_get_quality(Exalt_Wireless_Info* w);
int exalt_wirelessinfo_get_signallvl(Exalt_Wireless_Info* w);
int exalt_wirelessinfo_get_noiselvl(Exalt_Wireless_Info* w);

int exalt_wirelessinfo_is_known(Exalt_Wireless_Info* wi);
int exalt_wirelessinfo_is_scan(Exalt_Wireless_Info* wi);






const char* exalt_wirelessinfo_get_default_passwd(Exalt_Wireless_Info* wi);
void exalt_wirelessinfo_set_default_passwd(Exalt_Wireless_Info* wi,const char* passwd);
int exalt_wirelessinfo_get_default_passwd_mode(Exalt_Wireless_Info* wi);
void exalt_wirelessinfo_set_default_passwd_mode(Exalt_Wireless_Info* wi,int passwd_mode);
int exalt_wirelessinfo_get_default_security_mode(Exalt_Wireless_Info* wi);
void exalt_wirelessinfo_set_default_security_mode(Exalt_Wireless_Info* wi,int security_mode);
int exalt_wirelessinfo_get_default_mode(Exalt_Wireless_Info* wi);
void exalt_wirelessinfo_set_default_mode(Exalt_Wireless_Info* wi,int mode);

const char* exalt_wirelessinfo_get_default_ip(Exalt_Wireless_Info* wi);
const char* exalt_wirelessinfo_get_default_netmask(Exalt_Wireless_Info* wi);
const char* exalt_wirelessinfo_get_default_gateway(Exalt_Wireless_Info* wi);
int exalt_wirelessinfo_set_default_ip(Exalt_Wireless_Info* wi, const char* ip);
int exalt_wirelessinfo_set_default_netmask(Exalt_Wireless_Info* wi, const char* netmask);
int exalt_wirelessinfo_set_default_gateway(Exalt_Wireless_Info* wi, const char* gateway);

short exalt_wirelessinfo_is_default_dhcp(Exalt_Wireless_Info* wi);
int exalt_wirelessinfo_set_default_dhcp(Exalt_Wireless_Info* wi, short dhcp);


/** @} */

#endif

