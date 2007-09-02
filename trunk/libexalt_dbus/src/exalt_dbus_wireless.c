/*
 * =====================================================================================
 *
 *       Filename:  exalt_dbus_wireless.c
 *
 *    Description:  All functions about a wireless device
 *
 *        Version:  1.0
 *        Created:  08/31/2007 12:34:48 PM CEST
 *       Revision:  none
 *       Compiler:  gcc
 *
 *         Author:   (Watchwolf), Atton Jonathan <watchwolf@watchwolf.fr>
 *        Company:
 *
 * =====================================================================================
 */

#include "exalt_dbus_wireless.h"

Ecore_List* exalt_dbus_wireless_scan_wait(exalt_dbus_conn* conn, char* eth)
{
    DBusPendingCall * ret;
    DBusMessage *msg;
    DBusMessageIter args;
    Ecore_List* res;

    if(!conn || !eth)
    {
        char buf[1024];
        sprintf(buf,"conn==%p, eth==%p \n",conn, eth);
        print_error("ERROR", __FILE__,__LINE__,__func__,buf);
        return 0;
    }

    msg = exalt_dbus_read_call_new("IFACE_SCAN_WAIT");
    dbus_message_iter_init_append(msg, &args);
    if (!dbus_message_iter_append_basic(&args, DBUS_TYPE_STRING, &eth)) {
        print_error("ERROR", __FILE__,__LINE__,__func__,"Out Of Memory!");
        dbus_message_unref(msg);
        return 0;
    }
    dbus_connection_send_with_reply (conn->conn, msg, &ret, -1);
    dbus_pending_call_block(ret);
    msg = dbus_pending_call_steal_reply(ret);
    dbus_pending_call_unref(ret);

    //read the response
    res = exalt_dbus_response_strings(msg);
    dbus_message_unref(msg);
    return res;
}

