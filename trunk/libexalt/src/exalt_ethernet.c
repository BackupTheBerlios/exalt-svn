/** @file exalt_ethernet.c */
#include "exalt_ethernet.h"
#include "libexalt_private.h"
#include "../config.h"

/**
 * Private functions headers
 */

short _exalt_eth_set_state(Exalt_Ethernet* eth, Exalt_Enum_State state);
const char* _exalt_eth_get_save_ip(Exalt_Ethernet* eth);
const char* _exalt_eth_get_save_gateway(Exalt_Ethernet* eth);
const char* _exalt_eth_get_save_netmask(Exalt_Ethernet* eth);
short _exalt_eth_get_save_link(Exalt_Ethernet* eth);
short _exalt_eth_get_save_up(Exalt_Ethernet* eth);

int _exalt_eth_set_save_ip(Exalt_Ethernet* eth,const char* ip);
int _exalt_eth_set_save_netmask(Exalt_Ethernet* eth,const char* netmask);
int _exalt_eth_set_save_gateway(Exalt_Ethernet* eth,const char* gateway);
int _exalt_eth_set_save_link(Exalt_Ethernet* eth,short link);
int _exalt_eth_set_save_up(Exalt_Ethernet* eth,short up);

int _exalt_eth_set_udi(Exalt_Ethernet* eth,const char* udi);
int _exalt_eth_set_ifindex(Exalt_Ethernet* eth,int ifindex);
int _exalt_eth_set_name(Exalt_Ethernet* eth,const char* name);

void _exalt_cb_is_net(void *user_data, void *reply_data, DBusError *error);

void _exalt_cb_net_properties(void *data, void *reply_data, DBusError *error);

int _exalt_eth_remove_udi(const char* udi);


int _exalt_apply_timer(void *data);
int _exalt_eth_apply_dhcp(Exalt_Ethernet* eth);
int _exalt_eth_apply_static(Exalt_Ethernet *eth);



/**
 * @addtogroup Exalt_Ethernet
 */

struct Exalt_Ethernet
{
    char* name; //eth0, eth1...
    char* udi;
    int ifindex;

    Exalt_Enum_State state;
    Exalt_Connection *connection;
    Exalt_Wireless* wireless; //if null, the interface is not wireless

    char* _save_ip;
    char* _save_netmask;
    char* _save_gateway;
    short _save_link;
    short _save_up;

    pid_t apply_pid;
    Ecore_Timer *apply_timer;

    short new_up;
};


/*
 * Constructor / Destructor
 */



/**
 * @brief create a Exalt_Ethernet structure
 * @param name the name of the card (eth0, ath3 ...)
 * @return Return a new Exalt_Ethernet structure
 */
Exalt_Ethernet* exalt_eth_new(const char* name)
{
    struct iwreq wrq;
    Exalt_Ethernet* eth;

    eth = (Exalt_Ethernet*)malloc((unsigned int)sizeof(Exalt_Ethernet));
    EXALT_ASSERT_QUIT(eth!=NULL);

    eth->state = EXALT_DOWN;
    eth->connection = NULL;

    eth->name = NULL;
    _exalt_eth_set_name(eth,name);
    eth->udi =  NULL;
    eth->ifindex = 0;
    eth->_save_ip = NULL;
    eth->_save_netmask = NULL;
    eth->_save_gateway = NULL;
    eth-> _save_link = 0;
    eth->_save_up = 0;
    eth->wireless = NULL;

    strncpy(wrq.ifr_name, exalt_eth_get_name(eth), sizeof(wrq.ifr_name));
    if(exalt_ioctl(&wrq, SIOCGIWNAME))
        eth->wireless = exalt_wireless_new(eth);

    //now update the state
    if(exalt_eth_is_up(eth))
        _exalt_eth_set_state(eth,EXALT_UP);

    return eth;
}



/**
 * @brief free the "library" (exalt_eth_interfaces)
 */
void exalt_eth_ethernets_free()
{
    e_dbus_connection_close(exalt_eth_interfaces.dbus_conn);
    ecore_list_destroy(exalt_eth_interfaces.ethernets);
}



/**
 * @brief free a Exalt_Ethernet structure
 * @param data a Exalt_Ethernet* structure
 */
void exalt_eth_free(void *data)
{
    Exalt_Ethernet* eth = Exalt_Ethernet(data);
    EXALT_FREE(eth->name);
    EXALT_FREE(eth->udi);
    EXALT_FREE(eth->_save_ip);
    EXALT_FREE(eth->_save_netmask);
    EXALT_FREE(eth->_save_gateway);
    if(eth->connection)
        exalt_conn_free(eth->connection);
    if(exalt_eth_is_wireless(eth)) exalt_wireless_free(exalt_eth_get_wireless(eth));
    EXALT_FREE(eth);
}

/*
 * Load ethernet informations
 */


/**
 * test if a hal device is a net device
 */
void _exalt_cb_is_net(void *user_data, void *reply_data, DBusError *error)
{
    char *udi = user_data;
    E_Hal_Device_Query_Capability_Return *ret = reply_data;
    int *action = malloc(sizeof(int));
    *action = EXALT_ETH_CB_ACTION_ADD;


    EXALT_ASSERT_QUIT_VOID(!dbus_error_is_set(error));

    if (ret && ret->boolean)
        e_hal_device_get_all_properties(exalt_eth_interfaces.dbus_conn, udi, _exalt_cb_net_properties, action);
}

