/*
 * =====================================================================================
 *
 *       Filename:  exalt_dbus_dns.c
 *
 *    Description:  manage the dns list
 *
 *        Version:  1.0
 *        Created:  09/21/2007 09:03:33 PM CEST
 *       Revision:  none
 *       Compiler:  gcc
 *
 *         Author:   (Watchwolf), Atton Jonathan <watchwolf@watchwolf.fr>
 *        Company:
 *
 * =====================================================================================
 */

#include "exalt_dbus_dns.h"
#include "libexalt_dbus_private.h"

Ecore_List* exalt_dbus_dns_get_list(const exalt_dbus_conn* conn)
{
    DBusPendingCall * ret;
    DBusMessage *msg;
    Ecore_List* res;

    if(!conn)
    {
        exalt_dbus_print_error("ERROR", __FILE__,__LINE__,__func__,"conn==%p ",conn);
        return NULL;
    }

    msg = exalt_dbus_read_call_new("DNS_GET_LIST");
    if(!dbus_connection_send_with_reply (conn->conn, msg, &ret, -1))
    {
        exalt_dbus_print_error("ERROR", __FILE__,__LINE__,__func__,"Send Out Of Memory!");
        dbus_message_unref(msg);
        return NULL;
    }
    dbus_message_unref(msg);

    dbus_pending_call_block(ret);
    msg = dbus_pending_call_steal_reply(ret);
    if(msg == NULL)
    {
        exalt_dbus_print_error("ERROR", __FILE__,__LINE__,__func__,"msg=",msg);
        return NULL;
    }
    dbus_pending_call_unref(ret);

    //read the response
    res = exalt_dbus_response_strings(msg);
    dbus_message_unref(msg);
    return res;
}

void exalt_dbus_dns_add(const exalt_dbus_conn* conn, const char* dns)
{
    DBusPendingCall * ret;
    DBusMessage *msg;
    DBusMessageIter args;

    if(!conn || !dns)
    {
        exalt_dbus_print_error("ERROR", __FILE__,__LINE__,__func__,"conn=%p, dns=%p",conn, dns);
        return ;
    }

    msg = exalt_dbus_write_call_new("DNS_ADD");
    dbus_message_iter_init_append(msg, &args);
    if (!dbus_message_iter_append_basic(&args, DBUS_TYPE_STRING, &dns))
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

void exalt_dbus_dns_delete(const exalt_dbus_conn* conn, const char* dns)
{
    DBusPendingCall * ret;
    DBusMessage *msg;
    DBusMessageIter args;

    if(!conn || !dns)
    {
        exalt_dbus_print_error("ERROR", __FILE__,__LINE__,__func__,"conn=%p, dns=%p",conn, dns);
        return ;
    }

    msg = exalt_dbus_write_call_new("DNS_DELETE");
    dbus_message_iter_init_append(msg, &args);
    if (!dbus_message_iter_append_basic(&args, DBUS_TYPE_STRING, &dns))
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

void exalt_dbus_dns_replace(const exalt_dbus_conn* conn, const char* old_dns, const char* new_dns)
{
    DBusPendingCall * ret;
    DBusMessage *msg;
    DBusMessageIter args;

    if(!conn || !old_dns || !new_dns)
    {
        exalt_dbus_print_error("ERROR", __FILE__,__LINE__,__func__,"conn=%p, old_dns=%p, new_dns=%p",conn, old_dns, new_dns);
        return ;
    }

    msg = exalt_dbus_write_call_new("DNS_REPLACE");
    dbus_message_iter_init_append(msg, &args);
    if (!dbus_message_iter_append_basic(&args, DBUS_TYPE_STRING, &old_dns))
    {
        exalt_dbus_print_error("ERROR", __FILE__,__LINE__,__func__,"Out Of Memory!");
        dbus_message_unref(msg);
        return ;
    }

    if (!dbus_message_iter_append_basic(&args, DBUS_TYPE_STRING, &new_dns))
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
