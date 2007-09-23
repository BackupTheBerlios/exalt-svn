/*
 * =====================================================================================
 *
 *       Filename:  daemon.c
 *
 *    Description:  defines globals funtions
 *
 *        Version:  1.0
 *        Created:  08/28/2007 04:27:45 PM CEST
 *       Revision:  none
 *       Compiler:  gcc
 *
 *         Author:   (Watchwolf), Atton Jonathan <watchwolf@watchwolf.fr>
 *        Company:
 *
 * =====================================================================================
 */

#include "daemon.h"

int setup(E_DBus_Connection *conn)
{
    E_DBus_Object *obj;
    E_DBus_Interface *iface;
    e_dbus_request_name(conn, EXALTD_SERVICE , 0, NULL, NULL);
    obj = e_dbus_object_add(conn, EXALTD_PATH, NULL);
    iface = e_dbus_interface_new(EXALTD_INTERFACE_READ);

    e_dbus_interface_method_add(iface, "IS_ADDRESS", NULL, NULL, dbus_cb_is_address);
    e_dbus_interface_method_add(iface, "IS_ESSID", NULL, NULL, dbus_cb_is_essid);
    e_dbus_interface_method_add(iface, "IS_PASSWD", NULL, NULL, dbus_cb_is_passwd);

    e_dbus_interface_method_add(iface, "IFACE_GET_IP", NULL, NULL, dbus_cb_eth_get_ip);
    e_dbus_interface_method_add(iface, "IFACE_GET_NETMASK", NULL, NULL, dbus_cb_eth_get_netmask);
    e_dbus_interface_method_add(iface, "IFACE_GET_GATEWAY", NULL, NULL, dbus_cb_eth_get_gateway);
    e_dbus_interface_method_add(iface, "IFACE_GET_ETH_LIST", NULL, NULL, dbus_cb_eth_get_eth_list);
    e_dbus_interface_method_add(iface, "IFACE_IS_WIRELESS", NULL, NULL, dbus_cb_eth_is_wireless);
    e_dbus_interface_method_add(iface, "IFACE_IS_LINK", NULL, NULL, dbus_cb_eth_is_link);
    e_dbus_interface_method_add(iface, "IFACE_IS_UP", NULL, NULL, dbus_cb_eth_is_up);
    e_dbus_interface_method_add(iface, "IFACE_IS_DHCP", NULL, NULL, dbus_cb_eth_is_dhcp);


    e_dbus_interface_method_add(iface, "IFACE_GET_ESSID", NULL, NULL, dbus_cb_wireless_get_essid);
    e_dbus_interface_method_add(iface, "IFACE_SCAN_START", NULL, NULL, dbus_cb_wireless_scan_start);
    e_dbus_interface_method_add(iface, "IFACE_SCAN_STOP", NULL, NULL, dbus_cb_wireless_scan_stop);
    e_dbus_interface_method_add(iface, "IFACE_SCAN_WAIT", NULL, NULL, dbus_cb_wireless_scan_wait);

    e_dbus_interface_method_add(iface, "NETWORK_GET_QUALITY", NULL, NULL, dbus_cb_wirelessinfo_get_quality);
    e_dbus_interface_method_add(iface, "NETWORK_GET_ADDR", NULL, NULL, dbus_cb_wirelessinfo_get_addr);
    e_dbus_interface_method_add(iface, "NETWORK_GET_PROTOCOL", NULL, NULL, dbus_cb_wirelessinfo_get_protocol);
    e_dbus_interface_method_add(iface, "NETWORK_GET_MODE", NULL, NULL, dbus_cb_wirelessinfo_get_mode);
    e_dbus_interface_method_add(iface, "NETWORK_GET_CHANNEL", NULL, NULL, dbus_cb_wirelessinfo_get_channel);
    e_dbus_interface_method_add(iface, "NETWORK_GET_BITRATES", NULL, NULL, dbus_cb_wirelessinfo_get_bitrates);
    e_dbus_interface_method_add(iface, "NETWORK_GET_ENCRYPTION", NULL, NULL, dbus_cb_wirelessinfo_get_encryption);
    e_dbus_interface_method_add(iface, "NETWORK_GET_SIGNALLVL", NULL, NULL, dbus_cb_wirelessinfo_get_signallvl);
    e_dbus_interface_method_add(iface, "NETWORK_GET_NOISELVL", NULL, NULL, dbus_cb_wirelessinfo_get_noiselvl);

    e_dbus_interface_method_add(iface, "NETWORK_GET_DEFAULT_PASSWD", NULL, NULL, dbus_cb_wirelessinfo_get_default_passwd);
    e_dbus_interface_method_add(iface, "NETWORK_GET_DEFAULT_IP", NULL, NULL, dbus_cb_wirelessinfo_get_default_ip);
    e_dbus_interface_method_add(iface, "NETWORK_GET_DEFAULT_NETMASK", NULL, NULL, dbus_cb_wirelessinfo_get_default_netmask);
    e_dbus_interface_method_add(iface, "NETWORK_GET_DEFAULT_GATEWAY", NULL, NULL, dbus_cb_wirelessinfo_get_default_gateway);
    e_dbus_interface_method_add(iface, "NETWORK_GET_DEFAULT_PASSWD_MODE", NULL, NULL, dbus_cb_wirelessinfo_get_default_passwd_mode);
    e_dbus_interface_method_add(iface, "NETWORK_GET_DEFAULT_SECURITY_MODE", NULL, NULL, dbus_cb_wirelessinfo_get_default_security_mode);
    e_dbus_interface_method_add(iface, "NETWORK_GET_DEFAULT_MODE", NULL, NULL, dbus_cb_wirelessinfo_get_default_mode);
    e_dbus_interface_method_add(iface, "NETWORK_IS_DEFAULT_DHCP", NULL, NULL, dbus_cb_wirelessinfo_is_default_dhcp);

    e_dbus_interface_method_add(iface, "DNS_GET_LIST", NULL, NULL, dbus_cb_dns_get_list);

    e_dbus_object_interface_attach(obj, iface);

    iface = e_dbus_interface_new(EXALTD_INTERFACE_WRITE);
    e_dbus_interface_method_add(iface, "IFACE_UP", NULL, NULL, dbus_cb_eth_up);
    e_dbus_interface_method_add(iface, "IFACE_DOWN", NULL, NULL, dbus_cb_eth_down);
    e_dbus_interface_method_add(iface, "IFACE_SET_NEW_IP", NULL, NULL, dbus_cb_eth_set_new_ip);
    e_dbus_interface_method_add(iface, "IFACE_SET_NEW_NETMASK", NULL, NULL, dbus_cb_eth_set_new_netmask);
    e_dbus_interface_method_add(iface, "IFACE_SET_NEW_GATEWAY", NULL, NULL, dbus_cb_eth_set_new_gateway);
    e_dbus_interface_method_add(iface, "IFACE_SET_NEW_DHCP", NULL, NULL, dbus_cb_eth_set_new_dhcp);

    e_dbus_interface_method_add(iface, "IFACE_SET_NEW_ESSID", NULL, NULL, dbus_cb_eth_set_new_essid);
    e_dbus_interface_method_add(iface, "IFACE_SET_NEW_PASSWD", NULL, NULL, dbus_cb_eth_set_new_passwd);
    e_dbus_interface_method_add(iface, "IFACE_SET_NEW_PASSWD_MODE", NULL, NULL, dbus_cb_eth_set_new_passwd_mode);
    e_dbus_interface_method_add(iface, "IFACE_SET_NEW_MODE", NULL, NULL, dbus_cb_eth_set_new_mode);
    e_dbus_interface_method_add(iface, "IFACE_SET_NEW_SECURITY_MODE", NULL, NULL, dbus_cb_eth_set_new_security_mode);

    e_dbus_interface_method_add(iface, "DNS_ADD", NULL, NULL, dbus_cb_dns_add);
    e_dbus_interface_method_add(iface, "DNS_REPLACE", NULL, NULL, dbus_cb_dns_replace);
    e_dbus_interface_method_add(iface, "DNS_DELETE", NULL, NULL, dbus_cb_dns_delete);

    e_dbus_interface_method_add(iface, "IFACE_APPLY_CONF", NULL, NULL, dbus_cb_eth_apply_conf);

    e_dbus_object_interface_attach(obj, iface);


    return 1;
}