/**
 * load the property of a net device from hall
 */
void _exalt_cb_net_properties(void *data, void *reply_data, DBusError *error)
{
    int action = *((int*)data);
    E_Hal_Properties *ret = reply_data;
    int err = 0;
    Exalt_Ethernet* eth;
    char* str;

    EXALT_ASSERT_QUIT_VOID(!dbus_error_is_set(error));

    str = e_hal_property_string_get(ret,"net.interface", &err);
    eth = exalt_eth_new(str);
    EXALT_FREE(str);

    str = e_hal_property_string_get(ret,"info.udi", &err);
    _exalt_eth_set_udi(eth,str);
    EXALT_FREE(str);

    _exalt_eth_set_ifindex(eth,e_hal_property_int_get(ret,"net.linux.ifindex", &err));

    str = exalt_eth_get_ip(eth);
    _exalt_eth_set_save_ip(eth,str);
    EXALT_FREE(str);

    str = exalt_eth_get_netmask(eth);
    _exalt_eth_set_save_netmask(eth,str);
    EXALT_FREE(str);

    str = exalt_eth_get_gateway(eth);
    _exalt_eth_set_save_gateway(eth,str);
    EXALT_FREE(str);

    _exalt_eth_set_save_link(eth, exalt_eth_is_link(eth));
    _exalt_eth_set_save_up(eth, exalt_eth_is_up(eth));

    //add the interface in the list
    ecore_list_append(exalt_eth_interfaces.ethernets,(void *)eth);

    if(exalt_eth_interfaces.eth_cb)
        exalt_eth_interfaces.eth_cb(eth,action,exalt_eth_interfaces.eth_cb_user_data);
}


/**
 * @brief load the net device list from Hal
 */
void _exalt_cb_find_device_by_capability_net(void *user_data, void *reply_data, DBusError *error)
{
    E_Hal_Manager_Find_Device_By_Capability_Return *ret = reply_data;
    char *device;
    int *action = malloc(sizeof(int));
    *action = EXALT_ETH_CB_ACTION_NEW;

    EXALT_ASSERT_QUIT_VOID(ret!=NULL);
    EXALT_ASSERT_QUIT_VOID(ret->strings!=NULL);
    EXALT_ASSERT_QUIT_VOID(!dbus_error_is_set(error));

    ecore_list_first_goto(ret->strings);
    while ((device = ecore_list_next(ret->strings)))
    {
        e_hal_device_get_all_properties(exalt_eth_interfaces.dbus_conn, device, _exalt_cb_net_properties, action);
    }

    //EXALT_FREE(action);
}

/**
 * @brief call when a new device is added
 */
void _exalt_cb_signal_device_added(void *data, DBusMessage *msg)
{
    DBusError err;
    char *udi;
    int ret;

    dbus_error_init(&err);
    dbus_message_get_args(msg, &err, DBUS_TYPE_STRING, &udi, DBUS_TYPE_INVALID);
    ret = e_hal_device_query_capability(exalt_eth_interfaces.dbus_conn, udi, "net", _exalt_cb_is_net, strdup(udi));
}

/**
 * @brief call when a device is removed
 */
void _exalt_cb_signal_device_removed(void *data, DBusMessage *msg)
{
    DBusError err;
    char *udi;

    dbus_error_init(&err);
    dbus_message_get_args(msg, &err, DBUS_TYPE_STRING, &udi, DBUS_TYPE_INVALID);
    _exalt_eth_remove_udi(udi);
}

/**
 * @brief test if an interface is an ethernet interface
 * @param name the name of the interface
 * @return Return 1 if yes else 0
 */
short exalt_eth_is_ethernet(char* name)
{
    struct ifreq ifr;

    EXALT_ASSERT_QUIT(name!=NULL);

    strncpy(ifr.ifr_name,name,sizeof(ifr.ifr_name));
    if(!exalt_ioctl(&ifr, SIOCGIFHWADDR))
        return -1;

    return ifr.ifr_hwaddr.sa_family == ARPHRD_ETHER;
}


/*
 * up / down a card
 */


/**
 * @brief up the card "eth"
 * @param eth the card
 */
void exalt_eth_up(Exalt_Ethernet* eth)
{
    struct ifreq ifr;

    EXALT_ASSERT_QUIT_VOID(eth!=NULL);

    strncpy(ifr.ifr_name,exalt_eth_get_name(eth),sizeof(ifr.ifr_name));

    if( !exalt_ioctl(&ifr, SIOCGIFFLAGS))
        return ;

    ifr.ifr_flags |= IFF_UP;
    if( !exalt_ioctl(&ifr, SIOCSIFFLAGS))
        return ;
}



/**
 * @brief down the card eth"
 * @param eth the card
 */
void exalt_eth_down(Exalt_Ethernet* eth)
{
    struct ifreq ifr;

    EXALT_ASSERT_QUIT_VOID(eth!=NULL);

    if(exalt_eth_is_wireless(eth))
        _exalt_wpa_stop(exalt_eth_get_wireless(eth));

    strncpy(ifr.ifr_name,exalt_eth_get_name(eth),sizeof(ifr.ifr_name));

    if( !exalt_ioctl(&ifr, SIOCGIFFLAGS))
        return ;

    ifr.ifr_flags &= ~IFF_UP;
    if( !exalt_ioctl(&ifr, SIOCSIFFLAGS))
        return ;
}



