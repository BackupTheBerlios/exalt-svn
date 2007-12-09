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
    e_dbus_interface_method_add(iface, "IFACE_IS_DHCP", NULL, NULL, dbus_cb_eth_is_dhcp);


    e_dbus_interface_method_add(iface, "IFACE_GET_ESSID", NULL, NULL, dbus_cb_wireless_get_essid);
    e_dbus_interface_method_add(iface, "IFACE_SCAN_START", NULL, NULL, dbus_cb_wireless_scan_start);
    e_dbus_interface_method_add(iface, "IFACE_SCAN_STOP", NULL, NULL, dbus_cb_wireless_scan_stop);
    e_dbus_interface_method_add(iface, "IFACE_SCAN_WAIT", NULL, NULL, dbus_cb_wireless_scan_wait);
    e_dbus_interface_method_add(iface, "IFACE_GET_WPASUPPLICANT_DRIVER", NULL, NULL, dbus_cb_wireless_get_wpasupplicant_driver);


    e_dbus_interface_method_add(iface, "NETWORK_GET_QUALITY", NULL, NULL, dbus_cb_wirelessinfo_get_quality);
    e_dbus_interface_method_add(iface, "NETWORK_GET_ADDR", NULL, NULL, dbus_cb_wirelessinfo_get_addr);
    e_dbus_interface_method_add(iface, "NETWORK_GET_PROTOCOL", NULL, NULL, dbus_cb_wirelessinfo_get_protocol);
    e_dbus_interface_method_add(iface, "NETWORK_GET_MODE", NULL, NULL, dbus_cb_wirelessinfo_get_mode);
    e_dbus_interface_method_add(iface, "NETWORK_GET_CHANNEL", NULL, NULL, dbus_cb_wirelessinfo_get_channel);
    e_dbus_interface_method_add(iface, "NETWORK_GET_BITRATES", NULL, NULL, dbus_cb_wirelessinfo_get_bitrates);
    e_dbus_interface_method_add(iface, "NETWORK_GET_ENCRYPTION", NULL, NULL, dbus_cb_wirelessinfo_get_encryption);
    e_dbus_interface_method_add(iface, "NETWORK_GET_SIGNALLVL", NULL, NULL, dbus_cb_wirelessinfo_get_signallvl);
    e_dbus_interface_method_add(iface, "NETWORK_GET_NOISELVL", NULL, NULL, dbus_cb_wirelessinfo_get_noiselvl);

    e_dbus_interface_method_add(iface, "NETWORK_GET_DEFAULT_CONN", NULL, NULL, dbus_cb_wirelessinfo_get_default_conn);

    e_dbus_interface_method_add(iface, "DNS_GET_LIST", NULL, NULL, dbus_cb_dns_get_list);

    e_dbus_object_interface_attach(obj, iface);

    iface = e_dbus_interface_new(EXALTD_INTERFACE_WRITE);
    e_dbus_interface_method_add(iface, "IFACE_UP", NULL, NULL, dbus_cb_eth_up);
    e_dbus_interface_method_add(iface, "IFACE_DOWN", NULL, NULL, dbus_cb_eth_down);

    e_dbus_interface_method_add(iface, "IFACE_SET_WPASUPPLICANT_DRIVER", NULL, NULL, dbus_cb_wireless_set_wpasupplicant_driver);

    e_dbus_interface_method_add(iface, "DNS_ADD", NULL, NULL, dbus_cb_dns_add);
    e_dbus_interface_method_add(iface, "DNS_REPLACE", NULL, NULL, dbus_cb_dns_replace);
    e_dbus_interface_method_add(iface, "DNS_DELETE", NULL, NULL, dbus_cb_dns_delete);


    e_dbus_interface_method_add(iface, "IFACE_APPLY_CONN", NULL, NULL, dbus_cb_eth_apply_conn);

    e_dbus_object_interface_attach(obj, iface);

    return 1;
}


