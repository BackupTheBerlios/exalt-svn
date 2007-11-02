/** @file libexalt.c */
#include "libexalt.h"
#include "libexalt_private.h"
#include "config.h"
#include <E_Hal.h>

/**
 * @addtogroup Exalt_General
 * @{
 */

Exalt_Ethernets exalt_eth_interfaces;


/**
 * @brief intialise the library
 */
int exalt_init()
{
    exalt_eth_interfaces.is_launch = 0;
    exalt_eth_interfaces.ethernets = ecore_list_new();
    exalt_eth_interfaces.ethernets->free_func =  ECORE_FREE_CB(exalt_eth_free);
    ecore_list_init(exalt_eth_interfaces.ethernets);

    exalt_eth_interfaces.eth_cb = NULL;
    exalt_eth_interfaces.eth_cb_user_data = NULL;

    exalt_eth_interfaces.wireless_scan_cb = NULL;
    exalt_eth_interfaces.wireless_scan_cb_user_data = NULL;

    exalt_eth_interfaces.we_version = iw_get_kernel_we_version();

    //test if we have the administrator right
    if(getuid()==0)
        exalt_eth_interfaces.admin = 1;
    else
        exalt_eth_interfaces.admin = 0;

    return 1;
}

/*
 * @brief load cards and watch events
 */
int exalt_main()
{
    int *fd = malloc(sizeof(int));
    struct sockaddr_nl addr;

    if(exalt_eth_interfaces.is_launch>0)
    {
        print_error("ERROR", __FILE__, __LINE__,__func__,"exalt_is launch") ;
        return -1;
    }

    e_dbus_init();
    exalt_eth_interfaces.dbus_conn = e_dbus_bus_get(DBUS_BUS_SYSTEM);
    if (!exalt_eth_interfaces.dbus_conn)
    {
        print_error("ERROR", __FILE__, __LINE__,__func__,"Error connecting to system bus. Is it running?");
        return -1;
    }
    e_hal_manager_find_device_by_capability(exalt_eth_interfaces.dbus_conn, "net", _exalt_cb_find_device_by_capability_net, NULL);
    e_dbus_signal_handler_add(exalt_eth_interfaces.dbus_conn, "org.freedesktop.Hal", "/org/freedesktop/Hal/Manager", "org.freedesktop.Hal.Manager", "DeviceAdded", _exalt_cb_signal_device_added, NULL);
    e_dbus_signal_handler_add(exalt_eth_interfaces.dbus_conn, "org.freedesktop.Hal", "/org/freedesktop/Hal/Manager", "org.freedesktop.Hal.Manager", "DeviceRemoved", _exalt_cb_signal_device_removed, NULL);


    /* set up a rtnetlink socket */
    memset(&addr, 0, sizeof(addr));
    addr.nl_family = AF_NETLINK;
    addr.nl_groups = RTMGRP_LINK | RTMGRP_IPV4_IFADDR | RTMGRP_IPV4_ROUTE | RTMGRP_NOTIFY;

    *fd = socket(AF_NETLINK, SOCK_RAW, NETLINK_ROUTE);
    if(*fd < 0) {
        perror("socket()");
        return -1;
    }

    if(bind(*fd, (struct sockaddr *)&addr, sizeof(addr)) < 0) {
        perror("bind()");
        return -1;
    }

    exalt_eth_interfaces.rtlink_watch = ecore_main_fd_handler_add(*fd, ECORE_FD_READ,_exalt_rtlink_watch_cb, fd,NULL,NULL);

    exalt_eth_interfaces.is_launch = 1;

    return 1;
}




/**
 * @brief test if a string is a correct adress
 * @param ip the string ip
 * return Return 1 if sucess, else 0
 */
short exalt_is_address(const char* ip)
{
    Exalt_Regex *r;
    short res;

    if(!ip) return 0;

    r = exalt_regex_new(ip,REGEXP_IS_IP,0);
    res = exalt_regex_execute(r);
    exalt_regex_free(&r);
    return res;
}

/**
 * @brief test if a string is a correct essid
 * @param essid the essid
 * @return Return 1 if success, else 0
 */
short exalt_is_essid(const char* essid)
{
    if(!essid) return 0;
    return strlen(essid);
}

/**
 * @brief test if a string is a correct key
 * @param key the key
 * @param encryption_mode the password mode (WEP, WPA ...)
 * @return Return 1 if success, else 0
 */
short exalt_is_key(const char* key, Exalt_Enum_Encryption_Mode encryption_mode)
{
    Exalt_Regex *r;
    short res;
    if(encryption_mode == EXALT_ENCRYPTION_NONE)
        return 1;

    if(!key)
        return -0;

    if(encryption_mode == EXALT_ENCRYPTION_WEP_ASCII)
    {
        if(strlen(key)>0)
            return 1;
        else
            return 0;
    }

    if(encryption_mode == EXALT_ENCRYPTION_WEP_HEXA)
    {
        r = exalt_regex_new(strdup(key), REGEXP_IS_WEP_HEXA, 0);
        res = exalt_regex_execute(r);
        exalt_regex_free(&r);
        return res;
    }

    if(encryption_mode == EXALT_ENCRYPTION_WPA_PSK_TKIP_ASCII
            || encryption_mode ==EXALT_ENCRYPTION_WPA_PSK_CCMP_ASCII
            || encryption_mode == EXALT_ENCRYPTION_WPA2_PSK_TKIP_ASCII
            || encryption_mode == EXALT_ENCRYPTION_WPA2_PSK_CCMP_ASCII)
    {
        if(strlen(key)>=8 && strlen(key)<=63)
            return 1;
        else
            return 0;
    }

    return 0;
}

/**
 * @brief return if we have the administrator right
 * @return Return EXALT_TRUE if yes, else EXALT_FALSE
 */
short exalt_is_admin()
{
    return exalt_eth_interfaces.admin;
}

/**
 * @brief return if libexalt is built with the support of wpa_supplicant
 * @return Return 1 if yes, else 0
 */
short exalt_wpasupplicant_is_support()
{
#ifdef WPA_SUPPLICANT_COMMAND_PATH
    return 1;
#else
    return 0;
#endif
}


/**
 * @brief return if libexalt is built with the support of dhcp
 * @return Return 1 if yes, else 0
 */
short exalt_dhcp_is_support()
{
#ifdef DHCP_COMMAND_PATH
    return 1;
#else
    return 0;
#endif
}