/*
 * get / set informations about cards
 */



/**
 * @brief get the interface ecore list
 * @return Return the ecore list
 */
Ecore_List* exalt_eth_get_list()
{
    return exalt_eth_interfaces.ethernets;
}




/**
 * @brief get a card by his position in the card list
 * @param pos the position
 * @return Returns the card
 */
Exalt_Ethernet* exalt_eth_get_ethernet_bypos(int pos)
{
    return Exalt_Ethernet(ecore_list_index_goto(exalt_eth_interfaces.ethernets,pos));
}



/**
 * @brief get a card by his name
 * @param name the name
 * @return Returns the card
 */
Exalt_Ethernet* exalt_eth_get_ethernet_byname(const char* name)
{
    void *data;
    Exalt_Ethernet* eth;

    EXALT_ASSERT_QUIT(name!=NULL);

    ecore_list_first_goto(exalt_eth_interfaces.ethernets);
    data = ecore_list_next(exalt_eth_interfaces.ethernets);
    while(data)
    {
        eth = Exalt_Ethernet(data);
        if(strcmp(exalt_eth_get_name(eth),name) == 0)
            return eth;

        data = ecore_list_next(exalt_eth_interfaces.ethernets);
    }

    return NULL;
}

/**
 * @brief get a card by his udi
 * @param udi the udi
 * @return Returns the card
 */
Exalt_Ethernet* exalt_eth_get_ethernet_byudi(const char* udi)
{
    void *data;
    Exalt_Ethernet* eth;

    EXALT_ASSERT_QUIT(udi!=NULL);

    ecore_list_first_goto(exalt_eth_interfaces.ethernets);
    data = ecore_list_next(exalt_eth_interfaces.ethernets);
    while(data)
    {
        eth = Exalt_Ethernet(data);
        if(strcmp(exalt_eth_get_udi(eth),udi) == 0)
            return eth;

        data = ecore_list_next(exalt_eth_interfaces.ethernets);
    }

    return NULL;
}

/**
 * @brief get a card by his ifindex
 * @param ifindex the ifindex
 * @return Returns the card
 */
Exalt_Ethernet* exalt_eth_get_ethernet_byifindex(int ifindex)
{
    void *data;
    Exalt_Ethernet* eth;

    ecore_list_first_goto(exalt_eth_interfaces.ethernets);
    data = ecore_list_next(exalt_eth_interfaces.ethernets);
    while(data)
    {
        eth = Exalt_Ethernet(data);
        if(ifindex == exalt_eth_get_ifindex(eth))
            return eth;

        data = ecore_list_next(exalt_eth_interfaces.ethernets);
    }

    return NULL;
}



/**
 * @brief test if an interface is link
 * @param eth the card
 * @return Return 1 if yes, else 0
 */
short exalt_eth_is_link(Exalt_Ethernet* eth)
{
    struct ifreq ifr;
    struct ethtool_value edata;
    int res;


    EXALT_ASSERT_QUIT(eth!=NULL);

    if(exalt_eth_is_wireless(eth))
        return 1;

    strncpy(ifr.ifr_name,exalt_eth_get_name(eth),sizeof(ifr.ifr_name));
    memset(&edata, 0, sizeof(edata));
    edata.cmd = ETHTOOL_GLINK;
    ifr.ifr_data = (caddr_t)&edata;

    if( !(res = exalt_ioctl(&ifr, SIOCETHTOOL)))
        return res; //if an interface doesn't support SIOCETHTOOL, we return true

    return edata.data ? 1 : 0;
}

/**
 * @brief get the name of the card "eth" (eth0, eth1 ...)
 * @param eth the card
 * @return Returns the name
 */
const char* exalt_eth_get_name(const Exalt_Ethernet* eth)
{
    EXALT_ASSERT_QUIT(eth!=NULL);

    return eth->name;
}

/**
 * @brief get the udi of the card "eth" (eth0, eth1 ...)
 * @param eth the card
 * @return Returns the udi
 */
const char* exalt_eth_get_udi(Exalt_Ethernet* eth)
{
    EXALT_ASSERT_QUIT(eth!=NULL);
    return eth->udi;
}
/**
 * @brief get the ifindex of the card "eth" (eth0, eth1 ...)
 * @param eth the card
 * @return Returns the ifindex
 */
int exalt_eth_get_ifindex(Exalt_Ethernet* eth)
{
    EXALT_ASSERT_QUIT(eth!=NULL);
    return eth->ifindex;
}

/**
 * @brief get the connection of the card "eth"
 * @param eth the card
 * @return Returns the connection
 */
Exalt_Connection* exalt_eth_get_connection(Exalt_Ethernet* eth)
{
    EXALT_ASSERT_QUIT(eth!=NULL);
    return eth->connection;
}

/**
 * @brief set the connection of the card "eth"
 * @param eth the card
 * @param c the connection
 * @return returns 1 if the connexion is apply, else 0
 */
short exalt_eth_set_connection(Exalt_Ethernet* eth, Exalt_Connection* c)
{
    EXALT_ASSERT_QUIT(eth!=NULL);
    EXALT_ASSERT_QUIT(c!=NULL);

    if(eth->connection)
        exalt_conn_free(eth->connection);
    eth->connection = c;
    return 1;
}