int main(int argc, char** argv)
{
    E_DBus_Connection *conn;
    int daemon = 1;

    argc--;
    argv++;
    while(argc)
    {
        if(strcmp(*argv, "--nodaemon")==0)
        {
            daemon = 0;
        }
        else if(strcmp(*argv,"--help")==0)
        {
            printf("Usage exalt-daemon [OPTION]\n" \
                    "--nodaemon    doesn't run as a daemon\n" \
                    "--help        display this help and exit\n");
            exit(1);
        }
        argc--;
        argv++;
    }

    if(daemon)
    {
        if(fork()!=0)
            exit(0);

        //create the pid file
        FILE *fp;
        int size;
        char buf[PATH_MAX];
        size = snprintf(buf, PATH_MAX, "%d\n", getpid());
        if ((fp = fopen(EXALTD_PIDFILE, "w+")))
        {
            fwrite(buf, sizeof(char), size, fp);
            fclose(fp);
        }
        else
            print_error("WARNING", __FILE__, __LINE__,__func__, "Can not create the pid file: %s\n", EXALTD_PIDFILE);

        //redirect stderr and stdout >> /var/log/exald.log
        remove(EXALTD_LOGFILE);
        if ((fp = fopen(EXALTD_LOGFILE, "w+")))
        {
            stderr = fp;
            stdout = fp;
        }
        else
            print_error("WARNING", __FILE__, __LINE__,__func__, "Can not create the log file: %s\n",EXALTD_LOGFILE);
    }

    e_dbus_init();
    ecore_init();
    exalt_eth_init();

    conn = e_dbus_bus_get(DBUS_BUS_SYSTEM);
    if(!conn)
    {
        print_error("ERROR", __FILE__, __LINE__,__func__, "main(): can not connect to DBUS, maybe the daemon is not launch ?\n");
        e_dbus_shutdown();
        ecore_shutdown();
        return -1;
    }

    setup(conn);

    exalt_eth_set_cb(eth_cb,conn);
    exalt_eth_set_scan_cb(wireless_scan_cb,conn);

    exalt_main();

    if(!exalt_is_admin())
        print_error("WARNING", __FILE__, __LINE__,__func__, "The daemon is not run with administrator's right, you can't modify the configuration");

    ecore_main_loop_begin();

    e_dbus_shutdown();
    ecore_shutdown();
    return 1;
}

