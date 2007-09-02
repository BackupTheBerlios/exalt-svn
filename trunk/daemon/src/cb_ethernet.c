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

DBusMessage * dbus_cb_eth_get_eth_list(E_DBus_Object *obj, DBusMessage *msg)
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
        print_error("WARNING", __FILE__, __LINE__,__func__, "interfaces==null");
        return reply;
    }

    ecore_list_first_goto(interfaces);
    while( (data=ecore_list_next(interfaces)))
    {
        eth = EXALT_ETHERNET(data);
        interface = exalt_eth_get_name(eth);
        if(!interface)
        {
            print_error("WARNING", __FILE__, __LINE__,__func__, "interface==null");
            return reply;
        }
        if (!interface || !dbus_message_iter_append_basic(&args, DBUS_TYPE_STRING, &interface))
        {
            print_error("ERROR", __FILE__, __LINE__,__func__, "Out Of Memory");
            return reply;
        }
    }
    return reply;
}



DBusMessage * dbus_cb_eth_get_ip(E_DBus_Object *obj, DBusMessage *msg)
{
    DBusMessage *reply;
    DBusMessageIter args;
    char* interface;
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
        print_error("WARNING", __FILE__, __LINE__,__func__, "ip==null");
        return reply;
    }
    if (!dbus_message_iter_append_basic(&args, DBUS_TYPE_STRING, &ip))
    {
        print_error("ERROR", __FILE__, __LINE__,__func__, "Out Of Memory");
        return reply;
    }

    return reply;
}

DBusMessage * dbus_cb_eth_get_netmask(E_DBus_Object *obj, DBusMessage *msg)
{
    DBusMessage *reply;
    DBusMessageIter args;
    char* interface;
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
        print_error("WARNING", __FILE__, __LINE__,__func__, "netmask==null");
        return reply;
    }
    if (!dbus_message_iter_append_basic(&args, DBUS_TYPE_STRING, &netmask))
    {
        print_error("ERROR", __FILE__, __LINE__,__func__, "Out Of Memory");
        return reply;
    }

    return reply;
}

DBusMessage * dbus_cb_eth_get_gateway(E_DBus_Object *obj, DBusMessage *msg)
{
    DBusMessage *reply;
    DBusMessageIter args;
    char* interface;
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
        print_error("WARNING", __FILE__, __LINE__,__func__, "gateway==null");
        return reply;
    }
    if (!dbus_message_iter_append_basic(&args, DBUS_TYPE_STRING, &gateway))
    {
        print_error("ERROR", __FILE__, __LINE__,__func__, "Out Of Memory");
        return reply;
    }

    return reply;
}

DBusMessage * dbus_cb_eth_is_wireless(E_DBus_Object *obj, DBusMessage *msg)
{
    DBusMessage *reply;
    DBusMessageIter args;
    char* interface;
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

DBusMessage * dbus_cb_eth_is_link(E_DBus_Object *obj, DBusMessage *msg)
{
    DBusMessage *reply;
    DBusMessageIter args;
    char* interface;
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

DBusMessage * dbus_cb_eth_is_up(E_DBus_Object *obj, DBusMessage *msg)
{
    DBusMessage *reply;
    DBusMessageIter args;
    char* interface;
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
