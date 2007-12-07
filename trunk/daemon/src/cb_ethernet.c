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
    Exalt_Ethernet* eth;
    const char* interface;
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
        eth = Exalt_Ethernet(data);
        interface = exalt_eth_get_name(eth);
        if(!interface)
        {
            print_error("WARNING", __FILE__, __LINE__,__func__, "interface=%p",interface);
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



DBusMessage * dbus_cb_eth_get_ip(E_DBus_Object *obj __UNUSED__, DBusMessage *msg)
{
    DBusMessage *reply;
    DBusMessageIter args;
    Exalt_Ethernet* eth;
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
        EXALT_FREE(ip);
        return reply;
    }
    EXALT_FREE(ip);
    return reply;
}

DBusMessage * dbus_cb_eth_get_netmask(E_DBus_Object *obj __UNUSED__, DBusMessage *msg)
{
    DBusMessage *reply;
    DBusMessageIter args;
    Exalt_Ethernet* eth;
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
        EXALT_FREE(netmask);
        return reply;
    }

    EXALT_FREE(netmask);
    return reply;
}

DBusMessage * dbus_cb_eth_get_gateway(E_DBus_Object *obj __UNUSED__, DBusMessage *msg)
{
    DBusMessage *reply;
    DBusMessageIter args;
    Exalt_Ethernet* eth;
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
        EXALT_FREE(gateway);
        return reply;
    }

    //free -> segfault
    EXALT_FREE(gateway);
    return reply;
}

DBusMessage * dbus_cb_eth_is_wireless(E_DBus_Object *obj __UNUSED__, DBusMessage *msg)
{
    DBusMessage *reply;
    DBusMessageIter args;
    Exalt_Ethernet* eth;
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
    Exalt_Ethernet* eth;
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
    Exalt_Ethernet* eth;
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
    Exalt_Ethernet* eth;
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
    Exalt_Ethernet* eth;


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
    Exalt_Ethernet* eth;


    reply = dbus_message_new_method_return(msg);

    //search the interface
    eth = dbus_get_eth(msg);
    if(!eth)
        return reply;

    exalt_eth_down(eth);

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

DBusMessage * dbus_cb_eth_apply_conn(E_DBus_Object *obj __UNUSED__, DBusMessage *msg)
{
    DBusMessage *reply;
    DBusMessageIter args;
    Exalt_Ethernet* eth;
    Exalt_Connection* c;
    reply = dbus_message_new_method_return(msg);
    int i;
    char* s;

    c = exalt_conn_new();
    if(!c)
    {
        print_error("ERROR", __FILE__, __LINE__,__func__, "c=%p",c);
        return reply;
    }

    eth = dbus_get_eth(msg);

    if(!eth)
        return reply;

    //retrieve the connection
    if(!dbus_message_iter_init(msg, &args))
    {
        print_error("ERROR", __FILE__, __LINE__,__func__, "no argument");
        return reply;
    }
    dbus_message_iter_next(&args);
    if (DBUS_TYPE_INT32 != dbus_message_iter_get_arg_type(&args))
    {
        print_error("ERROR", __FILE__, __LINE__,__func__, "Argument is not a int");
        return reply;
    }
    else
        dbus_message_iter_get_basic(&args, &i);
    exalt_conn_set_mode(c,i);
    dbus_message_iter_next(&args);

    if(!exalt_conn_is_dhcp(c))
    {
        if (DBUS_TYPE_STRING != dbus_message_iter_get_arg_type(&args))
        {
            print_error("ERROR", __FILE__, __LINE__,__func__, "Argument is not a string");
            return reply;
        }
        else
            dbus_message_iter_get_basic(&args, &s);
        exalt_conn_set_ip(c,s);
        dbus_message_iter_next(&args);

        if (DBUS_TYPE_STRING != dbus_message_iter_get_arg_type(&args))
        {
            print_error("ERROR", __FILE__, __LINE__,__func__, "Argument is not a string");
            return reply;
        }
        else
            dbus_message_iter_get_basic(&args, &s);
        exalt_conn_set_netmask(c,s);
        dbus_message_iter_next(&args);

        if (DBUS_TYPE_STRING != dbus_message_iter_get_arg_type(&args))
        {
            print_error("ERROR", __FILE__, __LINE__,__func__, "Argument is not a string");
            return reply;
        }
        else
            dbus_message_iter_get_basic(&args, &s);
        exalt_conn_set_gateway(c,s);
        dbus_message_iter_next(&args);
    }

    if (DBUS_TYPE_INT32 != dbus_message_iter_get_arg_type(&args))
    {
        print_error("ERROR", __FILE__, __LINE__,__func__, "Argument is not a int");
        return reply;
    }
    else
        dbus_message_iter_get_basic(&args, &i);
    exalt_conn_set_wireless(c,i);
    dbus_message_iter_next(&args);

    if(exalt_conn_is_wireless(c))
    {
        if (DBUS_TYPE_STRING != dbus_message_iter_get_arg_type(&args))
        {
            print_error("ERROR", __FILE__, __LINE__,__func__, "Argument is not a string");
            return reply;
        }
        else
            dbus_message_iter_get_basic(&args, &s);
        exalt_conn_set_essid(c,s);
        dbus_message_iter_next(&args);


        if (DBUS_TYPE_INT32 != dbus_message_iter_get_arg_type(&args))
        {
            print_error("ERROR", __FILE__, __LINE__,__func__, "Argument is not a int");
            return reply;
        }
        else
            dbus_message_iter_get_basic(&args, &i);
        exalt_conn_set_encryption_mode(c,i);
        dbus_message_iter_next(&args);

        if(exalt_conn_get_encryption_mode(c)!= EXALT_ENCRYPTION_NONE)
        {
            if (DBUS_TYPE_STRING != dbus_message_iter_get_arg_type(&args))
            {
                print_error("ERROR", __FILE__, __LINE__,__func__, "Argument is not a string");
                return reply;
            }
            else
                dbus_message_iter_get_basic(&args, &s);
            exalt_conn_set_key(c,s);
            dbus_message_iter_next(&args);
        }

        if (DBUS_TYPE_INT32 != dbus_message_iter_get_arg_type(&args))
        {
            print_error("ERROR", __FILE__, __LINE__,__func__, "Argument is not a int");
            return reply;
        }
        else
            dbus_message_iter_get_basic(&args, &i);
        exalt_conn_set_connection_mode(c,i);
        dbus_message_iter_next(&args);


        if (DBUS_TYPE_INT32 != dbus_message_iter_get_arg_type(&args))
        {
            print_error("ERROR", __FILE__, __LINE__,__func__, "Argument is not a int");
            return reply;
        }
        else
            dbus_message_iter_get_basic(&args, &i);
        exalt_conn_set_security_mode(c,i);
        dbus_message_iter_next(&args);
    }

    exalt_eth_apply_conn(eth, c);

    //if it's a wireless connection
    //we save the configuration for the essid
    if(exalt_conn_is_wireless(c))
        exalt_wireless_conn_save(CONF_FILE, c);

    return reply;
}