/**
 * @brief get the ip address of the card "eth"
 * @param eth the card
 * @return Returns the ip address
 */
char* exalt_eth_get_ip(const Exalt_Ethernet* eth)
{
    struct sockaddr_in sin = { AF_INET };
    struct ifreq ifr;

    EXALT_ASSERT_QUIT(eth!=NULL);

    strncpy(ifr.ifr_name,exalt_eth_get_name(eth),sizeof(ifr.ifr_name));
    ifr.ifr_addr = *(struct sockaddr *) &sin;

    if( !exalt_ioctl(&ifr, SIOCGIFADDR))
        return NULL;

    sin = *(struct sockaddr_in*) &ifr.ifr_addr;

    return strdup(inet_ntoa(sin.sin_addr));
}



/**
 * @brief get the netmask address of the card "eth"
 * @param eth the card
 * @return Returns the netmask address
 */
char* exalt_eth_get_netmask(Exalt_Ethernet* eth)
{
    struct sockaddr_in sin = { AF_INET };
    struct ifreq ifr;

    EXALT_ASSERT_QUIT(eth!=NULL);

    strncpy(ifr.ifr_name,exalt_eth_get_name(eth),sizeof(ifr.ifr_name));
    ifr.ifr_addr = *(struct sockaddr *) &sin;
    if( !exalt_ioctl(&ifr, SIOCGIFNETMASK) )
        return NULL;

    sin = *(struct sockaddr_in*) &ifr.ifr_addr;
    return strdup(inet_ntoa(sin.sin_addr));
}



/**
 * @brief get the default gateway address of the card "eth"
 * @param eth the card
 * @return Returns the gateway address
 */
char* exalt_eth_get_gateway(Exalt_Ethernet* eth)
{
    FILE* f;
    char buf[1024];
    char* fmt;
    int num, iflags, metric, refcnt, use, mss, window, irtt;
    char iface[16];
    char gate_addr[128], net_addr[128];
    char mask_addr[128];
    short find = 0;

    EXALT_ASSERT_QUIT(eth!=NULL);

    f = fopen(EXALT_PATH_ROUTE,"r");
    EXALT_ASSERT_QUIT(f!=NULL);
    fmt = proc_gen_fmt(EXALT_PATH_ROUTE, 0, f,
            "Iface", "%16s",
            "Destination", "%128s",
            "Gateway", "%128s",
            "Flags", "%X",
            "RefCnt", "%d",
            "Use", "%d",
            "Metric", "%d",
            "Mask", "%128s",
            "MTU", "%d",
            "Window", "%d",
            "IRTT", "%d",
            NULL);

    while(fgets(buf,1024,f) && !find)
    {
        num = sscanf(buf, fmt,
                iface, net_addr, gate_addr,
                &iflags, &refcnt, &use, &metric, mask_addr,
                &mss, &window, &irtt);
        if(strcmp(iface,exalt_eth_get_name(eth))==0 && strcmp(net_addr,"00000000")==0)
            find = 1;
    }
    fclose(f);
    EXALT_FREE(fmt);
    if(find)
        return exalt_addr_hexa_to_dec(gate_addr);
    else
        return NULL;
}


/**
 * @brief get if the card "eth" use DHCP or static (look the configuration file)
 * @param eth the card
 * @return Returns 1 if the card use DHCP, 0 if static, -1 if unknow
 */
short exalt_eth_is_dhcp(Exalt_Ethernet* eth)
{
    Exalt_Connection *c;
    EXALT_ASSERT_QUIT(eth!=NULL);

    if( (c=exalt_eth_get_connection(eth)))
        return exalt_conn_is_dhcp(c);
    else
        return -1;
}



/**
 * @brief get if the card is activated
 * @param eth the card
 * @return Returns 1 if the card is activated, else 0
 */
short exalt_eth_is_up(Exalt_Ethernet* eth)
{
    struct ifreq ifr;

    EXALT_ASSERT_QUIT(eth!=NULL);

    strncpy(ifr.ifr_name,exalt_eth_get_name(eth),sizeof(ifr.ifr_name));

    if( !exalt_ioctl(&ifr,SIOCGIFFLAGS) )
        return 0;

    if( (ifr.ifr_flags & IFF_UP))
        return 1;
    else if( !(ifr.ifr_flags & IFF_UP))
        return 0;
    else
        return 0;
}



/**
 * @brief get if the card "eth" is a wireless card
 * @param eth the card
 * @return Returns 1 if the card is a wireless card, else 0
 */
short exalt_eth_is_wireless(Exalt_Ethernet* eth)
{
    EXALT_ASSERT_QUIT(eth!=NULL);
    return eth->wireless != NULL;
}



/**
 * @brief get the wireless structure of the card "eth"
 * @param eth the card
 * @return Returns the wireless structure
 */
Exalt_Wireless* exalt_eth_get_wireless(Exalt_Ethernet* eth)
{
    EXALT_ASSERT_QUIT(eth!=NULL);
    return eth->wireless;
}




/**
 * @brief set the callback function
 * @param fct function call when we have a new or remove interface
 * @param user_data user data
 */
int exalt_eth_set_cb(Exalt_Eth_Cb fct, void * user_data)
{
    exalt_eth_interfaces.eth_cb = fct;
    exalt_eth_interfaces.eth_cb_user_data = user_data;
    return 1;
}