void eth_cb(exalt_ethernet* eth, int action, void* data)
{
    E_DBus_Connection *conn;
    DBusMessage* msg;
    DBusMessageIter args;
    const char* name;

    conn = (E_DBus_Connection*) data;

    //if a new card appears we apply the configuration
    if(action == EXALT_ETH_CB_ACTION_NEW || action == EXALT_ETH_CB_ACTION_ADD)
    {
        printf("##### NEW CARD %s\n",exalt_eth_get_name(eth));
        if(exalt_eth_is_new_up(eth))
        {
            printf("UP\n");
            exalt_eth_up(eth);
        }
        else
        {
            printf("DOWN\n");
            exalt_eth_down(eth);
        }
    }

    if(action == EXALT_ETH_CB_ACTION_UP)
    {
        printf("APPLY CONFIGURARTION\n");
        exalt_eth_apply_conf(eth, NULL, NULL);
    }

    //send a broadcast
    msg = dbus_message_new_signal(EXALTD_PATH,EXALTD_INTERFACE_READ, "NOTIFY");
    if(!msg)
    {
        print_error("ERROR", __FILE__, __LINE__,__func__, "msg=%p",msg);
        return ;
    }

    name = exalt_eth_get_name(eth);

    if(!name)
    {
        print_error("ERROR", __FILE__, __LINE__,__func__, "name=%p",name);
        dbus_message_unref(msg);
        return ;
    }

    dbus_message_iter_init_append(msg, &args);
    if (!dbus_message_iter_append_basic(&args, DBUS_TYPE_STRING, &name))
    {
        print_error("ERROR", __FILE__, __LINE__,__func__, "Out Of Memory");
        dbus_message_unref(msg);
        return ;
    }
    if (!dbus_message_iter_append_basic(&args, DBUS_TYPE_UINT32, &action))
    {
        print_error("ERROR", __FILE__, __LINE__,__func__, "Out Of Memory");
        dbus_message_unref(msg);
        return ;
    }
    e_dbus_message_send(conn, msg, NULL, 3,NULL);
    dbus_message_unref(msg);
}

