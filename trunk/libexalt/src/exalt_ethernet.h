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
#include "exalt_connection.h"
#include <Ecore_Data.h>
#include <Ecore.h>
#include <E_Hal.h>
#include <sys/socket.h>

#include <linux/types.h>
#include <linux/ethtool.h>
#include <linux/netlink.h>
#include <linux/rtnetlink.h>

#include <sys/wait.h>

/** Define the list of notify action */
typedef enum Exalt_Enum_Action
{
    /** when we load the device list */
    EXALT_ETH_CB_ACTION_NEW ,
    /** when we have a new card */
    EXALT_ETH_CB_ACTION_ADD,
    /** when we have a remove card */
    EXALT_ETH_CB_ACTION_REMOVE,
    /** when a known card is up */
    EXALT_ETH_CB_ACTION_UP ,
    /** when a known card is down */
    EXALT_ETH_CB_ACTION_DOWN,
    /** when a card is link */
    EXALT_ETH_CB_ACTION_LINK ,
    /** when a card is unlink */
    EXALT_ETH_CB_ACTION_UNLINK,
    /** when an essid change */
    EXALT_WIRELESS_CB_ACTION_ESSIDCHANGE ,

    /** when we have a new address */
    EXALT_ETH_CB_ACTION_ADDRESS_NEW ,
    /** when we have a new netmask */
    EXALT_ETH_CB_ACTION_NETMASK_NEW ,
    /** when we have a new gateway */
    EXALT_ETH_CB_ACTION_GATEWAY_NEW
} Exalt_Enum_Action;

/** cast into an Exalt_Ethernet* struct */
#define Exalt_Ethernet(a) (Exalt_Ethernet*)a

/** callback function used for notifications when a new card is add, new essid ... */
typedef void (*Exalt_Eth_Cb) (Exalt_Ethernet* eth, Exalt_Enum_Action action, void* user_data);

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

short exalt_eth_is_wireless(Exalt_Ethernet* eth);
Exalt_Wireless* exalt_eth_get_wireless(Exalt_Ethernet* eth);

int exalt_eth_set_cb(Exalt_Eth_Cb fct, void* user_data;);
int exalt_eth_set_scan_cb(Exalt_Wifi_Scan_Cb fct, void* user_data);

int exalt_eth_apply_conn(Exalt_Ethernet* eth, Exalt_Connection* c,Exalt_Conf_Applied fct, void* user_data);
Exalt_Connection* exalt_eth_get_connection(Exalt_Ethernet* eth);
short exalt_eth_set_connection(Exalt_Ethernet* eth, Exalt_Connection* c);

/** @} */

#endif


