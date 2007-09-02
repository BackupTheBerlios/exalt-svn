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

DBusMessage * dbus_cb_wireless_scan_wait(E_DBus_Object *obj, DBusMessage *msg)
{
    DBusMessage *reply;
    DBusMessageIter args;
    exalt_ethernet* eth;
    Ecore_List* l;
    exalt_wireless* w;
    void* data;
    char* essid;
    exalt_wireless_info* wi;
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
        wi = EXALT_WIRELESS_INFO(data);
        essid = exalt_wirelessinfo_get_essid(wi);
        if(!essid)
        {
            print_error("WARNING", __FILE__, __LINE__,__func__, "essid==null");
            return reply;
        }
        if (!dbus_message_iter_append_basic(&args, DBUS_TYPE_STRING, &essid))
        {
            print_error("ERROR", __FILE__, __LINE__,__func__, "Out Of Memory");
            return reply;
        }
    }

    return reply;
}

