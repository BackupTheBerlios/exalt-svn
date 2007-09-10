/** @file exalt_ethernet.h */
#ifndef ETHERNET_H
#define ETHERNET_H

/**
 * @brief informations about a card
 * @structinfo
 */
typedef struct exalt_ethernet exalt_ethernet;

/**
 * @defgroup Exalt_Ethernet
 * @brief the exalt_ethernet struct contains all informations about a card.
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
#define EXALT_WIRELESS_CB_ACTION_ESSIDCHANGE 7

/** when we have a new address */
#define EXALT_ETH_CB_ACTION_ADDRESS_NEW 9
/** when we have a new netmask */
#define EXALT_ETH_CB_ACTION_NETMASK_NEW 10
/** when we have a new gateway */
#define EXALT_ETH_CB_ACTION_GATEWAY_NEW 11

/** cast into an exalt_ethernet* struct */
#define EXALT_ETHERNET(a) (exalt_ethernet*)a

/** callback function used for notifications when a new card is add, new essid ... */
typedef void (*Exalt_Eth_Cb) (exalt_ethernet* eth, int action, void* user_data);
/** cast into an Exalt_Eth_Cb function */
#define EXALT_ETH_CB(a) (Exalt_Eth_Cb)a
extern Exalt_Ethernets exalt_eth_interfaces;

/** callback function used for notifications during a scan */
typedef void (*Exalt_Wifi_Scan_Cb) (exalt_wireless_info* wi, int action, void* user_data);
/** cas into a Exalt_Wifi_Scan_Cb function */
#define EXALT_WIRELESS_SCAN_CB(a) (Exalt_Wifi_Scan_Cb)a

/** callback function used for notify than the configuration is applied */
typedef void (*Exalt_Conf_Applied) (exalt_ethernet* eth, void* data);

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
int exalt_eth_set_new_dhcp(exalt_ethernet* eth, short dhcp);


int exalt_eth_apply_conf(exalt_ethernet* eth, Exalt_Conf_Applied fct, void* user_data);

char* exalt_eth_get_new_ip(exalt_ethernet* eth);
char* exalt_eth_get_new_gateway(exalt_ethernet* eth);
char* exalt_eth_get_new_netmask(exalt_ethernet* eth);

/** @} */

#endif


