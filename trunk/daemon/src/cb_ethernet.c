/*
 * =====================================================================================
 *
 *       Filename:  cb_functions.c
 *
 *    Description:  all callback functions are defined here
 *
 *        Version:  1.0
 *        Created:  08/29/2007 01:48:10 PM CEST
 *       Revision:  none
 *       Compiler:  gcc
 *
 *         Author:   (Watchwolf), Atton Jonathan <watchwolf@watchwolf.fr>
 *        Company:
 *
 * =====================================================================================
 */

#include "cb_ethernet.h"

DBusMessage * dbus_cb_eth_get_eth_list(E_DBus_Object *obj __UNUSED__, DBusMessage *msg)
{
    DBusMessage *reply;
    DBusMessageIter args;
    exalt_ethernet* eth;
    char* interface;
    void* data;
    Ecore_List *interfaces;

    reply = dbus_message_new_method_return(msg);

    dbus_message_iter_init_append(reply, &args);
    interfaces = exalt_eth_get_list();
    if(!interfaces)
    {
        print_error("WARNING", __FILE__, __LINE__,__func__, "interfaces=%p",interfaces);
        return reply;
    }

    ecore_list_first_goto(interfaces);
    while( (data=ecore_list_next(interfaces)))
    {
        eth = EXALT_ETHERNET(data);
        interface = strdup(exalt_eth_get_name(eth));
        if(!interface)
        {
            print_error("WARNING", __FILE__, __LINE__,__func__, "interface=%p",interface);
            return reply;
        }
        if (!interface || !dbus_message_iter_append_basic(&args, DBUS_TYPE_STRING, &interface))
        {
            print_error("ERROR", __FILE__, __LINE__,__func__, "Out Of Memory");
            EXALT_FREE(interface)
            return reply;
        }
    }
    return reply;
}



DBusMessage * dbus_cb_eth_get_ip(E_DBus_Object *obj __UNUSED__, DBusMessage *msg)
{
    DBusMessage *reply;
    DBusMessageIter args;
    exalt_ethernet* eth;
    char* ip;

    reply = dbus_message_new_method_return(msg);

    eth= dbus_get_eth(msg);

    if(!eth)
        return reply;

    dbus_message_iter_init_append(reply, &args);
    ip = exalt_eth_get_ip(eth);
    if(!ip)
    {
        print_error("WARNING", __FILE__, __LINE__,__func__, "ip=%p",ip);
        return reply;
    }
    if (!dbus_message_iter_append_basic(&args, DBUS_TYPE_STRING, &ip))
    {
        print_error("ERROR", __FILE__, __LINE__,__func__, "Out Of Memory");
        return reply;
    }

    return reply;
}

DBusMessage * dbus_cb_eth_get_netmask(E_DBus_Object *obj __UNUSED__, DBusMessage *msg)
{
    DBusMessage *reply;
    DBusMessageIter args;
    exalt_ethernet* eth;
    char* netmask;

    reply = dbus_message_new_method_return(msg);

    eth= dbus_get_eth(msg);

    if(!eth)
        return reply;

    dbus_message_iter_init_append(reply, &args);
    netmask = exalt_eth_get_netmask(eth);
    if(!netmask)
    {
        print_error("WARNING", __FILE__, __LINE__,__func__, "netmask=%p",netmask);
        return reply;
    }
    if (!dbus_message_iter_append_basic(&args, DBUS_TYPE_STRING, &netmask))
    {
        print_error("ERROR", __FILE__, __LINE__,__func__, "Out Of Memory");
        return reply;
    }

    return reply;
}

DBusMessage * dbus_cb_eth_get_gateway(E_DBus_Object *obj __UNUSED__, DBusMessage *msg)
{
    DBusMessage *reply;
    DBusMessageIter args;
    exalt_ethernet* eth;
    char* gateway;

    reply = dbus_message_new_method_return(msg);

    eth= dbus_get_eth(msg);

    if(!eth)
        return reply;

    dbus_message_iter_init_append(reply, &args);
    gateway = exalt_eth_get_gateway(eth);
    if(!gateway)
    {
        print_error("WARNING", __FILE__, __LINE__,__func__, "gateway=%p",gateway);
        return reply;
    }
    if (!dbus_message_iter_append_basic(&args, DBUS_TYPE_STRING, &gateway))
    {
        print_error("ERROR", __FILE__, __LINE__,__func__, "Out Of Memory");
        return reply;
    }

    return reply;
}

