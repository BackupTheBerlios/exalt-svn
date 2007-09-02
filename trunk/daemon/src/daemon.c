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

    e_dbus_interface_method_add(iface, "IFACE_GET_IP", NULL, NULL, dbus_cb_eth_get_ip);
    e_dbus_interface_method_add(iface, "IFACE_GET_NETMASK", NULL, NULL, dbus_cb_eth_get_netmask);
    e_dbus_interface_method_add(iface, "IFACE_GET_GATEWAY", NULL, NULL, dbus_cb_eth_get_gateway);
    e_dbus_interface_method_add(iface, "IFACE_GET_ETH_LIST", NULL, NULL, dbus_cb_eth_get_eth_list);
    e_dbus_interface_method_add(iface, "IFACE_IS_WIRELESS", NULL, NULL, dbus_cb_eth_is_wireless);
    e_dbus_interface_method_add(iface, "IFACE_IS_LINK", NULL, NULL, dbus_cb_eth_is_link);
    e_dbus_interface_method_add(iface, "IFACE_IS_UP", NULL, NULL, dbus_cb_eth_is_up);

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
        {
            char buf[1024];
            sprintf(buf,"Can not create the pid file: %s\n", EXALTD_PIDFILE);
            print_error("WARNING", __FILE__, __LINE__,__func__, buf);
        }

        //redirect stderr and stdout >> /var/log/exald.log
        remove(EXALTD_LOGFILE);
        if ((fp = fopen(EXALTD_LOGFILE, "w+")))
        {
            stderr = fp;
            stdout = fp;
        }
        else
        {
            char buf[1024];
            sprintf(buf,"Can not create the log file: %s\n",EXALTD_LOGFILE);
            print_error("WARNING", __FILE__, __LINE__,__func__, buf);
        }
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
    char* name;

    conn = (E_DBus_Connection*) data;

    //send a broadcast
    msg = dbus_message_new_signal(EXALTD_PATH,EXALTD_INTERFACE_READ, "NOTIFY");
    if(!msg)
    {
        print_error("ERROR", __FILE__, __LINE__,__func__, "msg==null");
        return ;
    }

    name = exalt_eth_get_name(eth);

    if(!name)
    {
        print_error("ERROR", __FILE__, __LINE__,__func__, "name==null");
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
        char buf[1024];
        sprintf(buf,"the interface %s doesn't exist\n",interface);
        print_error("WARNING", __FILE__, __LINE__,__func__, buf);
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
        char buf[1024];
        sprintf(buf,"the interface %s doesn't exist\n",interface);
        print_error("WARNING", __FILE__, __LINE__,__func__, buf);
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
        char buf[1024];
        sprintf(buf,"the network %s doesn't exist\n",essid);
        print_error("WARNING", __FILE__, __LINE__,__func__, buf);
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
        char buf[1024];
        sprintf(buf, "eth==%p essid=%p",eth,essid);
        print_error("WARNING", __FILE__, __LINE__,__func__, buf);
        return NULL;
    }

    if(!exalt_eth_is_wireless(eth))
    {
        char buf[1024];
        sprintf(buf,"The card %s is not a wireless card",exalt_eth_get_name(eth));
        print_error("WARNING", __FILE__, __LINE__,__func__, buf);
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

void print_error(char* type, char* file, int line,const char* fct, char* msg)
{
    fprintf(stderr,"DAEMON:%s (%d)%s: %s\n",type,line,file,fct);
    fprintf(stderr,"\t%s\n\n",msg);
}
