/** @file exalt_ethernet.h */
#ifndef ETHERNET_H
#define ETHERNET_H

typedef struct exalt_ethernet exalt_ethernet;

#include "libexalt.h"
#include "exalt_wireless.h"
#include "exalt_wireless_info.h"
#include "iwlib.h"
#include "proc.h"

#include <Ecore_Data.h>
#include <Ecore.h>
#include <E_Hal.h>
#include <sys/socket.h>
#include <sys/types.h>
#include <linux/ethtool.h>
#include <linux/netlink.h>
#include <linux/rtnetlink.h>


/**
 * @defgroup Exalt_ethernet
 * @brief the exalt_ethernet contains all informations about all cards. It's the main structure
 * @{
 */

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


/** when a new wireless network appears */
#define EXALT_WIRELESS_SCAN_CB_ACTION_NEW 1
/** when a new wireless network disappears */
#define EXALT_WIRELESS_SCAN_CB_ACTION_REMOVE 2

#define EXALT_ETHERNET(a) (exalt_ethernet*)a

#define EXALT_ETH_UPDATE_TIME 2

/** action = EXALT_ETH_CB_* */
typedef void (*Exalt_Eth_Cb) (exalt_ethernet* eth, int action, void* user_data);
#define EXALT_ETH_CB(a) (Exalt_Eth_Cb)a

#define EXALT_WIRELESS_SCAN_UPDATE_TIME 10
/**action = EXALT_WIRELESS_SCAN_CB_* */
typedef void (*Exalt_Wifi_Scan_Cb) (exalt_wireless_info* wi, int action, void* user_data);
#define EXALT_WIRELESS_SCAN_CB(a) (Exalt_Wifi_Scan_Cb)a



/**
 * @brief informations about a card
 * @structinfo
 */
struct exalt_ethernet
{
	char* name; //eth0, eth1...
	char* udi;
        int ifindex;

        char* new_ip;
	char* new_netmask;
	char* new_gateway;
	short new_dhcp_static; //EXALT_DHCP || EXALT_STATIC
	exalt_wireless* wireless; //if null, the interface is not wireless

        char* _save_ip;
        char* _save_netmask;
        char* _save_gateway;
        short _save_link;
        short _save_up;
};

/**
 * @brief all cards
 * @structinfo
 */
typedef struct Exalt_Ethernets
{
    Ecore_List* ethernets;

    int we_version;

    short admin;

    E_DBus_Connection *dbus_conn;
    Ecore_Fd_Handler *rtlink_watch;

    Exalt_Eth_Cb eth_cb;
    void * eth_cb_user_data;

    Exalt_Wifi_Scan_Cb wireless_scan_cb;
    void* wireless_scan_cb_user_data;
} Exalt_Ethernets;


Exalt_Ethernets exalt_eth_interfaces;

exalt_ethernet* exalt_eth_create(const char* name);

int exalt_eth_init();
int exalt_main();
void exalt_eth_ethernets_free();
void exalt_eth_free(void* data);

void exalt_eth_up(exalt_ethernet* eth);
void exalt_eth_down(exalt_ethernet* eth);
short exalt_eth_is_up(exalt_ethernet * eth);

short exalt_eth_is_ethernet(char* name);


void exalt_eth_printf();

Ecore_List* exalt_eth_get_list();
exalt_ethernet* exalt_eth_get_ethernet_byname(char* name);
exalt_ethernet* exalt_eth_get_ethernet_bypos(int pos);
exalt_ethernet* exalt_eth_get_ethernet_byudi(char* udi);
exalt_ethernet* exalt_eth_get_ethernet_byifindex(int ifindex);


short exalt_eth_is_link(exalt_ethernet *eth);
char* exalt_eth_get_name(exalt_ethernet* eth);
char* exalt_eth_get_ip(exalt_ethernet* eth);
char* exalt_eth_get_netmask(exalt_ethernet* eth);
char* exalt_eth_get_gateway(exalt_ethernet* eth);
char* exalt_eth_get_udi(exalt_ethernet* eth);
int exalt_eth_get_ifindex(exalt_ethernet* eth);

short exalt_eth_is_dhcp(exalt_ethernet * eth);
short exalt_eth_is_new_dhcp(exalt_ethernet * eth);

short exalt_eth_is_wireless(exalt_ethernet* eth);
exalt_wireless* exalt_eth_get_wireless(exalt_ethernet* eth);

int exalt_eth_set_cb(Exalt_Eth_Cb fct, void* user_data;);
int exalt_eth_set_scan_cb(Exalt_Wifi_Scan_Cb fct, void* user_data);

int exalt_eth_set_new_ip(exalt_ethernet* eth,const char* ip);
int exalt_eth_set_new_netmask(exalt_ethernet* eth,const char* netmask);
int exalt_eth_set_new_gateway(exalt_ethernet* eth,const char* gateway);
int exalt_eth_set_dhcp(exalt_ethernet* eth, short dhcp);


int exalt_eth_apply_conf(exalt_ethernet* eth);
int exalt_eth_apply_gateway(exalt_ethernet *eth);
int exalt_eth_apply_dhcp(exalt_ethernet* eth);
int exalt_eth_apply_static(exalt_ethernet *eth);

char* exalt_eth_get_new_ip(exalt_ethernet* eth);
char* exalt_eth_get_new_gateway(exalt_ethernet* eth);
char* exalt_eth_get_new_netmask(exalt_ethernet* eth);


/** @} */

#endif


