#ifndef COMMON_H
#define COMMON_H


#define EXALT_FREE(p) if(p){ free(p); p=NULL;}
#define EXALT_PCLOSE(p) if(p){pclose(p); p=NULL;}
#define EXALT_DELETE_TIMER(a) if(a) {ecore_timer_del(a); a=NULL; }
#define EXALT_CLOSE(p) if(p>=0){close(p); p=0;}

#define EXALT_PATH_PROCNET_DEV "/proc/net/dev"
#define EXALT_PATH_ROUTE "/proc/net/route"
#define EXALT_WPA_IFACE_DIR "/var/run/wpa_supplicant"

#define EXALT_TRUE 1
#define EXALT_FALSE 0

#define EXALT_DHCP 1
#define EXALT_STATIC 0
#define EXALT_ERROR -1


#include <netinet/in.h>
#include <arpa/inet.h>
#include <sys/socket.h>
#include <net/route.h>

#include <linux/types.h>
#include <linux/ethtool.h>

#include <sys/ioctl.h>
#include <linux/sockios.h>


#include "exalt_ethernet.h"
#include "exalt_wireless_info.h"
#include "exalt_wireless.h"
#include "exalt_command.h"
#include "exalt_dns.h"
#include "exalt_sys_conf.h"
#include "exalt_regexp.h"
#include "iwlib.h"



short exalt_is_address(const char* ip);
short exalt_is_essid(const char* essid);
short exalt_is_passwd(const char* passwd, int passwd_mode);
char *str_remove (const char *s, const char *ct);
char* exalt_addr_hexa_to_dec(char* addr);
short exalt_ioctl(void* argp, int request);
short exalt_is_admin();

#endif