int main(int argc, char** argv)
{
    E_DBus_Connection *conn;
    int daemon = 1;
    FILE *fp;
    int size;
    char buf[PATH_MAX];

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
    exalt_init();

    if(!exalt_is_admin())
    {
        print_error("WARNING", __FILE__, __LINE__,__func__, "Please run as root. \n");
        e_dbus_shutdown();
        ecore_shutdown();
        return 1;
    }


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

    if(daemon)
    {
        //if we need waiting 1 or more card
        waiting_card_list = NULL;
        waiting_card_timer = NULL;
        waiting_card_load(CONF_FILE);
        if ( waiting_card_list )
        {
            //start the timer for the timeout
            int timeout = 0;
            waiting_card_timer = ecore_timer_add(5, waiting_card_stop, &timeout);;
            ecore_main_loop_begin();
        }

        //all waiting card are set (or timeout)
        //we create the child and then quit
        if(fork()!=0)
            exit(0);

        //create the pid file
        size = snprintf(buf, PATH_MAX, "%d\n", getpid());
        if ((fp = fopen(EXALTD_PIDFILE, "w+")))
        {
            fwrite(buf, sizeof(char), size, fp);
            fclose(fp);
        }
        else
            print_error("WARNING", __FILE__, __LINE__,__func__, "Can not create the pid file: %s\n", EXALTD_PIDFILE);
    }

    ecore_main_loop_begin();

    e_dbus_shutdown();
    ecore_shutdown();
    return 1;
}

