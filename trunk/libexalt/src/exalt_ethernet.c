/** @file exalt_ethernet.c */
#include "exalt_ethernet.h"


/**
 * @addtogroup Exalt_ethernet
 */

/**
 * Private functions headers
 */

char* _exalt_eth_get_save_ip(exalt_ethernet* eth);
char* _exalt_eth_get_save_gateway(exalt_ethernet* eth);
char* _exalt_eth_get_save_netmask(exalt_ethernet* eth);
short _exalt_eth_get_save_link(exalt_ethernet* eth);
short _exalt_eth_get_save_up(exalt_ethernet* eth);

int _exalt_eth_set_save_ip(exalt_ethernet* eth,const char* ip);
int _exalt_eth_set_save_netmask(exalt_ethernet* eth,const char* netmask);
int _exalt_eth_set_save_gateway(exalt_ethernet* eth,const char* gateway);
int _exalt_eth_set_save_link(exalt_ethernet* eth,short link);
int _exalt_eth_set_save_up(exalt_ethernet* eth,short up);

int _exalt_eth_set_udi(exalt_ethernet* eth,const char* udi);
int _exalt_eth_set_ifindex(exalt_ethernet* eth,int ifindex);
int _exalt_eth_set_name(exalt_ethernet* eth,const char* name);

void _exalt_cb_is_net(void *user_data, void *reply_data, DBusError *error);
void _exalt_cb_find_device_by_capability_net(void *user_data, void *reply_data, DBusError *error);
void _exalt_cb_signal_device_added(void *data, DBusMessage *msg);
void _exalt_cb_signal_device_removed(void *data, DBusMessage *msg);

void _exalt_cb_net_properties(void *data, void *reply_data, DBusError *error);

int _exalt_eth_remove_udi(const char* udi);

int _exalt_rtlink_watch_cb(void *data, Ecore_Fd_Handler *fd_handler);



/*
 * Constructor / Destructor
 */


/**
 * @brief intialise the library
 */