DBusMessage * dbus_cb_eth_is_wireless(E_DBus_Object *obj __UNUSED__, DBusMessage *msg)
{
    DBusMessage *reply;
    DBusMessageIter args;
    exalt_ethernet* eth;
    int is;


    reply = dbus_message_new_method_return(msg);

    //search the interface
    eth = dbus_get_eth(msg);
    if(!eth)
        return reply;

    dbus_message_iter_init_append(reply, &args);
    is = exalt_eth_is_wireless(eth);
    if (!dbus_message_iter_append_basic(&args, DBUS_TYPE_BOOLEAN, &is))
    {
        print_error("ERROR", __FILE__, __LINE__,__func__, "Out Of Memory");
        return reply;
    }

    return reply;
}

DBusMessage * dbus_cb_eth_is_link(E_DBus_Object *obj __UNUSED__, DBusMessage *msg)
{
    DBusMessage *reply;
    DBusMessageIter args;
    exalt_ethernet* eth;
    int is;


    reply = dbus_message_new_method_return(msg);

    //search the interface
    eth = dbus_get_eth(msg);
    if(!eth)
        return reply;

    dbus_message_iter_init_append(reply, &args);
    is = exalt_eth_is_link(eth);
    if (!dbus_message_iter_append_basic(&args, DBUS_TYPE_BOOLEAN, &is))
    {
        print_error("ERROR", __FILE__, __LINE__,__func__, "Out Of Memory");
        return reply;
    }

    return reply;
}

DBusMessage * dbus_cb_eth_is_up(E_DBus_Object *obj __UNUSED__, DBusMessage *msg)
{
    DBusMessage *reply;
    DBusMessageIter args;
    exalt_ethernet* eth;
    int is;


    reply = dbus_message_new_method_return(msg);

    //search the interface
    eth = dbus_get_eth(msg);
    if(!eth)
        return reply;

    dbus_message_iter_init_append(reply, &args);
    is = exalt_eth_is_up(eth);
    if (!dbus_message_iter_append_basic(&args, DBUS_TYPE_BOOLEAN, &is))
    {
        print_error("ERROR", __FILE__, __LINE__,__func__, "Out Of Memory");
        return reply;
    }

    return reply;
}


DBusMessage * dbus_cb_eth_is_dhcp(E_DBus_Object *obj __UNUSED__, DBusMessage *msg)
{
    DBusMessage *reply;
    DBusMessageIter args;
    exalt_ethernet* eth;
    int is;


    reply = dbus_message_new_method_return(msg);

    //search the interface
    eth = dbus_get_eth(msg);
    if(!eth)
        return reply;

    dbus_message_iter_init_append(reply, &args);
    is = exalt_eth_is_dhcp(eth);
    if (!dbus_message_iter_append_basic(&args, DBUS_TYPE_BOOLEAN, &is))
    {
        print_error("ERROR", __FILE__, __LINE__,__func__, "Out Of Memory");
        return reply;
    }

    return reply;
}















DBusMessage * dbus_cb_eth_up(E_DBus_Object *obj __UNUSED__, DBusMessage *msg)
{
    DBusMessage *reply;
    exalt_ethernet* eth;


    reply = dbus_message_new_method_return(msg);

    //search the interface
    eth = dbus_get_eth(msg);
    if(!eth)
        return reply;

    exalt_eth_up(eth);

    return reply;
}

DBusMessage * dbus_cb_eth_down(E_DBus_Object *obj __UNUSED__, DBusMessage *msg)
{
    DBusMessage *reply;
    exalt_ethernet* eth;


    reply = dbus_message_new_method_return(msg);

    //search the interface
    eth = dbus_get_eth(msg);
    if(!eth)
        return reply;

    exalt_eth_down(eth);

    return reply;
}

DBusMessage * dbus_cb_eth_set_new_ip(E_DBus_Object *obj __UNUSED__, DBusMessage *msg)
{
    DBusMessage *reply;
    DBusMessageIter args;
    exalt_ethernet* eth;
    char* ip;

    reply = dbus_message_new_method_return(msg);

    //search the interface
    eth = dbus_get_eth(msg);
    if(!eth)
        return reply;

    //get the new ip
    if(!dbus_message_iter_init(msg, &args))
    {
        print_error("ERROR", __FILE__, __LINE__,__func__, "no argument");
        return reply;
    }
    dbus_message_iter_next(&args);
    if (DBUS_TYPE_STRING != dbus_message_iter_get_arg_type(&args))
    {
        print_error("ERROR", __FILE__, __LINE__,__func__, "Argument is not a string");
        return reply;
    }
    else
        dbus_message_iter_get_basic(&args, &ip);

    if(!exalt_is_address(ip))
    {
        print_error("WARNING", __FILE__, __LINE__,__func__, "The address (%s) is not an ip address",ip);
        return reply;
    }

    exalt_eth_set_new_ip(eth,ip);

    return reply;
}

