/** @file libexalt.h */

#ifndef COMMON_H
#define COMMON_H


#define EXALT_FREE(p) do { if(p){ free(p); p=NULL;} }while(0)
#define EXALT_PCLOSE(p) do {if(p){pclose(p); p=NULL;} }while(0)
#define EXALT_DELETE_TIMER(a) do {if(a) {ecore_timer_del(a); a=NULL; } }while(0)
#define EXALT_CLOSE(p) do {if(p>=0){close(p); p=0;} }while(0)
#define EXALT_FCLOSE(p) do {if(p>=0){fclose(p); p=NULL;} }while(0)

#define EXALT_PATH_PROCNET_DEV "/proc/net/dev"
#define EXALT_PATH_ROUTE "/proc/net/route"
#define EXALT_WPA_IFACE_DIR "/var/run/wpa_supplicant"
#define EXALT_TEMP_FILE "/tmp/exalt_temp"

#include <netinet/in.h>
#include <arpa/inet.h>
#include <sys/socket.h>
#include <net/route.h>

#include <linux/types.h>
#include <linux/ethtool.h>

#include <sys/ioctl.h>
#include <linux/sockios.h>

#include <Eet.h>

#include "exalt_ethernet.h"
#include "exalt_wireless_info.h"
#include "exalt_wireless.h"
#include "exalt_dns.h"
#include "exalt_sys_conf.h"
#include "exalt_connection.h"

/**
 * @defgroup Exalt_General
 * @brief Some functions
 * @{
 */



short exalt_is_address(const char* ip);
short exalt_is_essid(const char* essid);
short exalt_is_admin();
short exalt_is_key(const char* key, Exalt_Enum_Encryption_Mode encryption_mode);

short exalt_wpasupplicant_is_support();
short exalt_dhcp_is_support();




#endif

/** @} */
