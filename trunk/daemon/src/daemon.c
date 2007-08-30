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

    e_dbus_interface_method_add(iface, "GET_IP", NULL, NULL, dbus_cb_get_ip);
    e_dbus_interface_method_add(iface, "GET_NETMASK", NULL, NULL, dbus_cb_get_netmask);
    e_dbus_interface_method_add(iface, "GET_GATEWAY", NULL, NULL, dbus_cb_get_gateway);
    e_dbus_interface_method_add(iface, "GET_ETH_LIST", NULL, NULL, dbus_cb_get_eth_list);
    e_dbus_interface_method_add(iface, "IS_WIRELESS", NULL, NULL, dbus_cb_is_wireless);
    e_dbus_interface_method_add(iface, "IS_LINK", NULL, NULL, dbus_cb_is_link);
    e_dbus_interface_method_add(iface, "IS_UP", NULL, NULL, dbus_cb_is_up);

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
            fprintf(stderr,"Can not create the pid file: %s\n",EXALTD_PIDFILE);

        //redirect stderr and stdout >> /var/log/exald.log
        remove(EXALTD_LOGFILE);
        if ((fp = fopen(EXALTD_LOGFILE, "w+")))
        {
            stderr = fp;
            stdout = fp;
        }
        else
            fprintf(stderr,"Can not create the log file: %s\n",EXALTD_LOGFILE);
    }
    fprintf(stdout,"INIT EXALT_DAEMON\n");
    e_dbus_init();
    ecore_init();
    exalt_eth_init();

    fprintf(stdout,"SETUP DBUS SERVER\n");
    conn = e_dbus_bus_get(DBUS_BUS_SYSTEM);
    if(!conn)
    {
        fprintf(stderr,"main(): can not connect to DBUS, maybe the daemon is not launch ?\n");
        e_dbus_shutdown();
        ecore_shutdown();
        return -1;
    }

    setup(conn);

    exalt_eth_set_cb(eth_cb,conn);
    exalt_main();

    if(!exalt_is_admin())
        fprintf(stderr,"The daemon is not run with administrator's right, you can't modify the configuration\n");

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
        fprintf(stderr, "eth_cb(): msg==null !\n");
        return ;
    }

    name = exalt_eth_get_name(eth);

    dbus_message_iter_init_append(msg, &args);
    if (!dbus_message_iter_append_basic(&args, DBUS_TYPE_STRING, &name))
        fprintf(stderr,"eth_cb(): Out Of Memory !\n");

    if (!dbus_message_iter_append_basic(&args, DBUS_TYPE_UINT32, &action))
        fprintf(stderr,"eth_cb(): Out Of Memory !\n");

    e_dbus_message_send(conn, msg, NULL, 3,NULL);
}


exalt_ethernet* dbus_get_eth(DBusMessage* msg)
{
    DBusMessageIter args;
    char* interface;
    exalt_ethernet* eth;

    if(!dbus_message_iter_init(msg, &args))
        fprintf(stderr,"dbus_get_eth(): no argument !! \n");

    if (DBUS_TYPE_STRING != dbus_message_iter_get_arg_type(&args))
        fprintf(stderr, "dbus_get_eth(): Argument is not string!\n");
    else
        dbus_message_iter_get_basic(&args, &interface);

    //search the interface
    eth = exalt_eth_get_ethernet_byname(interface);
    if(!eth)
    {
        fprintf(stderr,"dbus_get_eth(): the interface %s doesn't exist\n",interface);
        return NULL;
    }
    return eth;
}