void wireless_scan_cb(exalt_ethernet* eth,Ecore_List* new_networks, Ecore_List* old_networks, void* data)
{
    E_DBus_Connection *conn;
    DBusMessage* msg;
    DBusMessageIter args;
    const char* name;
    const char* essid;
    exalt_wireless_info *wi;
    int separator = 0;

    Ecore_List* l;

    if(!new_networks || !old_networks || !data)
    {
        print_error("ERROR", __FILE__, __LINE__,__func__, "new_networks=%p, old_networks=%p, data=%p",new_networks, old_networks, data);
        return ;
    }

    conn = (E_DBus_Connection*) data;

    //send a broadcast
    msg = dbus_message_new_signal(EXALTD_PATH,EXALTD_INTERFACE_READ, "SCAN_NOTIFY");
    if(!msg)
    {
        print_error("ERROR", __FILE__, __LINE__,__func__, "msg=%p",msg);
        return ;
    }

    name = exalt_eth_get_name(eth);
    if(!name)
    {
        print_error("ERROR", __FILE__, __LINE__,__func__, "name=%p",name);
        dbus_message_unref(msg);
        return ;
    }
    dbus_message_iter_init_append(msg, &args);
    if (!dbus_message_iter_append_basic(&args, DBUS_TYPE_STRING, &name))
    {
        print_error("ERROR", __FILE__, __LINE__,__func__, "Out Of Memory");
        dbus_message_unref(msg);
        return ;
    }

    l=new_networks;
    ecore_list_first_goto(l);
    while ( (wi =  ecore_list_next(l)))
    {
        essid = exalt_wirelessinfo_get_essid(wi);
        if(!essid)
        {
            print_error("ERROR", __FILE__, __LINE__,__func__, "essid=%p",essid);
            dbus_message_unref(msg);
            return ;
        }

        if (!dbus_message_iter_append_basic(&args, DBUS_TYPE_STRING, &essid))
        {
            print_error("ERROR", __FILE__, __LINE__,__func__, "Out Of Memory");
            dbus_message_unref(msg);
            return ;
        }
    }

    if (!dbus_message_iter_append_basic(&args, DBUS_TYPE_INT32, &separator))
    {
        print_error("ERROR", __FILE__, __LINE__,__func__, "Out Of Memory");
        dbus_message_unref(msg);
        return ;
    }

    l=old_networks;
    ecore_list_first_goto(l);
    while ( (wi =  ecore_list_next(l)))
    {
        essid = exalt_wirelessinfo_get_essid(wi);
        if(!essid)
        {
            print_error("ERROR", __FILE__, __LINE__,__func__, "essid=%p",essid);
            dbus_message_unref(msg);
            return ;
        }

        if (!dbus_message_iter_append_basic(&args, DBUS_TYPE_STRING, &essid))
        {
            print_error("ERROR", __FILE__, __LINE__,__func__, "Out Of Memory");
            dbus_message_unref(msg);
            return ;
        }
    }

    e_dbus_message_send(conn, msg, NULL, -1,NULL);
    dbus_message_unref(msg);
}