void eth_cb(Exalt_Ethernet* eth, Exalt_Enum_Action action, void* data)
{
    E_DBus_Connection *conn;
    DBusMessage* msg;
    DBusMessageIter args;
    const char* name;
    char* str;

    conn = (E_DBus_Connection*) data;

    if(action == EXALT_ETH_CB_ACTION_NEW || action == EXALT_ETH_CB_ACTION_ADD)
    {
        //first we load the driver if eth is a wireless connection
        if(exalt_eth_is_wireless(eth))
        {
            Exalt_Wireless* w = exalt_eth_get_wireless(eth);
            str = exalt_eth_driver_load(CONF_FILE,exalt_eth_get_udi(eth));
            exalt_wireless_set_wpasupplicant_driver(w,str);
            EXALT_FREE(str);
        }

        //apply or not apply a connection ?
        //load a connection
        //if no connection is load, we create one
        //if we didn't have a connection or if the card is save as "up"
        //  if is not up
        //      we up the card
        //  we wait than the card is up (with a timeout of 5 ms)
        //  if the card is link
        //      we apply the connection
        //else
        //  we down the card

        Exalt_Connection *c = exalt_eth_conn_load(CONF_FILE, exalt_eth_get_udi(eth));
        short not_c = 0;
        if(!c)
        {
            c = exalt_conn_new();
            not_c = 1;
        }

        if(not_c || exalt_eth_state_load(CONF_FILE, exalt_eth_get_udi(eth)) == EXALT_UP)
        {
            int i = 0;
            if(!exalt_eth_is_up(eth))
                exalt_eth_up(eth);
            while(!exalt_eth_is_up(eth) && i<10)
            {
                printf("WAIT\n");
                usleep(500);
                i++;
            }

            if(exalt_eth_is_link(eth) && exalt_eth_is_up(eth))
                exalt_eth_apply_conn(eth, c);
        }
        else
        {
            exalt_eth_down(eth);
        }
    }

    if (action == EXALT_ETH_CB_ACTION_LINK && exalt_eth_is_up(eth))
    {
        Exalt_Connection *c = exalt_eth_conn_load(CONF_FILE, exalt_eth_get_name(eth));
        if(!c)
            c = exalt_conn_new();
        if(exalt_eth_is_wireless(eth))
            exalt_conn_set_wireless(c, 1);
        else
            exalt_conn_set_wireless(c, 0);

        exalt_eth_apply_conn(eth, c);
        exalt_eth_save(CONF_FILE,eth);
    }

    if( action == EXALT_ETH_CB_ACTION_UP || action == EXALT_ETH_CB_ACTION_DOWN)
        exalt_eth_save(CONF_FILE, eth);


    if( action==EXALT_ETH_CB_ACTION_CONN_APPLY_DONE)
    {
        //save the configuration
        exalt_eth_save(CONF_FILE, eth);
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

    //waiting card
    if(waiting_card_list && waiting_card_is(eth) && action == EXALT_ETH_CB_ACTION_ADDRESS_NEW && strcmp(exalt_eth_get_ip(eth), "0.0.0.0")!=0 )
    {
        waiting_card_remove(eth);
        if(waiting_card_is_done())
        {
            //stop the timeout
            int no_timeout = 1;
            waiting_card_stop(&no_timeout);
        }
    }
}

void wireless_scan_cb(Exalt_Ethernet* eth,Ecore_List* new_networks, Ecore_List* old_networks, void* data)
{
    E_DBus_Connection *conn;
    DBusMessage* msg;
    DBusMessageIter args;
    const char* name;
    const char* essid;
    Exalt_Wireless_Info *wi;
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


Exalt_Ethernet* dbus_get_eth(DBusMessage* msg)
{
    DBusMessageIter args;
    char* interface= NULL;
    Exalt_Ethernet* eth;

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

Exalt_Wireless_Info* dbus_get_wirelessinfo(DBusMessage* msg)
{
    DBusMessageIter args;
    char* interface = NULL;
    Exalt_Ethernet* eth;
    char* essid = NULL;
    Exalt_Wireless_Info* wi;

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

Exalt_Wireless_Info* get_wirelessinfo(Exalt_Ethernet* eth, char* essid)
{
    Exalt_Wireless* w;
    Exalt_Wireless_Info* wi;
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
        wi = Exalt_Wireless_Info(data);
        if( strcmp(essid,exalt_wirelessinfo_get_essid(wi))==0 )
            return wi;
    }
    return NULL;
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


int waiting_card_is(const Exalt_Ethernet* eth)
{
    char* name;
    int find = 0;
    ecore_list_first_goto(waiting_card_list);
    while(!find && (name = ecore_list_next(waiting_card_list)))
    {
        if(strcmp(exalt_eth_get_name(eth),name) == 0)
            find = 1;
    }
    return find;
}

void waiting_card_remove(const Exalt_Ethernet* eth)
{
    //remove the card if the list
    char* name;
    int find = 0;

    ecore_list_first_goto(waiting_card_list);
    while(!find && (name = ecore_list_next(waiting_card_list)))
    {
        if( strcmp(exalt_eth_get_name(eth),name) == 0)
            find = 1;
    }

    if(!find)
        //the card is not in the list
        return ;

    ecore_list_index_goto(waiting_card_list,ecore_list_index(waiting_card_list) -1);
    ecore_list_remove(waiting_card_list);

    printf("Waiting card done: %s %s\n",exalt_eth_get_name(eth),exalt_eth_get_ip(eth));
}

void waiting_card_load(const char* file)
{
    char* name;
    waiting_card_list = ecore_list_new();
 //   ecore_list_append(waiting_card_list,"eth0");
 //   ecore_list_append(waiting_card_list,"eth10");

    printf("file: %s\n",file);

    ecore_list_first_goto(waiting_card_list);
    while( (name = ecore_list_next(waiting_card_list)) )
        printf("Waiting the card: %s\n",name);

    if(ecore_list_empty_is(waiting_card_list))
        EXALT_CLEAR_LIST(waiting_card_list);
}

int waiting_card_is_done()
{
    char* name;
    ecore_list_first_goto(waiting_card_list);
    while (( name= ecore_list_next(waiting_card_list)))
        if(exalt_eth_get_ethernet_byname(name))
            return 0;

    return 1;
}

int waiting_card_stop(void* data)
{
    if( (*(int*)data) ==0)
        printf("Timeout, continue boot process ...\n");
    else
        printf("All waiting card done, continue boot process ...\n");

    EXALT_DELETE_TIMER(waiting_card_timer);
    EXALT_CLEAR_LIST(waiting_card_list);
    ecore_main_loop_quit();
    return 0;
}