DBusMessage * dbus_cb_eth_set_new_netmask(E_DBus_Object *obj __UNUSED__, DBusMessage *msg)
{
    DBusMessage *reply;
    DBusMessageIter args;
    exalt_ethernet* eth;
    char* netmask;

    reply = dbus_message_new_method_return(msg);

    //search the interface
    eth = dbus_get_eth(msg);
    if(!eth)
        return reply;

    //get the new netmask
    if(!dbus_message_iter_init(msg, &args))
    {
        print_error("ERROR", __FILE__, __LINE__,__func__, "no argument");
        return reply;
    }
    dbus_message_iter_next(&args);
    if (DBUS_TYPE_STRING != dbus_message_iter_get_arg_type(&args))
    {
        print_error("ERROR", __FILE__, __LINE__,__func__, "Argument is not a string");
        return reply;
    }
    else
        dbus_message_iter_get_basic(&args, &netmask);

    if(!exalt_is_address(netmask))
    {
        print_error("WARNING", __FILE__, __LINE__,__func__, "The address (%s) is not an netmask address",netmask);
        return reply;
    }

    exalt_eth_set_new_netmask(eth,netmask);

    return reply;
}

DBusMessage * dbus_cb_eth_set_new_gateway(E_DBus_Object *obj __UNUSED__, DBusMessage *msg)
{
    DBusMessage *reply;
    DBusMessageIter args;
    exalt_ethernet* eth;
    char* gateway;

    reply = dbus_message_new_method_return(msg);

    //search the interface
    eth = dbus_get_eth(msg);
    if(!eth)
        return reply;

    //get the new gateway
    if(!dbus_message_iter_init(msg, &args))
    {
        print_error("ERROR", __FILE__, __LINE__,__func__, "no argument");
        return reply;
    }
    dbus_message_iter_next(&args);
    if (DBUS_TYPE_STRING != dbus_message_iter_get_arg_type(&args))
    {
        print_error("ERROR", __FILE__, __LINE__,__func__, "Argument is not a string");
        return reply;
    }
    else
        dbus_message_iter_get_basic(&args, &gateway);

    if(!exalt_is_address(gateway))
    {
        print_error("WARNING", __FILE__, __LINE__,__func__, "The address (%s) is not an gateway address",gateway);
        return reply;
    }

    exalt_eth_set_new_gateway(eth,gateway);

    return reply;
}

DBusMessage * dbus_cb_eth_set_new_dhcp(E_DBus_Object *obj __UNUSED__, DBusMessage *msg)
{
    DBusMessage *reply;
    DBusMessageIter args;
    exalt_ethernet* eth;
    int dhcp;

    reply = dbus_message_new_method_return(msg);

    //search the interface
    eth = dbus_get_eth(msg);
    if(!eth)
        return reply;

    //get the new dhcp
    if(!dbus_message_iter_init(msg, &args))
    {
        print_error("ERROR", __FILE__, __LINE__,__func__, "no argument");
        return reply;
    }

    dbus_message_iter_next(&args);
    if (DBUS_TYPE_BOOLEAN != dbus_message_iter_get_arg_type(&args))
    {
        print_error("ERROR", __FILE__, __LINE__,__func__, "Argument is not a boolean");
        return reply;
    }
    else
        dbus_message_iter_get_basic(&args, &dhcp);

    exalt_eth_set_new_dhcp(eth,dhcp);

    return reply;
}

struct E_DBus_Object
{
    E_DBus_Connection *conn;
    char *path;
    Ecore_List *interfaces;
    char *introspection_data;
    int introspection_dirty;

    E_DBus_Object_Property_Get_Cb cb_property_get;
    E_DBus_Object_Property_Set_Cb cb_property_set;

    void *data;
};

DBusMessage * dbus_cb_eth_apply_conf(E_DBus_Object *obj __UNUSED__, DBusMessage *msg)
{
    DBusMessage *reply;
    DBusMessageIter args;
    exalt_ethernet* eth;

    reply = dbus_message_new_method_return(msg);

    eth= dbus_get_eth(msg);

    if(!eth)
        return reply;

    dbus_message_iter_init_append(reply, &args);
    exalt_eth_apply_conf(eth, dbus_cb_notify_conf_applied_cb, obj->conn);

    return reply;
}

void dbus_cb_notify_conf_applied_cb(exalt_ethernet* eth, void* data)
{
    E_DBus_Connection *conn;
    DBusMessage* msg;
    DBusMessageIter args;
    const char* name;

    conn = (E_DBus_Connection*) data;

    //send a broadcast
    msg = dbus_message_new_signal(EXALTD_PATH,EXALTD_INTERFACE_READ, "NOTIFY_CONF_APPLIED");
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
    e_dbus_message_send(conn, msg, NULL, 3,NULL);
    dbus_message_unref(msg);
}