/**
 * @brief set the callback scan function
 * @param fct function call when we have a new or old wireless network
 * @param user_data user data
 */
int exalt_eth_set_scan_cb(Exalt_Wifi_Scan_Cb fct, void * user_data)
{
    exalt_eth_interfaces.wireless_scan_cb = fct;
    exalt_eth_interfaces.wireless_scan_cb_user_data = user_data;
    return 1;
}




/*
 * Apply the current configuration
 */



/**
 * @brief apply the connection for the card "eth"
 * @param eth the card
 * @param c the connection
 * @return Returns 1 if the configuration is apply, else 0
 */
int exalt_eth_apply_conn(Exalt_Ethernet* eth, Exalt_Connection *c)
{
    int res;
    struct rtentry rt;
    struct sockaddr_in sin = { AF_INET };

    EXALT_ASSERT_QUIT(eth!=NULL);
    EXALT_ASSERT_ADV(exalt_conn_is_valid(c),eth->apply_pid = -1; _exalt_apply_timer(eth), "exalt_conn_is_valid(c) failed");

    //apply start
    if(exalt_eth_interfaces.eth_cb)
        exalt_eth_interfaces.eth_cb(eth,EXALT_ETH_CB_ACTION_CONN_APPLY_START,exalt_eth_interfaces.eth_cb_user_data);

    exalt_eth_set_connection(eth,c);

    eth->apply_pid = fork();
    EXALT_ASSERT_ADV(eth->apply_pid>=0,eth->apply_pid = -1; _exalt_apply_timer(eth), "exalt_conn_is_valid(c) failed");
    if(eth->apply_pid == 0)
    {
        if(exalt_eth_is_wireless(eth))
            exalt_wireless_apply_conn(exalt_eth_get_wireless(eth));

        //remove old gateway
        if(exalt_conn_is_dhcp(c) || exalt_is_address(exalt_conn_get_gateway(c)))
        {
            memset((char *) &rt, 0, sizeof(struct rtentry));
            rt.rt_flags = ( RTF_UP | RTF_GATEWAY );
            sin.sin_addr.s_addr = inet_addr ("0.0.0.0");
            rt.rt_dst = *(struct sockaddr *) &sin;

            while (exalt_ioctl(&rt, SIOCDELRT))
                usleep(100);
        }
        if(exalt_conn_is_dhcp(c))
            res = _exalt_eth_apply_dhcp(eth);
        else
            res = _exalt_eth_apply_static(eth);

        exit(res);
    }

    eth->apply_timer = ecore_timer_add(1 ,_exalt_apply_timer,eth);

    return 1;
}


/*
 * Others
 */


/**
 * @brief print card informations in the standard output
 */
void exalt_eth_printf()
{
    void *data;
    Exalt_Ethernet* eth;

    ecore_list_first_goto(exalt_eth_interfaces.ethernets);
    data = ecore_list_next(exalt_eth_interfaces.ethernets);
    while(data)
    {
        eth = Exalt_Ethernet(data);
        printf("###   %s   ###\n",eth->name);
        printf("Up: %d\n",exalt_eth_is_up(eth));
        if(exalt_eth_is_dhcp(eth))
            printf("-- DHCP mode --\n");

        printf("ip: %s\n",exalt_eth_get_ip(eth));
        printf("mask: %s\n",exalt_eth_get_netmask(eth));
        printf("gateway: %s\n",exalt_eth_get_gateway(eth));
        printf("Wifi: %s\n",(eth->wireless==NULL?"no":"yes"));
        if(eth->wireless!=NULL)
        {
            exalt_wireless_scan_wait(eth);
            Exalt_Wireless *w = exalt_eth_get_wireless(eth);
            exalt_wireless_printf(w);
        }
        data = ecore_list_next(exalt_eth_interfaces.ethernets);
    }
}




/** @} */



/**
 * Private functions bodies
 */

/**
 * @brief set the state of a card (need for save)
 * @param eth the card
 * @param state the new state
 * @return Returns 1 if the state is apply, else 0
 */
short _exalt_eth_set_state(Exalt_Ethernet* eth, Exalt_Enum_State state)
{
    EXALT_ASSERT_QUIT(eth!=NULL);
    eth->state = state;
    return 1;
}

/**
 * @brief get the state of a card (need for save)
 * @param eth the card
 * @return Returns the state
 */
Exalt_Enum_State _exalt_eth_get_state(Exalt_Ethernet* eth)
{
    EXALT_ASSERT_QUIT(eth!=NULL);
    return eth->state;
}

/**
 * @brief get the save ip address of the card "eth"
 * @param eth the card
 * @return Returns the ip address
 */
const char* _exalt_eth_get_save_ip(Exalt_Ethernet* eth)
{
    EXALT_ASSERT_QUIT(eth!=NULL);
    return eth->_save_ip;
}

/**
 * @brief get the save default gateway of the card "eth"
 * @param eth the card
 * @return Returns the gateway
 */
const char* _exalt_eth_get_save_gateway(Exalt_Ethernet* eth)
{
    EXALT_ASSERT_QUIT(eth!=NULL);
    return eth->_save_gateway;
}

/**
 * @brief get the save netmask of the card "eth"
 * @param eth the card
 * @return Returns the netmask
 */
