/*
 * =====================================================================================
 *
 *       Filename:  cb_wireless.c
 *
 *    Description:  All CB about a wireless interface
 *
 *        Version:  1.0
 *        Created:  09/01/2007 12:05:24 PM CEST
 *       Revision:  none
 *       Compiler:  gcc
 *
 *         Author:   (Watchwolf), Atton Jonathan <watchwolf@watchwolf.fr>
 *        Company:
 *
 * =====================================================================================
 */

#include "cb_wireless.h"

DBusMessage * dbus_cb_wireless_scan_wait(E_DBus_Object *obj __UNUSED__, DBusMessage *msg)
{
    DBusMessage *reply;
    DBusMessageIter args;
    Exalt_Ethernet* eth;
    Ecore_List* l;
    Exalt_Wireless *w;
    void* data;
    char* essid;
    Exalt_Wireless_Info* wi;
    reply = dbus_message_new_method_return(msg);

    //search the interface
    eth = dbus_get_eth(msg);
    if(!eth || !exalt_eth_is_wireless(eth))
        return reply;

    w = exalt_eth_get_wireless(eth);

    dbus_message_iter_init_append(reply, &args);

    exalt_wireless_scan_wait(eth);
    l = exalt_wireless_get_networks_list(w);
    ecore_list_first_goto(l);

    while( (data=ecore_list_next(l)))
    {
        wi = Exalt_Wireless_Info(data);
        essid = strdup(exalt_wirelessinfo_get_essid(wi));
        if(!essid)
        {
            print_error("WARNING", __FILE__, __LINE__,__func__, "essid=%p",essid);
            return reply;
        }
        if (!dbus_message_iter_append_basic(&args, DBUS_TYPE_STRING, &essid))
        {
            print_error("ERROR", __FILE__, __LINE__,__func__, "Out Of Memory");
            EXALT_FREE(essid);
            return reply;
        }
        EXALT_FREE(essid);
    }
    return reply;
}

DBusMessage * dbus_cb_wireless_get_essid(E_DBus_Object *obj __UNUSED__, DBusMessage *msg)
{
    DBusMessage *reply;
    DBusMessageIter args;
    Exalt_Ethernet* eth;
    const char* essid;

    reply = dbus_message_new_method_return(msg);

    eth= dbus_get_eth(msg);

    if(!eth)
        return reply;

    dbus_message_iter_init_append(reply, &args);
    essid = exalt_wireless_get_essid(exalt_eth_get_wireless(eth));
    if(!essid)
    {
        print_error("WARNING", __FILE__, __LINE__,__func__, "essid=%p",essid);
        return reply;
    }
    if (!dbus_message_iter_append_basic(&args, DBUS_TYPE_STRING, &essid))
    {
        print_error("ERROR", __FILE__, __LINE__,__func__, "Out Of Memory");
        return reply;
    }
    return reply;
}


DBusMessage * dbus_cb_wireless_scan_start(E_DBus_Object *obj __UNUSED__, DBusMessage *msg)
{
    DBusMessage *reply;
    Exalt_Ethernet* eth;
    reply = dbus_message_new_method_return(msg);


    //search the interface
    eth = dbus_get_eth(msg);
    if(!eth || !exalt_eth_is_wireless(eth))
        return reply;

    exalt_wireless_scan_start(eth);

    return reply;
}


DBusMessage * dbus_cb_wireless_scan_stop(E_DBus_Object *obj __UNUSED__, DBusMessage *msg)
{
    DBusMessage *reply;
    Exalt_Ethernet* eth;
    reply = dbus_message_new_method_return(msg);

    //search the interface
    eth = dbus_get_eth(msg);
    if(!eth || !exalt_eth_is_wireless(eth))
        return reply;


    exalt_wireless_scan_stop(eth);

    return reply;
}