int exalt_eth_init()
{
    if(ecore_config_init("econfig") != ECORE_CONFIG_ERR_SUCC)
    {
        fprintf(stderr,"exalt_eth_init(): error can't init ecore config\n");
        return -1;
    }

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
        exalt_eth_interfaces.admin = EXALT_TRUE;
    else
        exalt_eth_interfaces.admin = EXALT_FALSE;

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
        fprintf(stderr,"exalt_main(): exalt_is launch !\n") ;
        return -1;
    }

    e_dbus_init();
    exalt_eth_interfaces.dbus_conn = e_dbus_bus_get(DBUS_BUS_SYSTEM);
    if (!exalt_eth_interfaces.dbus_conn)
    {
        fprintf(stderr,"exalt_eth_init(): Error connecting to system bus. Is it running?\n");
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
 * @brief create a exalt_ethernet structure
 * @param name the name of the card (eth0, ath3 ...)
 * @return Return a new exalt_ethernet structure
 */
exalt_ethernet* exalt_eth_create(const char* name)
{
    struct iwreq wrq;
    exalt_ethernet* eth = (exalt_ethernet*)malloc((unsigned int)sizeof(exalt_ethernet));
    if(!eth)
    {
        fprintf(stderr,"exalt_eth_create(): eth==null, malloc error ! \n");
        return NULL;
    }

    eth->name = NULL;
    _exalt_eth_set_name(eth,name);
    eth->udi =  NULL;
    eth->ifindex = 0;

    eth->new_ip = NULL;
    eth->new_netmask = NULL;
    eth->wireless = NULL;
    eth->new_gateway = NULL;

    eth->_save_ip = NULL;
    eth->_save_netmask = NULL;
    eth->_save_gateway = NULL;
    eth-> _save_link = 0;
    eth->_save_up = 0;


    strncpy(wrq.ifr_name, exalt_eth_get_name(eth), sizeof(wrq.ifr_name));
    if(exalt_ioctl(&wrq, SIOCGIWNAME) >= 0)
        eth->wireless = exalt_wireless_create(eth);

    return eth;
}



/**
 * @brief free the "library" (exalt_eth_interfaces)
 */
void exalt_eth_ethernets_free()
{
    e_dbus_connection_unref(exalt_eth_interfaces.dbus_conn);
    ecore_list_destroy(exalt_eth_interfaces.ethernets);
}



/**
 * @brief free a exalt_ethernet structure
 * @param data a exalt_ethernet* structure
 */
void exalt_eth_free(void *data)
{
	exalt_ethernet* eth = EXALT_ETHERNET(data);
	EXALT_FREE(eth->name)
        EXALT_FREE(eth->udi)
	EXALT_FREE(eth->new_ip)
		EXALT_FREE(eth->new_netmask)
		EXALT_FREE(eth->new_gateway)
		if(exalt_eth_is_wireless(eth)) exalt_wireless_free(exalt_eth_get_wireless(eth));
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

    if (dbus_error_is_set(error))
    {
        fprintf(stderr,"_exalt_cb_is_net(): dbus error ! \n");
        dbus_error_free(error);
        return ;
    }

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
    exalt_ethernet* eth;

    if (dbus_error_is_set(error))
    {
        fprintf(stderr,"_exalt_cb_net_properties(): dbus error ! \n");
        return ;
    }

    eth = exalt_eth_create(e_hal_property_string_get(ret,"net.interface", &err));
    _exalt_eth_set_udi(eth,e_hal_property_string_get(ret,"info.udi", &err));
    _exalt_eth_set_ifindex(eth,e_hal_property_int_get(ret,"net.linux.ifindex", &err));

    _exalt_eth_set_save_ip(eth,exalt_eth_get_ip(eth));
    _exalt_eth_set_save_netmask(eth,exalt_eth_get_netmask(eth));
    _exalt_eth_set_save_gateway(eth,exalt_eth_get_gateway(eth));
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

    if (!ret || !ret->strings)
    {
        fprintf(stderr,"_exalt_cb_find_device_by_capability_net(): ret == null !\n");
        return ;
    }
    if (dbus_error_is_set(error))
    {
        fprintf(stderr,"_exalt_cb_find_device_by_capability_net(): dbus error\n");
        dbus_error_free(error);
        return ;
    }
    ecore_list_first_goto(ret->strings);
    while ((device = ecore_list_next(ret->strings)))
    {
        e_hal_device_get_all_properties(exalt_eth_interfaces.dbus_conn, device, _exalt_cb_net_properties, action);
    }
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
 * @brief test if a interface is a ethernet interface
 * @param name the name of the interface
 * @return Return 1 if yes else 0
 */
short exalt_eth_is_ethernet(char* name)
{
	struct ifreq ifr;

	if(!name)
	{
	 	fprintf(stderr,"exalt_eth_is_ethernet(): name==null ! \n");
		return -1;
	}

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
void exalt_eth_up(exalt_ethernet* eth)
{
	struct ifreq ifr;

	if(!eth)
	{
	 	fprintf(stderr,"exalt_eth_up(): eth==null ! \n");
		return ;
	}

 	strncpy(ifr.ifr_name,exalt_eth_get_name(eth),sizeof(ifr.ifr_name));

	if( !exalt_ioctl(&ifr, SIOCGIFFLAGS))
		return ;

 	ifr.ifr_flags |= IFF_UP;
	if( !exalt_ioctl(&ifr, SIOCSIFFLAGS))
		return ;

	//save the configuration
	exalt_sys_conf_save_autoload(eth);
}



/**
 * @brief down the card eth"
 * @param eth the card
 */
void exalt_eth_down(exalt_ethernet* eth)
{
    struct ifreq ifr;

    if(!eth)
    {
        fprintf(stderr,"exalt_eth_down(): eth==null ! \n");
        return ;
    }

    strncpy(ifr.ifr_name,exalt_eth_get_name(eth),sizeof(ifr.ifr_name));

    if( !exalt_ioctl(&ifr, SIOCGIFFLAGS))
        return ;

    ifr.ifr_flags &= ~IFF_UP;
    if( !exalt_ioctl(&ifr, SIOCSIFFLAGS))
        return ;

    //save the configuration
    exalt_sys_conf_save_autoload(eth);
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
exalt_ethernet* exalt_eth_get_ethernet_bypos(int pos)
{
	return EXALT_ETHERNET(ecore_list_index_goto(exalt_eth_interfaces.ethernets,pos));
}



/**
 * @brief get a card by his name
 * @param name the name
 * @return Returns the card
 */
exalt_ethernet* exalt_eth_get_ethernet_byname(char* name)
{
	void *data;
	exalt_ethernet* eth;

	if(!name)
	{
		fprintf(stderr,"exalt_eth_get_exalt_ethernet_byname(): name==null! \n");
		return NULL;
	}

 	ecore_list_first_goto(exalt_eth_interfaces.ethernets);
	data = ecore_list_next(exalt_eth_interfaces.ethernets);
	while(data)
	{
	 	eth = EXALT_ETHERNET(data);
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
exalt_ethernet* exalt_eth_get_ethernet_byudi(char* udi)
{
	void *data;
	exalt_ethernet* eth;

	if(!udi)
	{
		fprintf(stderr,"exalt_eth_get_exalt_ethernet_byudi(): udi==null! \n");
		return NULL;
	}

 	ecore_list_first_goto(exalt_eth_interfaces.ethernets);
	data = ecore_list_next(exalt_eth_interfaces.ethernets);
	while(data)
	{
	 	eth = EXALT_ETHERNET(data);
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
exalt_ethernet* exalt_eth_get_ethernet_byifindex(int ifindex)
{
	void *data;
	exalt_ethernet* eth;

 	ecore_list_first_goto(exalt_eth_interfaces.ethernets);
	data = ecore_list_next(exalt_eth_interfaces.ethernets);
	while(data)
	{
	 	eth = EXALT_ETHERNET(data);
		if(ifindex == exalt_eth_get_ifindex(eth))
			return eth;

		data = ecore_list_next(exalt_eth_interfaces.ethernets);
	}

	return NULL;
}



/**
 * @brief test if an interface is link
 * @param eth the card
 * @return Return EXALT_TRUE if yes, else EXALT_FALSE
 */
short exalt_eth_is_link(exalt_ethernet* eth)
{
    struct ifreq ifr;
    struct ethtool_value edata;
    int res;
    if(!eth)
    {
        fprintf(stderr,"exalt_eth_load_ip(): eth==null ! \n");
        return EXALT_ERROR;
    }

    if(exalt_eth_is_wireless(eth))
        return EXALT_TRUE;

    strncpy(ifr.ifr_name,exalt_eth_get_name(eth),sizeof(ifr.ifr_name));
    memset(&edata, 0, sizeof(edata));
    edata.cmd = ETHTOOL_GLINK;
    ifr.ifr_data = (caddr_t)&edata;

    if( (res = exalt_ioctl(&ifr, SIOCETHTOOL))<=-1)
        return res; //if an interface doesn't support SIOCETHTOOL, we return true

    return edata.data ? EXALT_TRUE : EXALT_FALSE;
}

/**
 * @brief get the name of the card "eth" (eth0, eth1 ...)
 * @param eth the card
 * @return Returns the name
 */
char* exalt_eth_get_name(exalt_ethernet* eth)
{
	if(eth)
		return eth->name;
	else
		return NULL;
}

/**
 * @brief get the udi of the card "eth" (eth0, eth1 ...)
 * @param eth the card
 * @return Returns the udi
 */
char* exalt_eth_get_udi(exalt_ethernet* eth)
{
    if(eth)
        return eth->udi;
    else
        return NULL;
}
/**
 * @brief get the ifindex of the card "eth" (eth0, eth1 ...)
 * @param eth the card
 * @return Returns the ifindex
 */
int exalt_eth_get_ifindex(exalt_ethernet* eth)
{
    if(eth)
        return eth->ifindex;
    else
        return -1;
}




/**
 * @brief get the ip address of the card "eth"
 * @param eth the card
 * @return Returns the ip address
 */
char* exalt_eth_get_ip(exalt_ethernet* eth)
{
    struct sockaddr_in sin = { AF_INET };
    struct ifreq ifr;

    if(!eth)
    {
        fprintf(stderr,"exalt_eth_load_ip(): eth==null ! \n");
        return NULL;
    }

    strncpy(ifr.ifr_name,exalt_eth_get_name(eth),sizeof(ifr.ifr_name));
    ifr.ifr_addr = *(struct sockaddr *) &sin;

    if( !exalt_ioctl(&ifr, SIOCGIFADDR))
        return NULL;

    sin = *(struct sockaddr_in*) &ifr.ifr_addr;

    return inet_ntoa(sin.sin_addr);
}



/**
 * @brief get the netmask address of the card "eth"
 * @param eth the card
 * @return Returns the netmask address
 */
char* exalt_eth_get_netmask(exalt_ethernet* eth)
{
    struct sockaddr_in sin = { AF_INET };
    struct ifreq ifr;

    if(!eth)
    {
        fprintf(stderr,"exalt_eth_load_netmask(): eth==null ! \n");
        return NULL;
    }

    strncpy(ifr.ifr_name,exalt_eth_get_name(eth),sizeof(ifr.ifr_name));
    ifr.ifr_addr = *(struct sockaddr *) &sin;
    if( !exalt_ioctl(&ifr, SIOCGIFNETMASK) )
        return NULL;

    sin = *(struct sockaddr_in*) &ifr.ifr_addr;
    return inet_ntoa(sin.sin_addr);
}



/**
 * @brief get the default gateway address of the card "eth"
 * @param eth the card
 * @return Returns the gateway address
 */
char* exalt_eth_get_gateway(exalt_ethernet* eth)
{
    FILE* f;
    char buf[1024];
    char* fmt;
    int num, iflags, metric, refcnt, use, mss, window, irtt;
    char iface[16];
    char gate_addr[128], net_addr[128];
    char mask_addr[128];
    short find = 0;

    if(!eth)
    {
        fprintf(stderr,"eth_load_gateway_byeth(): eth==null ! \n");
        return NULL;
    }

    f = fopen(EXALT_PATH_ROUTE,"r");
    if(!f)
    {
        fprintf(stderr,"eth_load_gateway_byeth(): failed to open: %s\n",EXALT_PATH_ROUTE);
        return NULL;
    }
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
    if(find)
        return strdup(exalt_addr_hexa_to_dec(gate_addr));
    else
        return "0.0.0.0";
}


/**
 * @brief get the new ip address of the card "eth"
 * @param eth the card
 * @return Returns the ip address
 */
char* exalt_eth_get_new_ip(exalt_ethernet* eth)
{
    if(!eth)
    {
        fprintf(stderr,"exalt_eth_get_new_ip(): eth==null ! \n");
        return NULL;
    }
    else
        return eth->new_ip;
}

/**
 * @brief get the new mask address of the card "eth"
 * @param eth the card
 * @return Returns the ip address
 */
char* exalt_eth_get_new_netmask(exalt_ethernet* eth)
{
    if(!eth)
    {
        fprintf(stderr,"exalt_eth_get_new_netmask(): eth==null ! \n");
        return NULL;
    }
    else
        return eth->new_netmask;
}

/**
 * @brief get the new gateway address of the card "eth"
 * @param eth the card
 * @return Returns the ip address
 */
char* exalt_eth_get_new_gateway(exalt_ethernet* eth)
{
    if(!eth)
    {
        fprintf(stderr,"exalt_eth_get_new_gateway(): eth==null ! \n");
        return NULL;
    }
    else
        return eth->new_gateway;
}


/**
 * @brief get if the card "eth" will uses DHCP or static
 * @param eth the card
 * @return Returns 1 if the card use DHCP, else 0
 */
short exalt_eth_is_new_dhcp(exalt_ethernet* eth)
{
	if(eth)
		return eth->new_dhcp_static == EXALT_DHCP;
	else
		return 0;
}


/**
 * @brief get if the card "eth" use DHCP or static (look the configuration file)
 * @param eth the card
 * @return Returns 1 if the card use DHCP, else 0
 */
short exalt_eth_is_dhcp(exalt_ethernet* eth)
{
	if(eth)
		return exalt_sys_conf_is_dhcp(eth);
	else
		return EXALT_ERROR;
}



/**
 * @brief get if the card is activated
 * @param eth the card
 * @return Returns 1 if the card is activated, else 0
 */
short exalt_eth_is_up(exalt_ethernet* eth)
{
	struct ifreq ifr;

	if(!eth)
	{
	 	fprintf(stderr,"exalt_eth_is_up(): eth==null ! \n");
		return -1;
	}

 	strncpy(ifr.ifr_name,exalt_eth_get_name(eth),sizeof(ifr.ifr_name));

        if( !exalt_ioctl(&ifr,SIOCGIFFLAGS) )
            return -1;

        if( (ifr.ifr_flags & IFF_UP))
 	 	return 1;
	else if( !(ifr.ifr_flags & IFF_UP))
	 	return 0;
        else
            return -1;
}



/**
 * @brief get if the card "eth" is a wireless card
 * @param eth the card
 * @return Returns 1 if the card is a wireless card, else 0
 */
short exalt_eth_is_wireless(exalt_ethernet* eth)
{
	if(eth)
	{
            return eth->wireless != NULL;
        }
	else
		return 0;
}



/**
 * @brief get the wireless structure of the card "eth"
 * @param eth the card
 * @return Returns the wireless structure
 */
exalt_wireless* exalt_eth_get_wireless(exalt_ethernet* eth)
{
	if(eth)
		return eth->wireless;
	else
		return NULL;
}



/**
 * @brief set the ip address of the card "eth"
 * @param eth the card
 * @param ip the new ip address
 * @return Returns 1 if the new ip address is apply, 0 if the "ip" doesn't have a correct format else -1
 */
int exalt_eth_set_new_ip(exalt_ethernet* eth, const char* ip)
{
 	if(!eth || !ip)
	{
	 	fprintf(stderr,"eth_set_new_ip(): eth==%p ip==%p !\n",eth,ip);
		return -1;
	}
	if(!exalt_is_address(ip))
	{
	 	fprintf(stderr,"eth_set_new_ip(): ip(%s) is not a valid address\n",ip);
		return 0;
	}

	EXALT_FREE(eth->new_ip)
	eth->new_ip=strdup(ip);
	return 1;
}



/**
 * @brief set the netmask address of the card "eth"
 * @param eth the card
 * @param netmask the new netmask address
 * @return Returns 1 if the new netmask address is apply, 0 if the "netmask" doesn't have a correct format else -1
 */
int exalt_eth_set_new_netmask(exalt_ethernet* eth, const char* netmask)
{
	if(!eth || !netmask)
	{
		fprintf(stderr,"eth_set_new_netmask(): eth==%p broadcast==%p !\n",eth,netmask);
		return -1;
	}
	if(!exalt_is_address(netmask))
	{
	 	fprintf(stderr,"eth_set_new_netmask(): broadcast(%s) is not a valid address\n",netmask);
		return 0;
	}

	EXALT_FREE(eth->new_netmask)
		eth->new_netmask=strdup(netmask);
	return 1;
}



/**
 * @brief set the gateway address of the card "eth"
 * @param eth the card
 * @param gateway the new gateway address
 * @return Returns 1 if the new gateway address is apply, 0 if the "gateway" doesn't have a correct format else -1
 */
int exalt_eth_set_new_gateway(exalt_ethernet* eth, const char* gateway)
{
	if(!eth || !gateway)
	{
		fprintf(stderr,"eth_set_new_gateway(): eth==%p gateway==%p !\n",eth,gateway);
		return -1;
	}
	if(!exalt_is_address(gateway) && strlen(gateway)>0)
	{
		fprintf(stderr,"eth_set_new_gateway(): broadcast(%s) is not a valid address\n",gateway);
		return 0;
	}
	EXALT_FREE(eth->new_gateway)
		eth->new_gateway=strdup(gateway);
	return 1;
}







/**
 * @brief set the the dhcp mode of the card "eth"
 * @param eth the card
 * @param dhcp the mode: 1 -> dhcp, 0 -> static
 * @return Returns 1 if the mode is apply, else 0
 */
int exalt_eth_set_new_dhcp(exalt_ethernet* eth, short dhcp)
{
	if(eth)
	{
		eth->new_dhcp_static=dhcp;
		return 1;
	}
	else
		return 0;
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
 * @brief apply the configuration for the card "eth"
 * @param eth the card
 * @return Returns 1 if the configuration is apply, else 0
 */
int exalt_eth_apply_conf(exalt_ethernet* eth)
{
	int res;
 	struct rtentry rt;
	struct sockaddr_in sin = { AF_INET };
	if(!eth)
	{
		fprintf(stderr,"eth_apply(): eth==null! \n");
		return -1;
	}


        printf("## Apply configuration for %s ##\n",exalt_eth_get_name(eth));

        if(!exalt_is_admin())
        {
            fprintf(stderr,"exalt_eth_apply_conf(): you need to be root if you want save the configuration file! \n");
            return -1;
        }

	exalt_sys_conf_save(eth);

	if(exalt_eth_is_wireless(eth))
	 	exalt_wireless_apply_conf(exalt_eth_get_wireless(eth));

	//remove old gateway
        if(exalt_eth_is_new_dhcp(eth) || exalt_is_address(exalt_eth_get_new_gateway(eth)))
        {
            memset((char *) &rt, 0, sizeof(struct rtentry));
            rt.rt_flags = ( RTF_UP | RTF_GATEWAY );
            sin.sin_addr.s_addr = inet_addr ("0.0.0.0");
            rt.rt_dst = *(struct sockaddr *) &sin;

            while (exalt_ioctl(&rt, SIOCDELRT) >=0)
                ;
        }
        if(exalt_eth_is_new_dhcp(eth))
		res = exalt_eth_apply_dhcp(eth);
	else
		res = exalt_eth_apply_static(eth);

	printf("## End configuration ## \n");
	return res;
}



/**
 * @brief apply static address for the card "eth"
 * @param eth the card
 * @return Returns 1 if static address are apply, else 0
 */
int exalt_eth_apply_static(exalt_ethernet *eth)
{
	struct sockaddr_in sin = { AF_INET };
	struct ifreq ifr;
	struct rtentry rt;

	if(!eth)
	{
	 	fprintf(stderr,"exalt_eth_apply_static(): eth==null ! \n");
		return -1;
	}

 	strncpy(ifr.ifr_name,exalt_eth_get_name(eth),sizeof(ifr.ifr_name));

 	//apply the ip
 	sin.sin_addr.s_addr = inet_addr (exalt_eth_get_new_ip(eth));
	ifr.ifr_addr = *(struct sockaddr *) &sin;
	if( exalt_ioctl(&ifr, SIOCSIFADDR) < 0)
		return -1;

	//apply the netmask
 	sin.sin_addr.s_addr = inet_addr (exalt_eth_get_new_netmask(eth));
	ifr.ifr_addr = *(struct sockaddr *) &sin;
	if( exalt_ioctl(&ifr, SIOCSIFNETMASK ) < 0)
	 	return -1;


	if(!exalt_eth_get_gateway(eth))
	 	return 1;

	//apply the default gateway
	memset((char *) &rt, 0, sizeof(struct rtentry));
 	rt.rt_flags = ( RTF_UP | RTF_GATEWAY );
	sin.sin_addr.s_addr = inet_addr (exalt_eth_get_new_gateway(eth));
	rt.rt_gateway = *(struct sockaddr *) &sin;
 	sin.sin_addr.s_addr = inet_addr ("0.0.0.0");
	rt.rt_dst = *(struct sockaddr *) &sin;
	rt.rt_metric = 2001;
	rt.rt_dev = exalt_eth_get_name(eth);

	if ( exalt_ioctl(&rt, SIOCADDRT) < 0)
	    return -1;

	return 1;
}



/**
 * @brief apply the dhcp mode for the card "eth"
 * @param eth the card
 * @return Returns 1 if the dhcp is apply, else 0
 */
int exalt_eth_apply_dhcp(exalt_ethernet* eth)
{
	FILE* f;
	char command[1024];
	char buf[1024];
	if(!eth)
	{
		fprintf(stderr,"eth_apply_dhcp(): eth==null! \n");
		return -1;
	}

	sprintf(command,"%s %s\n",COMMAND_DHCLIENT,exalt_eth_get_name(eth));
	printf("\t%s\n",command);
	f = exalt_execute_command(command);
	while(fgets(buf,1024,f))
		printf("\t%s\n",buf);
	EXALT_PCLOSE(f);

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
	exalt_ethernet* eth;

	ecore_list_first_goto(exalt_eth_interfaces.ethernets);
	data = ecore_list_next(exalt_eth_interfaces.ethernets);
	while(data)
	{
	 	eth = EXALT_ETHERNET(data);
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
			exalt_wireless_scan(eth);
			exalt_wireless_printf(*(eth->wireless));
		}
		data = ecore_list_next(exalt_eth_interfaces.ethernets);
	}
}







/**
 * Private functions bodies
 */

/**
 * @brief get the save ip address of the card "eth"
 * @param eth the card
 * @return Returns the ip address
 */
char* _exalt_eth_get_save_ip(exalt_ethernet* eth)
{
    if(!eth)
    {
        fprintf(stderr,"exalt_eth_get_save_ip(): eth==null ! \n");
        return NULL;
    }
    else
        return eth->_save_ip;
}

/**
 * @brief get the save default gateway of the card "eth"
 * @param eth the card
 * @return Returns the gateway
 */
char* _exalt_eth_get_save_gateway(exalt_ethernet* eth)
{
    if(!eth)
    {
        fprintf(stderr,"exalt_eth_get_save_gateway(): eth==null ! \n");
        return NULL;
    }
    else
        return eth->_save_gateway;
}

/**
 * @brief get the save netmask of the card "eth"
 * @param eth the card
 * @return Returns the netmask
 */
char* _exalt_eth_get_save_netmask(exalt_ethernet* eth)
{
    if(!eth)
    {
        fprintf(stderr,"_exalt_eth_get_save_netmask(): eth==null ! \n");
        return NULL;
    }
    else
        return eth->_save_netmask;
}

/**
 * @brief get the save link stat of the card "eth"
 * @param eth the card
 * @return Returns EXALT_TRUE or EXALT_FALSE
 */
short _exalt_eth_get_save_link(exalt_ethernet* eth)
{
    if(!eth)
    {
        fprintf(stderr,"_exalt_eth_get_save_link(): eth==null ! \n");
        return -1;
    }
    else
        return eth->_save_link;
}

/**
 * @brief get the save up stat of the card "eth"
 * @param eth the card
 * @return Returns EXALT_TRUE or EXALT_FALSE
 */
short _exalt_eth_get_save_up(exalt_ethernet* eth)
{
    if(!eth)
    {
        fprintf(stderr,"_exalt_eth_get_save_up(): eth==null ! \n");
        return -1;
    }
    else
        return eth->_save_up;
}


/**
 * @brief set the save ip address of the card "eth"
 * @param eth the card
 * @param ip the new ip address
 * @return Returns 1 if the save ip address is apply, 0 if the ip address doesn't have a correct format else -1
 */
int _exalt_eth_set_save_ip(exalt_ethernet* eth,const char* ip)
{
 	if(!eth || !ip)
	{
	 	fprintf(stderr,"_exalt_eth_set_save_ip(): eth==%p ip==%p !\n",eth,ip);
		return -1;
	}
	if(!exalt_is_address(ip))
	{
	 	fprintf(stderr,"_exalt_eth_set_save_ip(): ip(%s) is not a valid address\n",ip);
		return 0;
	}

	EXALT_FREE(eth->_save_ip)
	eth->_save_ip=strdup(ip);
	return 1;
}

/**
 * @brief set the save netmask of the card "eth"
 * @param eth the card
 * @param ip the new netmask
 * @return Returns 1 if the netmask is apply, 0 if the netmask doesn't have a correct format else -1
 */
int _exalt_eth_set_save_netmask(exalt_ethernet* eth,const char* netmask)
{
 	if(!eth || !netmask)
	{
	 	fprintf(stderr,"_exalt_eth_set_save_netmask(): eth==%p netmask==%p !\n",eth,netmask);
		return -1;
	}
	if(!exalt_is_address(netmask))
	{
	 	fprintf(stderr,"_exalt_eth_set_save_netmask(): netmask(%s) is not a valid address\n",netmask);
		return 0;
	}

	EXALT_FREE(eth->_save_netmask)
	eth->_save_netmask=strdup(netmask);
	return 1;
}

/**
 * @brief set the save gateway of the card "eth"
 * @param eth the card
 * @param ip the new gateway
 * @return Returns 1 if the new gateway is apply, 0 if the gateway doesn't have a correct format else -1
 */
int _exalt_eth_set_save_gateway(exalt_ethernet* eth,const char* gateway)
{
 	if(!eth || !gateway)
	{
	 	fprintf(stderr,"_exalt_eth_set_save_gateway(): eth==%p gateway==%p !\n",eth,gateway);
		return -1;
	}
	if(!exalt_is_address(gateway))
	{
	 	fprintf(stderr,"_exalt_eth_set_save_ip(): gateway(%s) is not a valid address\n",gateway);
		return 0;
	}

	EXALT_FREE(eth->_save_gateway)
	eth->_save_gateway=strdup(gateway);
	return 1;
}

/**
 * @brief set the save link state of the card "eth"
 * @param eth the card
 * @param ip the state link (EXALT_TRUE or EXALT_FALSE)
 * @return Returns 1 if the new stat is apply,else -1
 */
int _exalt_eth_set_save_link(exalt_ethernet* eth,short link)
{
 	if(!eth )
	{
	 	fprintf(stderr,"_exalt_eth_set_save_link(): eth==%p !\n",eth);
		return -1;
	}
	eth->_save_link=link;
	return 1;
}
/**
 * @brief set the save up state of the card "eth"
 * @param eth the card
 * @param ip the up state (EXALT_TRUE or EXALT_FALSE)
 * @return Returns 1 if the new state is apply,else -1
 */
int _exalt_eth_set_save_up(exalt_ethernet* eth,short up)
{
 	if(!eth )
	{
	 	fprintf(stderr,"_exalt_eth_set_save_up(): eth==%p !\n",eth);
		return -1;
	}
	eth->_save_up=up;
	return 1;
}


/**
 * @brief set the name of the card "eth"
 * @param eth the card
 * @param name the new name
 * @return Returns 1 if the new name is apply, else 0
 */
int _exalt_eth_set_name(exalt_ethernet* eth, const char* name)
{
	if(eth && name)
	{
		EXALT_FREE(eth->name)
		eth->name=strdup(name);
		return 1;
	}
	else
		return 0;
}


/**
 * @brief set the udi of the card "eth"
 * @param eth the card
 * @param udi the new udi
 * @return Returns 1 if udi is apply, else -1
 */
int _exalt_eth_set_udi(exalt_ethernet* eth,const char* udi)
{
 	if(!eth || !udi)
	{
	 	fprintf(stderr,"_exalt_eth_set_udi(): eth==%p udi==%p !\n",eth,udi);
		return -1;
	}

	EXALT_FREE(eth->udi)
	eth->udi=strdup(udi);
	return 1;
}

/**
 * @brief set the ifindex of the card "eth"
 * @param eth the card
 * @param ifindex the ifindex
 * @return Returns 1 if the new ifindex is apply,else -1
 */
int _exalt_eth_set_ifindex(exalt_ethernet* eth,int ifindex)
{
 	if(!eth )
	{
	 	fprintf(stderr,"_exalt_eth_set_ifindex(): eth==%p !\n",eth);
		return -1;
	}
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
        exalt_ethernet* eth = EXALT_ETHERNET(data);
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
 * @brief call when the kernel send a information
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
    exalt_ethernet* eth;
    void* data_l;
    Ecore_List* l;


    fd = *((int *)data);

    bin = recvmsg(fd, &msg, 0);
    if(bin < 0)
    {
        perror("_exalt_rtlink_watch_cb(): recvmsg()");
        return -1;
    }

    for(nh = (struct nlmsghdr *)buf; NLMSG_OK(nh, bin);
            nh = NLMSG_NEXT(nh, bin))
    {
        switch(nh->nlmsg_type)
        {
            case RTM_NEWLINK:
            case RTM_DELLINK:
                ifmsg = NLMSG_DATA(nh);
                rtlen = RTM_PAYLOAD(nh);
                if(rtlen < sizeof(struct ifinfomsg))
                {
                    fprintf(stderr,"_exalt_rtlink_watch_cb(): Bad rtnetlink packet (%d < %d).\n", rtlen,
                            sizeof(struct ifinfomsg));
                    break;
                }

                printf("ifmsg: { %d, %d, %d, 0x%x, 0x%x }\n",
                        ifmsg->ifi_family, ifmsg->ifi_type,
                        ifmsg->ifi_index, ifmsg->ifi_flags,
                        ifmsg->ifi_change);

                eth = exalt_eth_get_ethernet_byifindex(ifmsg->ifi_index);
                if(!eth)
                    break;

                if(_exalt_eth_get_save_up(eth) != exalt_eth_is_up(eth))
                {
                    _exalt_eth_set_save_up(eth, exalt_eth_is_up(eth));
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
                    exalt_rtlink_essid_change(exalt_eth_get_wireless(eth));
                }
                break;
            case RTM_NEWADDR:
            case RTM_DELADDR:
                ifaddr = NLMSG_DATA(nh);
                rtlen = RTM_PAYLOAD(nh);
                if(rtlen < sizeof(struct ifaddrmsg))
                {
                    fprintf(stderr,"_exalt_rtlink_watch_cb(): Bad rtnetlink packet (%d < %d).\n", rtlen,
                            sizeof(struct ifaddrmsg));
                    break;
                }

                eth = exalt_eth_get_ethernet_byifindex(ifaddr->ifa_index);
                if(!eth)
                    break;

                if(strcmp(_exalt_eth_get_save_ip(eth),exalt_eth_get_ip(eth)) != 0)
                {
                    _exalt_eth_set_save_ip(eth, exalt_eth_get_ip(eth));
                    if(exalt_eth_interfaces.eth_cb)
                        exalt_eth_interfaces.eth_cb(eth,EXALT_ETH_CB_ACTION_ADDRESS_NEW,exalt_eth_interfaces.eth_cb_user_data);
                }

                if(strcmp(_exalt_eth_get_save_netmask(eth),exalt_eth_get_netmask(eth)) != 0)
                {
                    _exalt_eth_set_save_netmask(eth, exalt_eth_get_netmask(eth));
                    if(exalt_eth_interfaces.eth_cb)
                        exalt_eth_interfaces.eth_cb(eth,EXALT_ETH_CB_ACTION_NETMASK_NEW,exalt_eth_interfaces.eth_cb_user_data);
                }

            case RTM_NEWROUTE:
            case RTM_DELROUTE:
                ifroute = NLMSG_DATA(nh);
                rtlen = RTM_PAYLOAD(nh);
                if(rtlen < sizeof(struct rtmsg))
                {
                    fprintf(stderr,"_exalt_rtlink_watch_cb(): Bad rtnetlink packet (%d < %d).\n", rtlen,
                            sizeof(struct rtmsg));
                    break;
                }

                //test for each interface if the route has been modified
                l = exalt_eth_get_list();
                ecore_list_first_goto(l);
                data_l = ecore_list_next(l);
                while(data_l)
                {
                    eth = EXALT_ETHERNET(data_l);
                    if(strcmp(_exalt_eth_get_save_gateway(eth),exalt_eth_get_gateway(eth)) != 0)
                    {
                        _exalt_eth_set_save_gateway(eth, exalt_eth_get_gateway(eth));
                        if(exalt_eth_interfaces.eth_cb)
                            exalt_eth_interfaces.eth_cb(eth,EXALT_ETH_CB_ACTION_GATEWAY_NEW,exalt_eth_interfaces.eth_cb_user_data);
                    }

                    data_l = ecore_list_next(l);
                }
                break;
            default: printf("hd cb default!\n");break;
        }
    }
    return 1;
}

/** @} */