const char* _exalt_eth_get_save_netmask(Exalt_Ethernet* eth)
{
    EXALT_ASSERT_QUIT(eth!=NULL);
    return eth->_save_netmask;
}

/**
 * @brief get the save link stat of the card "eth"
 * @param eth the card
 * @return Returns 1 or 0
 */
short _exalt_eth_get_save_link(Exalt_Ethernet* eth)
{
    EXALT_ASSERT_QUIT(eth!=NULL);
    return eth->_save_link;
}

/**
 * @brief get the save up stat of the card "eth"
 * @param eth the card
 * @return Returns 1 or 0
 */
short _exalt_eth_get_save_up(Exalt_Ethernet* eth)
{
    EXALT_ASSERT_QUIT(eth!=NULL);
    return eth->_save_up;
}


/**
 * @brief set the save ip address of the card "eth"
 * @param eth the card
 * @param ip the new ip address
 * @return Returns 1 if the save ip address is apply, 0 if the ip address doesn't have a correct format else -1
 */
int _exalt_eth_set_save_ip(Exalt_Ethernet* eth,const char* ip)
{
    EXALT_ASSERT_QUIT(eth!=NULL);
    EXALT_ASSERT_QUIT(exalt_is_address(ip) || ip==NULL);

    EXALT_FREE(eth->_save_ip);
    if(ip)
        eth->_save_ip=strdup(ip);
    return 1;
}

/**
 * @brief set the save netmask of the card "eth"
 * @param eth the card
 * @param ip the new netmask
 * @return Returns 1 if the netmask is apply, 0 if the netmask doesn't have a correct format else -1
 */
int _exalt_eth_set_save_netmask(Exalt_Ethernet* eth,const char* netmask)
{
    EXALT_ASSERT_QUIT(eth!=NULL);
    EXALT_ASSERT_QUIT(exalt_is_address(netmask) || netmask==NULL);

    EXALT_FREE(eth->_save_netmask);
    if(netmask)
        eth->_save_netmask=strdup(netmask);
    return 1;
}

/**
 * @brief set the save gateway of the card "eth"
 * @param eth the card
 * @param ip the new gateway
 * @return Returns 1 if the new gateway is apply, else 0
 */
int _exalt_eth_set_save_gateway(Exalt_Ethernet* eth,const char* gateway)
{
    EXALT_ASSERT_QUIT(eth!=NULL);
    EXALT_ASSERT_QUIT(exalt_is_address(gateway) || gateway==NULL);

    EXALT_FREE(eth->_save_gateway);
    if(gateway)
        eth->_save_gateway=strdup(gateway);
    return 1;
}

/**
 * @brief set the save link state of the card "eth"
 * @param eth the card
 * @param link the state link (1 or 0)
 * @return Returns 1 if the new stat is apply,else -1
 */
int _exalt_eth_set_save_link(Exalt_Ethernet* eth,short link)
{
    EXALT_ASSERT_QUIT(eth!=NULL);
    eth->_save_link=link;
    return 1;
}
/**
 * @brief set the save up state of the card "eth"
 * @param eth the card
 * @param ip the up state (1 or 0)
 * @return Returns 1 if the new state is apply,else -1
 */
int _exalt_eth_set_save_up(Exalt_Ethernet* eth,short up)
{
    EXALT_ASSERT_QUIT(eth!=NULL);
    eth->_save_up=up;
    return 1;
}


/**
 * @brief set the name of the card "eth"
 * @param eth the card
 * @param name the new name
 * @return Returns 1 if the new name is apply, else 0
 */
int _exalt_eth_set_name(Exalt_Ethernet* eth, const char* name)
{
    EXALT_ASSERT_QUIT(eth!=NULL);
    EXALT_ASSERT_QUIT(name!=NULL);

    EXALT_FREE(eth->name);
    eth->name=strdup(name);
    return 1;
}


/**
 * @brief set the udi of the card "eth"
 * @param eth the card
 * @param udi the new udi
 * @return Returns 1 if udi is apply, else -1
 */
int _exalt_eth_set_udi(Exalt_Ethernet* eth,const char* udi)
{
    EXALT_ASSERT_QUIT(eth!=NULL);
    EXALT_ASSERT_QUIT(udi!=NULL);

    EXALT_FREE(eth->udi);
    eth->udi=strdup(udi);
    return 1;
}

/**
 * @brief set the ifindex of the card "eth"
 * @param eth the card
 * @param ifindex the ifindex
 * @return Returns 1 if the new ifindex is apply,else -1
 */
int _exalt_eth_set_ifindex(Exalt_Ethernet* eth,int ifindex)
{
    EXALT_ASSERT_QUIT(eth!=NULL);
    eth->ifindex=ifindex;
    return 1;
}


/**
 * @brief remove an interface by his udi
 * @param udi the udi
 * @return Returns 1 if the card is remove, else -1
 */
int _exalt_eth_remove_udi(const char* udi)
{
    Ecore_List* l = exalt_eth_interfaces.ethernets;
    void* data;

    ecore_list_first_goto(l);
    data = ecore_list_next(l);
    while(data)
    {
        Exalt_Ethernet* eth = Exalt_Ethernet(data);
        if(strcmp(exalt_eth_get_udi(eth),udi)==0)
        {
            if(exalt_eth_interfaces.eth_cb)
                exalt_eth_interfaces.eth_cb(eth,EXALT_ETH_CB_ACTION_REMOVE,exalt_eth_interfaces.eth_cb_user_data);

            ecore_list_index_goto(l, ecore_list_index(l)-1);
            ecore_list_remove_destroy(l);
            return 1;
        }
        else
            data = ecore_list_next(l);
    }
    return -1;
}


