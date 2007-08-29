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

#include "cb_functions.h"

DBusMessage * dbus_cb_get_eth_list(E_DBus_Object *obj, DBusMessage *msg)
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
        fprintf(stderr,"dbus_cb_get_eth_list(): interfaces ==null! \n");
        return reply;
    }

    ecore_list_first_goto(interfaces);
    while( (data=ecore_list_next(interfaces)))
    {
        eth = EXALT_ETHERNET(data);
        interface = exalt_eth_get_name(eth);
        if (!dbus_message_iter_append_basic(&args, DBUS_TYPE_STRING, &interface))
        {
            fprintf(stderr, "dbus_cb_get_eth_list(): Out Of Memory!\n");
            return reply;
        }
    }
    return reply;
}



DBusMessage * dbus_cb_get_ip(E_DBus_Object *obj, DBusMessage *msg)
{
    DBusMessage *reply;
    DBusMessageIter args;
    char* interface;
    exalt_ethernet* eth;
    char* ip;

    if(!dbus_message_iter_init(msg, &args))
        fprintf(stderr,"dbus_cb_get_ip(): no argument !! \n");

    if (DBUS_TYPE_STRING != dbus_message_iter_get_arg_type(&args))
        fprintf(stderr, "dbus_cb_get_ip(): Argument is not string!\n");
    else
        dbus_message_iter_get_basic(&args, &interface);

    reply = dbus_message_new_method_return(msg);

    //search the interface
    eth = exalt_eth_get_ethernet_byname(interface);
    if(!eth)
    {
        fprintf(stderr,"dbus_cb_get_ip(): the interface %s doesn't exist\n",interface);
        return reply;
    }

    dbus_message_iter_init_append(reply, &args);
    ip = exalt_eth_get_ip(eth);
    if (!dbus_message_iter_append_basic(&args, DBUS_TYPE_STRING, &ip))
    {
        fprintf(stderr, "dbus_cb_get_ip(): Out Of Memory!\n");
        return reply;
    }

    return reply;
}

DBusMessage * dbus_cb_get_netmask(E_DBus_Object *obj, DBusMessage *msg)
{
    DBusMessage *reply;
    DBusMessageIter args;
    char* interface;
    exalt_ethernet* eth;
    char* netmask;

    if(!dbus_message_iter_init(msg, &args))
        fprintf(stderr,"dbus_cb_get_netmask(): no argument !! \n");

    if (DBUS_TYPE_STRING != dbus_message_iter_get_arg_type(&args))
        fprintf(stderr, "dbus_cb_get_netmask(): Argument is not string!\n");
    else
        dbus_message_iter_get_basic(&args, &interface);

    reply = dbus_message_new_method_return(msg);

    //search the interface
    eth = exalt_eth_get_ethernet_byname(interface);
    if(!eth)
    {
        fprintf(stderr,"dbus_cb_get_netmask(): the interface %s doesn't exist\n",interface);
        return reply;
    }

    dbus_message_iter_init_append(reply, &args);
    netmask = exalt_eth_get_netmask(eth);
    if (!dbus_message_iter_append_basic(&args, DBUS_TYPE_STRING, &netmask))
    {
        fprintf(stderr, "dbus_cb_get_ip(): Out Of Memory!\n");
        return reply;
    }

    return reply;
}

DBusMessage * dbus_cb_get_gateway(E_DBus_Object *obj, DBusMessage *msg)
{
    DBusMessage *reply;
    DBusMessageIter args;
    char* interface;
    exalt_ethernet* eth;
    char* gateway;

    if(!dbus_message_iter_init(msg, &args))
        fprintf(stderr,"dbus_cb_get_gateway(): no argument !! \n");

    if (DBUS_TYPE_STRING != dbus_message_iter_get_arg_type(&args))
        fprintf(stderr, "dbus_cb_get_gateway(): Argument is not string!\n");
    else
        dbus_message_iter_get_basic(&args, &interface);

    reply = dbus_message_new_method_return(msg);

    //search the interface
    eth = exalt_eth_get_ethernet_byname(interface);
    if(!eth)
    {
        fprintf(stderr,"dbus_cb_get_gateway(): the interface %s doesn't exist\n",interface);
        return reply;
    }

    dbus_message_iter_init_append(reply, &args);
    gateway = exalt_eth_get_gateway(eth);
    if (!dbus_message_iter_append_basic(&args, DBUS_TYPE_STRING, &gateway))
    {
        fprintf(stderr, "dbus_cb_get_ip(): Out Of Memory!\n");
        return reply;
    }

    return reply;
}

DBusMessage * dbus_cb_is_wireless(E_DBus_Object *obj, DBusMessage *msg)
{
    DBusMessage *reply;
    DBusMessageIter args;
    char* interface;
    exalt_ethernet* eth;
    int is;

    if(!dbus_message_iter_init(msg, &args))
        fprintf(stderr,"dbus_cb_is_wireless(): no argument !! \n");

    if (DBUS_TYPE_STRING != dbus_message_iter_get_arg_type(&args))
        fprintf(stderr, "dbus_cb_is_wireless(): Argument is not string!\n");
    else
        dbus_message_iter_get_basic(&args, &interface);

    reply = dbus_message_new_method_return(msg);

    //search the interface
    eth = exalt_eth_get_ethernet_byname(interface);
    if(!eth)
    {
        fprintf(stderr,"dbus_cb_is_wireless(): the interface %s doesn't exist\n",interface);
        return reply;
    }

    dbus_message_iter_init_append(reply, &args);
    is = exalt_eth_is_wireless(eth);
    if (!dbus_message_iter_append_basic(&args, DBUS_TYPE_BOOLEAN, &is))
    {
        fprintf(stderr, "dbus_cb_is_wireless(): Out Of Memory!\n");
        return reply;
    }

    return reply;
}

DBusMessage * dbus_cb_is_link(E_DBus_Object *obj, DBusMessage *msg)
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
        fprintf(stderr, "dbus_cb_is_wireless(): Out Of Memory!\n");
        return reply;
    }

    return reply;
}

