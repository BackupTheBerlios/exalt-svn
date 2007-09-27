/** @file exalt_ethernet.h */
#ifndef EXALT_ETHERNET_H
#define EXALT_ETHERNET_H

/**
 * @brief informations about a card
 * @structinfo
 */
typedef struct Exalt_Ethernet Exalt_Ethernet;

/**
 * @defgroup Exalt_Ethernet
 * @brief the Exalt_Ethernet struct contains all informations about a card.
 * @{
 */

/**
 * @brief all cards
 * @structinfo
 */
typedef struct Exalt_Ethernets Exalt_Ethernets;

#include "libexalt.h"
#include "exalt_wireless.h"
#include "exalt_wireless_info.h"

#include <Ecore_Data.h>
#include <Ecore.h>
#include <E_Hal.h>
#include <sys/socket.h>

#include <linux/types.h>
#include <linux/ethtool.h>
#include <linux/netlink.h>
#include <linux/rtnetlink.h>

#include <sys/wait.h>

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
#define exalt_wireless_CB_ACTION_ESSIDCHANGE 7

/** when we have a new address */
#define EXALT_ETH_CB_ACTION_ADDRESS_NEW 9
/** when we have a new netmask */
#define EXALT_ETH_CB_ACTION_NETMASK_NEW 10
/** when we have a new gateway */
#define EXALT_ETH_CB_ACTION_GATEWAY_NEW 11

/** cast into an Exalt_Ethernet* struct */
#define Exalt_Ethernet(a) (Exalt_Ethernet*)a

/** callback function used for notifications when a new card is add, new essid ... */
typedef void (*Exalt_Eth_Cb) (Exalt_Ethernet* eth, int action, void* user_data);

extern Exalt_Ethernets exalt_eth_interfaces;

/** callback function used for notifications during a scan */
typedef void (*Exalt_Wifi_Scan_Cb) (Exalt_Ethernet* eth, Ecore_List* new_networks, Ecore_List* old_networks, void* user_data);

/** callback function used for notify than the configuration is applied */
typedef void (*Exalt_Conf_Applied) (Exalt_Ethernet* eth, void* data);


Exalt_Ethernet* exalt_eth_create(const char* name);

int exalt_eth_init();
int exalt_main();
void exalt_eth_ethernets_free();
void exalt_eth_free(void* data);

void exalt_eth_up(Exalt_Ethernet* eth);
void exalt_eth_down(Exalt_Ethernet* eth);
short exalt_eth_is_up(Exalt_Ethernet * eth);

short exalt_eth_is_ethernet(char* name);


void exalt_eth_printf();

Ecore_List* exalt_eth_get_list();
Exalt_Ethernet* exalt_eth_get_ethernet_byname(char* name);
Exalt_Ethernet* exalt_eth_get_ethernet_bypos(int pos);
Exalt_Ethernet* exalt_eth_get_ethernet_byudi(char* udi);
Exalt_Ethernet* exalt_eth_get_ethernet_byifindex(int ifindex);


short exalt_eth_is_link(Exalt_Ethernet *eth);
const char* exalt_eth_get_name(Exalt_Ethernet* eth);
char* exalt_eth_get_ip(Exalt_Ethernet* eth);
char* exalt_eth_get_netmask(Exalt_Ethernet* eth);
char* exalt_eth_get_gateway(Exalt_Ethernet* eth);
const char* exalt_eth_get_udi(Exalt_Ethernet* eth);
int exalt_eth_get_ifindex(Exalt_Ethernet* eth);

short exalt_eth_is_dhcp(Exalt_Ethernet * eth);
short exalt_eth_is_new_dhcp(Exalt_Ethernet * eth);
short exalt_eth_is_new_up(Exalt_Ethernet * eth);

short exalt_eth_is_wireless(Exalt_Ethernet* eth);
Exalt_Wireless* exalt_eth_get_wireless(Exalt_Ethernet* eth);

int exalt_eth_set_cb(Exalt_Eth_Cb fct, void* user_data;);
int exalt_eth_set_scan_cb(Exalt_Wifi_Scan_Cb fct, void* user_data);

int exalt_eth_set_new_ip(Exalt_Ethernet* eth,const char* ip);
int exalt_eth_set_new_netmask(Exalt_Ethernet* eth,const char* netmask);
int exalt_eth_set_new_gateway(Exalt_Ethernet* eth,const char* gateway);
int exalt_eth_set_new_dhcp(Exalt_Ethernet* eth, short dhcp);
int exalt_eth_set_new_up(Exalt_Ethernet* eth, short up);


int exalt_eth_apply_conf(Exalt_Ethernet* eth, Exalt_Conf_Applied fct, void* user_data);

const char* exalt_eth_get_new_ip(Exalt_Ethernet* eth);
const char* exalt_eth_get_new_gateway(Exalt_Ethernet* eth);
const char* exalt_eth_get_new_netmask(Exalt_Ethernet* eth);

/** @} */

#endif