/**
 * @brief called when the kernel send an piece of information
 */
int _exalt_rtlink_watch_cb(void *data, Ecore_Fd_Handler *fd_handler)
{
    int fd;
    ssize_t bin;
    struct sockaddr_nl addr;
    unsigned short rtlen;
    char buf[4096];
    struct ifinfomsg *ifmsg;
    struct ifaddrmsg *ifaddr;
    struct rtmsg *ifroute;
    struct nlmsghdr *nh;
    struct iovec iov = { buf, sizeof(buf) };
    struct msghdr msg = { (void *)&addr, sizeof(addr), &iov, 1, NULL, 0, 0 };
    Exalt_Ethernet* eth;
    void* data_l;
    Ecore_List* l;
    char* str;
    const char* str2;


    fd = exalt_eth_interfaces.rtlink_sock;

    bin = recvmsg(fd, &msg, 0);
    EXALT_ASSERT_QUIT(bin>=0);

    for(nh = (struct nlmsghdr *)buf; NLMSG_OK(nh, bin);
            nh = NLMSG_NEXT(nh, bin))
    {
        switch(nh->nlmsg_type)
        {
            case RTM_NEWLINK:
            case RTM_DELLINK:
                ifmsg = NLMSG_DATA(nh);
                rtlen = RTM_PAYLOAD(nh);
                EXALT_ASSERT_ADV(rtlen>=sizeof(struct ifinfomsg),break,
                        "Bad rtnetlink packet (%d < %d)", rtlen,
                        sizeof(struct ifinfomsg));

                //printf("ifmsg: { %d, %d, %d, 0x%x, 0x%x }\n",
                //        ifmsg->ifi_family, ifmsg->ifi_type,
                //        ifmsg->ifi_index, ifmsg->ifi_flags,
                //        ifmsg->ifi_change);

                eth = exalt_eth_get_ethernet_byifindex(ifmsg->ifi_index);
                if(!eth)
                    break;

                if(_exalt_eth_get_save_up(eth) != exalt_eth_is_up(eth))
                {
                    _exalt_eth_set_save_up(eth, exalt_eth_is_up(eth));
                    _exalt_eth_set_state(eth,exalt_eth_is_up(eth));
                    if(exalt_eth_is_up(eth) && exalt_eth_interfaces.eth_cb)
                        exalt_eth_interfaces.eth_cb(eth,EXALT_ETH_CB_ACTION_UP,exalt_eth_interfaces.eth_cb_user_data);
                    else if(exalt_eth_interfaces.eth_cb)
                        exalt_eth_interfaces.eth_cb(eth,EXALT_ETH_CB_ACTION_DOWN,exalt_eth_interfaces.eth_cb_user_data);
                }

                if(_exalt_eth_get_save_link(eth) != exalt_eth_is_link(eth))
                {
                    _exalt_eth_set_save_link(eth, exalt_eth_is_link(eth));

                    if(exalt_eth_is_link(eth) && exalt_eth_interfaces.eth_cb)
                        exalt_eth_interfaces.eth_cb(eth,EXALT_ETH_CB_ACTION_LINK,exalt_eth_interfaces.eth_cb_user_data);
                    else if(exalt_eth_interfaces.eth_cb)
                        exalt_eth_interfaces.eth_cb(eth,EXALT_ETH_CB_ACTION_UNLINK,exalt_eth_interfaces.eth_cb_user_data);
                }

                if(exalt_eth_is_wireless(eth))
                {
                    _exalt_rtlink_essid_change(exalt_eth_get_wireless(eth));
                }
                break;
            case RTM_NEWADDR:
            case RTM_DELADDR:
                ifaddr = NLMSG_DATA(nh);
                rtlen = RTM_PAYLOAD(nh);
                EXALT_ASSERT_ADV(rtlen>=sizeof(struct ifaddrmsg),break,
                        "Bad rtnetlink packet (%d < %d)", rtlen,
                        sizeof(struct ifaddrmsg));

                eth = exalt_eth_get_ethernet_byifindex(ifaddr->ifa_index);
                if(!eth)
                    break;

                str = exalt_eth_get_ip(eth);
                str2 = _exalt_eth_get_save_ip(eth);
                if((!str && str2)
                        || (str && !str2)
                        || (str && str2 && strcmp(str2,str) != 0))
                {
                    _exalt_eth_set_save_ip(eth, str);
                    if(exalt_eth_interfaces.eth_cb)
                        exalt_eth_interfaces.eth_cb(eth,EXALT_ETH_CB_ACTION_ADDRESS_NEW,exalt_eth_interfaces.eth_cb_user_data);
                }
                EXALT_FREE(str);

                str = exalt_eth_get_netmask(eth);
                str2 = _exalt_eth_get_save_netmask(eth);
                if((!str && str2)
                        || (str && !str2)
                        || (str && str2 && strcmp(str2,str) != 0))
                {
                    _exalt_eth_set_save_netmask(eth, str);
                    if(exalt_eth_interfaces.eth_cb)
                        exalt_eth_interfaces.eth_cb(eth,EXALT_ETH_CB_ACTION_NETMASK_NEW,exalt_eth_interfaces.eth_cb_user_data);
                }
                EXALT_FREE(str);
            case RTM_NEWROUTE:
            case RTM_DELROUTE:
                ifroute = NLMSG_DATA(nh);
                rtlen = RTM_PAYLOAD(nh);
                EXALT_ASSERT_ADV(rtlen>=sizeof(struct rtmsg),break,
                        "Bad rtnetlink packet (%d < %d)", rtlen,
                        sizeof(struct rtmsg));

                //test for each interface if the route has been modified
                l = exalt_eth_get_list();
                ecore_list_first_goto(l);
                data_l = ecore_list_next(l);
                while(data_l)
                {
                    eth = Exalt_Ethernet(data_l);
                    str = exalt_eth_get_gateway(eth);
                    str2 = _exalt_eth_get_save_gateway(eth);
                    if((!str && str2)
                            || (str && !str2)
                            || (str && str2 &&strcmp(str2,str ) != 0))
                    {
                        _exalt_eth_set_save_gateway(eth, str);
                        if(exalt_eth_interfaces.eth_cb)
                            exalt_eth_interfaces.eth_cb(eth,EXALT_ETH_CB_ACTION_GATEWAY_NEW,exalt_eth_interfaces.eth_cb_user_data);
                    }
                    EXALT_FREE(str);
                    data_l = ecore_list_next(l);
                }
                break;
            default: printf("hd cb default!\n");break;
        }
    }
    return 1;
}

