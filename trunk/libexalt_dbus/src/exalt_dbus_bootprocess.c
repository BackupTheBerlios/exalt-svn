/*
 * =====================================================================================
 *
 *       Filename:  exalt_dbus_bootprocess.c
 *
 *    Description:  manage the boot process, during the boot process the system can wait a card.
 *
 *        Version:  1.0
 *        Created:  01/04/2008 01:00:19 PM CET
 *       Revision:  none
 *       Compiler:  gcc
 *
 *         Author:  (Watchwolf), Atton Jonathan <watchwolf@watchwolf.fr>
 *        Company:
 *
 * =====================================================================================
 */

#include "exalt_dbus_bootprocess.h"

#include "libexalt_dbus_private.h"

void exalt_dbus_bootprocess_iface_add(const exalt_dbus_conn* conn, const char* interface)
{
    DBusPendingCall * ret;
    DBusMessage *msg;
    DBusMessageIter args;

    if(!conn || !interface)
    {
        exalt_dbus_print_error("ERROR", __FILE__,__LINE__,__func__,"conn=%p, interface=%p",conn, interface);
        return ;
    }

    msg = exalt_dbus_write_call_new("BOOTPROCESS_IFACE_ADD");
    dbus_message_iter_init_append(msg, &args);
    if (!dbus_message_iter_append_basic(&args, DBUS_TYPE_STRING, &interface))
    {
        exalt_dbus_print_error("ERROR", __FILE__,__LINE__,__func__,"Out Of Memory!");
        dbus_message_unref(msg);
        return ;
    }

    if(!dbus_connection_send_with_reply (conn->conn, msg, &ret, -1))
    {
        exalt_dbus_print_error("ERROR", __FILE__,__LINE__,__func__,"Send Out Of Memory!");
        dbus_message_unref(msg);
        return ;
    }
    dbus_message_unref(msg);

    dbus_pending_call_block(ret);
    msg = dbus_pending_call_steal_reply(ret);
    if(msg == NULL)
    {
        exalt_dbus_print_error("ERROR", __FILE__,__LINE__,__func__,"msg=",msg);
        return ;
    }
    dbus_pending_call_unref(ret);

    dbus_message_unref(msg);
}

void exalt_dbus_bootprocess_iface_remove(const exalt_dbus_conn* conn, const char* interface)
{
    DBusPendingCall * ret;
    DBusMessage *msg;
    DBusMessageIter args;

    if(!conn || !interface)
    {
        exalt_dbus_print_error("ERROR", __FILE__,__LINE__,__func__,"conn=%p, interface=%p",conn, interface);
        return ;
    }

    msg = exalt_dbus_write_call_new("BOOTPROCESS_IFACE_REMOVE");
    dbus_message_iter_init_append(msg, &args);
    if (!dbus_message_iter_append_basic(&args, DBUS_TYPE_STRING, &interface))
    {
        exalt_dbus_print_error("ERROR", __FILE__,__LINE__,__func__,"Out Of Memory!");
        dbus_message_unref(msg);
        return ;
    }

    if(!dbus_connection_send_with_reply (conn->conn, msg, &ret, -1))
    {
        exalt_dbus_print_error("ERROR", __FILE__,__LINE__,__func__,"Send Out Of Memory!");
        dbus_message_unref(msg);
        return ;
    }
    dbus_message_unref(msg);

    dbus_pending_call_block(ret);
    msg = dbus_pending_call_steal_reply(ret);
    if(msg == NULL)
    {
        exalt_dbus_print_error("ERROR", __FILE__,__LINE__,__func__,"msg=",msg);
        return ;
    }
    dbus_pending_call_unref(ret);

    dbus_message_unref(msg);
}

int exalt_dbus_bootprocess_iface_is(const exalt_dbus_conn* conn, const char* interface)
{
    DBusPendingCall * ret;
    DBusMessage *msg;
    DBusMessageIter args;
    int res;

    if(!conn || !interface)
    {
        exalt_dbus_print_error("ERROR", __FILE__,__LINE__,__func__,"conn=%p, interface=%p",conn, interface);
        return 0;
    }

    msg = exalt_dbus_read_call_new("BOOTPROCESS_IFACE_IS");
    dbus_message_iter_init_append(msg, &args);
    if (!dbus_message_iter_append_basic(&args, DBUS_TYPE_STRING, &interface))
    {
        exalt_dbus_print_error("ERROR", __FILE__,__LINE__,__func__,"Out Of Memory!");
        dbus_message_unref(msg);
        return 0;
    }

    if(!dbus_connection_send_with_reply (conn->conn, msg, &ret, -1))
    {
        exalt_dbus_print_error("ERROR", __FILE__,__LINE__,__func__,"Send Out Of Memory!");
        dbus_message_unref(msg);
        return 0;
    }
    dbus_message_unref(msg);

    dbus_pending_call_block(ret);
    msg = dbus_pending_call_steal_reply(ret);
    if(msg == NULL)
    {
        exalt_dbus_print_error("ERROR", __FILE__,__LINE__,__func__,"msg=",msg);
        return 0;
    }
    dbus_pending_call_unref(ret);
    res=exalt_dbus_response_boolean(msg);
    dbus_message_unref(msg);

    return res;
}

