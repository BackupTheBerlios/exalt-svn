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
#include "libexalt_dbus_private.h"

Ecore_List* exalt_dbus_wireless_scan_wait(const exalt_dbus_conn* conn, const char* eth)
{
    DBusPendingCall * ret;
    DBusMessage *msg;
    DBusMessageIter args;
    Ecore_List* res;

    if(!conn || !eth)
    {
        exalt_dbus_print_error("ERROR", __FILE__,__LINE__,__func__,"conn=%p, eth=%p \n",conn, eth);
        return 0;
    }

    msg = exalt_dbus_read_call_new("IFACE_SCAN_WAIT");
    dbus_message_iter_init_append(msg, &args);
    if (!dbus_message_iter_append_basic(&args, DBUS_TYPE_STRING, &eth)) {
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

    //read the response
    res = exalt_dbus_response_strings(msg);
    dbus_message_unref(msg);
    return res;
}

void exalt_dbus_wireless_scan_start(const exalt_dbus_conn* conn, const char* eth)
{
    DBusPendingCall * ret;
    DBusMessage *msg;
    DBusMessageIter args;

    if(!conn || !eth)
    {
        exalt_dbus_print_error("ERROR", __FILE__,__LINE__,__func__,"conn=%p, eth=%p \n",conn, eth);
        return ;
    }

    msg = exalt_dbus_read_call_new("IFACE_SCAN_START");
    dbus_message_iter_init_append(msg, &args);
    if (!dbus_message_iter_append_basic(&args, DBUS_TYPE_STRING, &eth)) {
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
}

void exalt_dbus_wireless_scan_stop(const exalt_dbus_conn* conn, const char* eth)
{
    DBusPendingCall * ret;
    DBusMessage *msg;
    DBusMessageIter args;

    if(!conn || !eth)
    {
        exalt_dbus_print_error("ERROR", __FILE__,__LINE__,__func__,"conn=%p, eth=%p \n",conn, eth);
        return ;
    }

    msg = exalt_dbus_read_call_new("IFACE_SCAN_STOP");
    dbus_message_iter_init_append(msg, &args);
    if (!dbus_message_iter_append_basic(&args, DBUS_TYPE_STRING, &eth)) {
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
}


char* exalt_dbus_wireless_get_essid(const exalt_dbus_conn* conn, const char* eth)
{
    DBusPendingCall * ret;
    DBusMessage *msg;
    DBusMessageIter args;
    char* res;

    if(!conn || !eth)
    {
        exalt_dbus_print_error("ERROR", __FILE__,__LINE__,__func__,"conn==%p, eth==%p ",conn, eth);
        return NULL;
    }

    msg = exalt_dbus_read_call_new("IFACE_GET_ESSID");
    dbus_message_iter_init_append(msg, &args);
    if (!dbus_message_iter_append_basic(&args, DBUS_TYPE_STRING, &eth)) {
        exalt_dbus_print_error("ERROR", __FILE__,__LINE__,__func__,"Out Of Memory!");
        dbus_message_unref(msg);
        return NULL;
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

    //read the response
    res = exalt_dbus_response_string(msg);
    dbus_message_unref(msg);
    return res;
}

void exalt_dbus_wireless_set_new_essid(const exalt_dbus_conn* conn, const char* eth, const char* essid)
{
    DBusPendingCall * ret;
    DBusMessage *msg;
    DBusMessageIter args;

    if(!conn || !eth || !essid)
    {
        exalt_dbus_print_error("ERROR", __FILE__,__LINE__,__func__,"conn=%p, eth=%p essid=%p",conn, eth, essid);
        return ;
    }

    msg = exalt_dbus_write_call_new("IFACE_SET_NEW_ESSID");
    dbus_message_iter_init_append(msg, &args);
    if (!dbus_message_iter_append_basic(&args, DBUS_TYPE_STRING, &eth))
    {
        exalt_dbus_print_error("ERROR", __FILE__,__LINE__,__func__,"Out Of Memory!");
        dbus_message_unref(msg);
        return ;
    }
    if (!dbus_message_iter_append_basic(&args, DBUS_TYPE_STRING, &essid))
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

void exalt_dbus_wireless_set_new_passwd(const exalt_dbus_conn* conn, const char* eth, const char* passwd)
{
    DBusPendingCall * ret;
    DBusMessage *msg;
    DBusMessageIter args;

    if(!conn || !eth || !passwd)
    {
        exalt_dbus_print_error("ERROR", __FILE__,__LINE__,__func__,"conn=%p, eth=%p passwd=%p",conn, eth, passwd);
        return ;
    }

    msg = exalt_dbus_write_call_new("IFACE_SET_NEW_PASSWD");
    dbus_message_iter_init_append(msg, &args);
    if (!dbus_message_iter_append_basic(&args, DBUS_TYPE_STRING, &eth))
    {
        exalt_dbus_print_error("ERROR", __FILE__,__LINE__,__func__,"Out Of Memory!");
        dbus_message_unref(msg);
        return ;
    }
    if (!dbus_message_iter_append_basic(&args, DBUS_TYPE_STRING, &passwd))
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

void exalt_dbus_wireless_set_new_passwd_mode(const exalt_dbus_conn* conn, const char* eth, int passwd_mode)
{
    DBusPendingCall * ret;
    DBusMessage *msg;
    DBusMessageIter args;

    if(!conn || !eth || !passwd_mode)
    {
        exalt_dbus_print_error("ERROR", __FILE__,__LINE__,__func__,"conn=%p, eth=%p passwd_mode=%p",conn, eth, passwd_mode);
        return ;
    }

    msg = exalt_dbus_write_call_new("IFACE_SET_NEW_PASSWD_MODE");
    dbus_message_iter_init_append(msg, &args);
    if (!dbus_message_iter_append_basic(&args, DBUS_TYPE_STRING, &eth))
    {
        exalt_dbus_print_error("ERROR", __FILE__,__LINE__,__func__,"Out Of Memory!");
        dbus_message_unref(msg);
        return ;
    }
    if (!dbus_message_iter_append_basic(&args, DBUS_TYPE_INT32, &passwd_mode))
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

void exalt_dbus_wireless_set_new_mode(const exalt_dbus_conn* conn, const char* eth, int mode)
{
    DBusPendingCall * ret;
    DBusMessage *msg;
    DBusMessageIter args;

    if(!conn || !eth || !mode)
    {
        exalt_dbus_print_error("ERROR", __FILE__,__LINE__,__func__,"conn=%p, eth=%p mode=%p",conn, eth, mode);
        return ;
    }

    msg = exalt_dbus_write_call_new("IFACE_SET_NEW_MODE");
    dbus_message_iter_init_append(msg, &args);
    if (!dbus_message_iter_append_basic(&args, DBUS_TYPE_STRING, &eth))
    {
        exalt_dbus_print_error("ERROR", __FILE__,__LINE__,__func__,"Out Of Memory!");
        dbus_message_unref(msg);
        return ;
    }
    if (!dbus_message_iter_append_basic(&args, DBUS_TYPE_INT32, &mode))
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

void exalt_dbus_wireless_set_new_security_mode(const exalt_dbus_conn* conn, const char* eth, int security_mode)
{
    DBusPendingCall * ret;
    DBusMessage *msg;
    DBusMessageIter args;

    if(!conn || !eth || !security_mode)
    {
        exalt_dbus_print_error("ERROR", __FILE__,__LINE__,__func__,"conn=%p, eth=%p security_mode=%p",conn, eth, security_mode);
        return ;
    }

    msg = exalt_dbus_write_call_new("IFACE_SET_NEW_SECURITY_MODE");
    dbus_message_iter_init_append(msg, &args);
    if (!dbus_message_iter_append_basic(&args, DBUS_TYPE_STRING, &eth))
    {
        exalt_dbus_print_error("ERROR", __FILE__,__LINE__,__func__,"Out Of Memory!");
        dbus_message_unref(msg);
        return ;
    }
    if (!dbus_message_iter_append_basic(&args, DBUS_TYPE_INT32, &security_mode))
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