exalt_ethernet* dbus_get_eth(DBusMessage* msg)
{
    DBusMessageIter args;
    char* interface= NULL;
    exalt_ethernet* eth;

    if(!dbus_message_iter_init(msg, &args))
    {
        print_error("ERROR", __FILE__, __LINE__,__func__, "no argument");
        return NULL;
    }
    if (DBUS_TYPE_STRING != dbus_message_iter_get_arg_type(&args))
    {
        print_error("ERROR", __FILE__, __LINE__,__func__, "Argument is not a string");
        return NULL;
    }
    else
        dbus_message_iter_get_basic(&args, &interface);

    //search the interface
    eth = exalt_eth_get_ethernet_byname(interface);
    if(!eth)
    {
        print_error("WARNING", __FILE__, __LINE__,__func__, "the interface %s doesn't exist\n",interface);
        return NULL;
    }
    return eth;
}

exalt_wireless_info* dbus_get_wirelessinfo(DBusMessage* msg)
{
    DBusMessageIter args;
    char* interface = NULL;
    exalt_ethernet* eth;
    char* essid = NULL;
    exalt_wireless_info* wi;

    if(!dbus_message_iter_init(msg, &args))
    {
        print_error("ERROR", __FILE__, __LINE__,__func__, "no argument");
        return NULL;
    }

    if (DBUS_TYPE_STRING != dbus_message_iter_get_arg_type(&args))
    {
        print_error("ERROR", __FILE__, __LINE__,__func__, "Argument is not a string");
        return NULL;
    }
    else
        dbus_message_iter_get_basic(&args, &interface);

    //search the interface
    eth = exalt_eth_get_ethernet_byname(interface);
    if(!eth)
    {
        print_error("WARNING", __FILE__, __LINE__,__func__, "the interface %s doesn't exist\n",interface);
        return NULL;
    }

    dbus_message_iter_next(&args);
    if (DBUS_TYPE_STRING != dbus_message_iter_get_arg_type(&args))
    {
        print_error("ERROR", __FILE__, __LINE__,__func__, "Argument is not a string");
        return NULL;
    }
    else
        dbus_message_iter_get_basic(&args, &essid);

    //search the interface
    wi = get_wirelessinfo(eth,essid);
    if(!wi)
    {
        print_error("WARNING", __FILE__, __LINE__,__func__, "the network %s doesn't exist\n",essid);
        return NULL;
    }

    return wi;
}

exalt_wireless_info* get_wirelessinfo(exalt_ethernet* eth, char* essid)
{
    exalt_wireless* w;
    exalt_wireless_info* wi;
    Ecore_List *l;
    void *data;

    if(!eth || !essid)
    {
        print_error("WARNING", __FILE__, __LINE__,__func__, "eth==%p essid=%p",eth,essid);
        return NULL;
    }

    if(!exalt_eth_is_wireless(eth))
    {
        print_error("WARNING", __FILE__, __LINE__,__func__, "The card %s is not a wireless card",exalt_eth_get_name(eth));
        return NULL;
    }
    w = exalt_eth_get_wireless(eth);

    l = exalt_wireless_get_networks_list(w);
    ecore_list_first_goto(l);

    while( (data=ecore_list_next(l)))
    {
        wi = EXALT_WIRELESS_INFO(data);
        if( strcmp(essid,exalt_wirelessinfo_get_essid(wi))==0 )
            return wi;
    }
    return NULL;
}