int _exalt_apply_timer(void *data)
{
    Exalt_Ethernet* eth = Exalt_Ethernet(data);
    int res;
    int status;

    res = waitpid(eth->apply_pid,&status,WNOHANG);
    if(eth->apply_pid!=-1 && res == 0)
    {
        return 1;
    }

    //apply done
    if(exalt_eth_interfaces.eth_cb)
        exalt_eth_interfaces.eth_cb(eth,EXALT_ETH_CB_ACTION_CONN_APPLY_DONE,exalt_eth_interfaces.eth_cb_user_data);

    return 0;
}


/**
 * @brief apply static address for the card "eth"
 * @param eth the card
 * @return Returns 1 if static address are apply, else 0
 */
int _exalt_eth_apply_static(Exalt_Ethernet *eth)
{
    struct sockaddr_in sin = { AF_INET };
    struct ifreq ifr;
    struct rtentry rt;
    Exalt_Connection *c;

    EXALT_ASSERT_QUIT(eth!=NULL);
    c = exalt_eth_get_connection(eth);
    EXALT_ASSERT_QUIT(c!=NULL);

    strncpy(ifr.ifr_name,exalt_eth_get_name(eth),sizeof(ifr.ifr_name));

    //apply the ip
    sin.sin_addr.s_addr = inet_addr (exalt_conn_get_ip(c));
    ifr.ifr_addr = *(struct sockaddr *) &sin;
    if( !exalt_ioctl(&ifr, SIOCSIFADDR) )
        return -1;

    //apply the netmask
    sin.sin_addr.s_addr = inet_addr (exalt_conn_get_netmask(c));
    ifr.ifr_addr = *(struct sockaddr *) &sin;
    if( !exalt_ioctl(&ifr, SIOCSIFNETMASK ) )
        return -1;


    if(!exalt_conn_get_gateway(c))
        return 1;

    //apply the default gateway
    memset((char *) &rt, 0, sizeof(struct rtentry));
    rt.rt_flags = ( RTF_UP | RTF_GATEWAY );
    sin.sin_addr.s_addr = inet_addr (exalt_conn_get_gateway(c));
    rt.rt_gateway = *(struct sockaddr *) &sin;
    sin.sin_addr.s_addr = inet_addr ("0.0.0.0");
    rt.rt_dst = *(struct sockaddr *) &sin;
    rt.rt_metric = 2001;
    rt.rt_dev = strdup(exalt_eth_get_name(eth));

    if ( !exalt_ioctl(&rt, SIOCADDRT))
        return -1;

    EXALT_FREE(rt.rt_dev);
    return 1;
}



/**
 * @brief apply the dhcp mode for the card "eth"
 * @param eth the card
 * @return Returns 1 if the dhcp is apply, else 0
 */
int _exalt_eth_apply_dhcp(Exalt_Ethernet* eth)
{
#ifdef HAVE_DHCP
    Ecore_Exe * exe;
    int status;
    char command[1024];
    FILE* f;
    char buf[1024];
    pid_t pid;

    EXALT_ASSERT_QUIT(eth!=NULL);

    sprintf(command,COMMAND_DHCLIENT,exalt_eth_get_name(eth));
    exe = ecore_exe_run(command, NULL);
    waitpid(ecore_exe_pid_get(exe), &status, 0);
    ecore_exe_free(exe);

    //dhclient create a background process, we don't need it, we kill it.
    f = fopen(DHCLIENT_PID_FILE,"r");
    EXALT_ASSERT_QUIT(f!=NULL);
    fgets(buf,1024,f);
    pid = atoi(buf);
    kill(pid,SIGKILL);
    fclose(f);

    return 1;
#else
    EXALT_ASSERT_ADV(0,return 0,"Your build of libexalt doesn't support dhcp");
#endif
}