DBusMessage * dbus_cb_is_address(E_DBus_Object *obj __UNUSED__, DBusMessage *msg)
{
    DBusMessage *reply;
    DBusMessageIter args;
    int is;
    char* address;


    reply = dbus_message_new_method_return(msg);

    if(!dbus_message_iter_init(msg, &args))
    {
        print_error("ERROR", __FILE__, __LINE__,__func__, "no argument");
        return reply;
    }
    if (DBUS_TYPE_STRING != dbus_message_iter_get_arg_type(&args))
    {
        print_error("ERROR", __FILE__, __LINE__,__func__, "Argument is not a string");
        return reply;
    }
    else
        dbus_message_iter_get_basic(&args, &address);


    dbus_message_iter_init_append(reply, &args);
    is = exalt_is_address(address);
    if (!dbus_message_iter_append_basic(&args, DBUS_TYPE_BOOLEAN, &is))
    {
        print_error("ERROR", __FILE__, __LINE__,__func__, "Out Of Memory");
        return reply;
    }

    return reply;
}

DBusMessage * dbus_cb_is_essid(E_DBus_Object *obj __UNUSED__, DBusMessage *msg)
{
    DBusMessage *reply;
    DBusMessageIter args;
    int is;
    char* essid;


    reply = dbus_message_new_method_return(msg);

    if(!dbus_message_iter_init(msg, &args))
    {
        print_error("ERROR", __FILE__, __LINE__,__func__, "no argument");
        return reply;
    }
    if (DBUS_TYPE_STRING != dbus_message_iter_get_arg_type(&args))
    {
        print_error("ERROR", __FILE__, __LINE__,__func__, "Argument is not a string");
        return reply;
    }
    else
        dbus_message_iter_get_basic(&args, &essid);


    dbus_message_iter_init_append(reply, &args);
    is = exalt_is_essid(essid);
    if (!dbus_message_iter_append_basic(&args, DBUS_TYPE_BOOLEAN, &is))
    {
        print_error("ERROR", __FILE__, __LINE__,__func__, "Out Of Memory");
        return reply;
    }

    return reply;
}

DBusMessage * dbus_cb_is_passwd(E_DBus_Object *obj __UNUSED__, DBusMessage *msg)
{
    DBusMessage *reply;
    DBusMessageIter args;
    int is;
    char* passwd;
    int mode;

    reply = dbus_message_new_method_return(msg);

    if(!dbus_message_iter_init(msg, &args))
    {
        print_error("ERROR", __FILE__, __LINE__,__func__, "no argument");
        return reply;
    }
    if (DBUS_TYPE_STRING != dbus_message_iter_get_arg_type(&args))
    {
        print_error("ERROR", __FILE__, __LINE__,__func__, "Argument is not a string");
        return reply;
    }
    else
        dbus_message_iter_get_basic(&args, &passwd);

    dbus_message_iter_next(&args);
    if (DBUS_TYPE_INT32 != dbus_message_iter_get_arg_type(&args))
    {
        print_error("ERROR", __FILE__, __LINE__,__func__, "Argument is not an int32");
        return reply;
    }
    else
        dbus_message_iter_get_basic(&args, &mode);



    dbus_message_iter_init_append(reply, &args);
    is = exalt_is_passwd(passwd,mode);
    if (!dbus_message_iter_append_basic(&args, DBUS_TYPE_BOOLEAN, &is))
    {
        print_error("ERROR", __FILE__, __LINE__,__func__, "Out Of Memory");
        return reply;
    }

    return reply;
}

void print_error(const char* type, const char* file, int line,const char* fct, const char* msg, ...)
{
    va_list ap;
    va_start(ap,msg);
    fprintf(stderr,"EXALTD:%s (%d)%s: %s\n",type,line,file,fct);
    fprintf(stderr,"\t");
    vfprintf(stderr,msg,ap);
    fprintf(stderr,"\n\n");
    va_end(ap);
}

