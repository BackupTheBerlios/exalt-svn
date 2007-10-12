/*
 * =====================================================================================
 *
 *       Filename:  exalt_dbus_wireless_info.c
 *
 *    Description:  All functions about a wireless network
 *
 *        Version:  1.0
 *        Created:  08/31/2007 08:58:49 PM CEST
 *       Revision:  none
 *       Compiler:  gcc
 *
 *         Author:   (Watchwolf), Atton Jonathan <watchwolf@watchwolf.fr
 *        Company:
 *
 * =====================================================================================
 */

#include "exalt_dbus_wireless_info.h"
#include "libexalt_dbus_private.h"

int exalt_dbus_wirelessinfo_get_quality(const exalt_dbus_conn* conn, const char* eth, const char* essid)
{
    DBusPendingCall * ret;
    DBusMessage *msg;
    DBusMessageIter args;
    int res;

    if(!conn || !eth || !essid)
    {

        exalt_dbus_print_error("ERROR", __FILE__,__LINE__,__func__,"conn==%p, eth==%p essid=%p",conn, eth, essid);
        return -1;
    }

    msg = exalt_dbus_read_call_new("NETWORK_GET_QUALITY");
    dbus_message_iter_init_append(msg, &args);
    if (!dbus_message_iter_append_basic(&args, DBUS_TYPE_STRING, &eth)) {
        exalt_dbus_print_error("ERROR", __FILE__,__LINE__,__func__,"Out Of Memory!");
        dbus_message_unref(msg);
        return -1;
    }

    if (!dbus_message_iter_append_basic(&args, DBUS_TYPE_STRING, &essid)) {
        exalt_dbus_print_error("ERROR", __FILE__,__LINE__,__func__,"Out Of Memory!");
        dbus_message_unref(msg);
        return -1;
    }

    dbus_connection_send_with_reply (conn->conn, msg, &ret, -1);
    dbus_pending_call_block(ret);
    msg = dbus_pending_call_steal_reply(ret);
    dbus_pending_call_unref(ret);

    //read the response
    res = exalt_dbus_response_integer(msg);
    dbus_message_unref(msg);
    return res;
}

int exalt_dbus_wirelessinfo_get_encryption(const exalt_dbus_conn* conn, const char* eth, const char* essid)
{
    DBusPendingCall * ret;
    DBusMessage *msg;
    DBusMessageIter args;
    int res;

    if(!conn || !eth || !essid)
    {
        exalt_dbus_print_error("ERROR", __FILE__,__LINE__,__func__,"conn==%p, eth==%p essid=%p",conn, eth, essid);
        return -1;
    }

    msg = exalt_dbus_read_call_new("NETWORK_GET_ENCRYPTION");
    dbus_message_iter_init_append(msg, &args);
    if (!dbus_message_iter_append_basic(&args, DBUS_TYPE_STRING, &eth)) {
        exalt_dbus_print_error("ERROR", __FILE__,__LINE__,__func__,"Out Of Memory!");
        dbus_message_unref(msg);
        return -1;
    }

    if (!dbus_message_iter_append_basic(&args, DBUS_TYPE_STRING, &essid)) {
        exalt_dbus_print_error("ERROR", __FILE__,__LINE__,__func__,"Out Of Memory!");
        dbus_message_unref(msg);
        return -1;
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
    res = exalt_dbus_response_integer(msg);
    dbus_message_unref(msg);
    return res;
}

int exalt_dbus_wirelessinfo_get_signallvl(const exalt_dbus_conn* conn, const char* eth, const char* essid)
{
    DBusPendingCall * ret;
    DBusMessage *msg;
    DBusMessageIter args;
    int res;

    if(!conn || !eth || !essid)
    {
        exalt_dbus_print_error("ERROR", __FILE__,__LINE__,__func__,"conn==%p, eth==%p essid=%p",conn, eth, essid);
        return -1;
    }

    msg = exalt_dbus_read_call_new("NETWORK_GET_SIGNALLVL");
    dbus_message_iter_init_append(msg, &args);
    if (!dbus_message_iter_append_basic(&args, DBUS_TYPE_STRING, &eth)) {
        exalt_dbus_print_error("ERROR", __FILE__,__LINE__,__func__,"Out Of Memory!");
        dbus_message_unref(msg);
        return -1;
    }

    if (!dbus_message_iter_append_basic(&args, DBUS_TYPE_STRING, &essid)) {
        exalt_dbus_print_error("ERROR", __FILE__,__LINE__,__func__,"Out Of Memory!");
        dbus_message_unref(msg);
        return -1;
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
    res = exalt_dbus_response_integer(msg);
    dbus_message_unref(msg);
    return res;
}

int exalt_dbus_wirelessinfo_get_noiselvl(const exalt_dbus_conn* conn, const char* eth, const char* essid)
{
    DBusPendingCall * ret;
    DBusMessage *msg;
    DBusMessageIter args;
    int res;

    if(!conn || !eth || !essid)
    {
        exalt_dbus_print_error("ERROR", __FILE__,__LINE__,__func__,"conn==%p, eth==%p essid=%p",conn, eth, essid);
        return -1;
    }

    msg = exalt_dbus_read_call_new("NETWORK_GET_NOISELVL");
    dbus_message_iter_init_append(msg, &args);
    if (!dbus_message_iter_append_basic(&args, DBUS_TYPE_STRING, &eth)) {
        exalt_dbus_print_error("ERROR", __FILE__,__LINE__,__func__,"Out Of Memory!");
        dbus_message_unref(msg);
        return -1;
    }

    if (!dbus_message_iter_append_basic(&args, DBUS_TYPE_STRING, &essid)) {
        exalt_dbus_print_error("ERROR", __FILE__,__LINE__,__func__,"Out Of Memory!");
        dbus_message_unref(msg);
        return -1;
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
    res = exalt_dbus_response_integer(msg);
    dbus_message_unref(msg);
    return res;
}

char* exalt_dbus_wirelessinfo_get_addr(const exalt_dbus_conn* conn, const char* eth, const char* essid)
{
    DBusPendingCall * ret;
    DBusMessage *msg;
    DBusMessageIter args;
    char* res;

    if(!conn || !eth || !essid)
    {
        exalt_dbus_print_error("ERROR", __FILE__,__LINE__,__func__,"conn==%p, eth==%p essid=%p",conn, eth, essid);
        return NULL;
    }

    msg = exalt_dbus_read_call_new("NETWORK_GET_ADDR");
    dbus_message_iter_init_append(msg, &args);
    if (!dbus_message_iter_append_basic(&args, DBUS_TYPE_STRING, &eth)) {
        exalt_dbus_print_error("ERROR", __FILE__,__LINE__,__func__,"Out Of Memory!");
        dbus_message_unref(msg);
        return NULL;
    }

    if (!dbus_message_iter_append_basic(&args, DBUS_TYPE_STRING, &essid)) {
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

char* exalt_dbus_wirelessinfo_get_protocol(const exalt_dbus_conn* conn, const char* eth, const char* essid)
{
    DBusPendingCall * ret;
    DBusMessage *msg;
    DBusMessageIter args;
    char* res;

    if(!conn || !eth || !essid)
    {
        exalt_dbus_print_error("ERROR", __FILE__,__LINE__,__func__,"conn==%p, eth==%p essid=%p",conn, eth, essid);
        return NULL;
    }

    msg = exalt_dbus_read_call_new("NETWORK_GET_PROTOCOL");
    dbus_message_iter_init_append(msg, &args);
    if (!dbus_message_iter_append_basic(&args, DBUS_TYPE_STRING, &eth)) {
        exalt_dbus_print_error("ERROR", __FILE__,__LINE__,__func__,"Out Of Memory!");
        dbus_message_unref(msg);
        return NULL;
    }

    if (!dbus_message_iter_append_basic(&args, DBUS_TYPE_STRING, &essid)) {
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

char* exalt_dbus_wirelessinfo_get_mode(const exalt_dbus_conn* conn, const char* eth, const char* essid)
{
    DBusPendingCall * ret;
    DBusMessage *msg;
    DBusMessageIter args;
    char* res;

    if(!conn || !eth || !essid)
    {
        exalt_dbus_print_error("ERROR", __FILE__,__LINE__,__func__,"conn==%p, eth==%p essid=%p",conn, eth, essid);
        return NULL;
    }

    msg = exalt_dbus_read_call_new("NETWORK_GET_MODE");
    dbus_message_iter_init_append(msg, &args);
    if (!dbus_message_iter_append_basic(&args, DBUS_TYPE_STRING, &eth)) {
        exalt_dbus_print_error("ERROR", __FILE__,__LINE__,__func__,"Out Of Memory!");
        dbus_message_unref(msg);
        return NULL;
    }

    if (!dbus_message_iter_append_basic(&args, DBUS_TYPE_STRING, &essid)) {
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

char* exalt_dbus_wirelessinfo_get_channel(const exalt_dbus_conn* conn, const char* eth, const char* essid)
{
    DBusPendingCall * ret;
    DBusMessage *msg;
    DBusMessageIter args;
    char* res;

    if(!conn || !eth || !essid)
    {
        exalt_dbus_print_error("ERROR", __FILE__,__LINE__,__func__,"conn==%p, eth==%p essid=%p",conn, eth, essid);
        return NULL;
    }

    msg = exalt_dbus_read_call_new("NETWORK_GET_CHANNEL");
    dbus_message_iter_init_append(msg, &args);
    if (!dbus_message_iter_append_basic(&args, DBUS_TYPE_STRING, &eth)) {
        exalt_dbus_print_error("ERROR", __FILE__,__LINE__,__func__,"Out Of Memory!");
        dbus_message_unref(msg);
        return NULL;
    }

    if (!dbus_message_iter_append_basic(&args, DBUS_TYPE_STRING, &essid)) {
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

char* exalt_dbus_wirelessinfo_get_bitrates(const exalt_dbus_conn* conn, const char* eth, const char* essid)
{
    DBusPendingCall * ret;
    DBusMessage *msg;
    DBusMessageIter args;
    char* res;

    if(!conn || !eth || !essid)
    {
        exalt_dbus_print_error("ERROR", __FILE__,__LINE__,__func__,"conn==%p, eth==%p essid=%p",conn, eth, essid);
        return NULL;
    }

    msg = exalt_dbus_read_call_new("NETWORK_GET_BITRATES");
    dbus_message_iter_init_append(msg, &args);
    if (!dbus_message_iter_append_basic(&args, DBUS_TYPE_STRING, &eth)) {
        exalt_dbus_print_error("ERROR", __FILE__,__LINE__,__func__,"Out Of Memory!");
        dbus_message_unref(msg);
        return NULL;
    }

    if (!dbus_message_iter_append_basic(&args, DBUS_TYPE_STRING, &essid)) {
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


Exalt_Connection * exalt_dbus_wirelessinfo_get_default_conn(const exalt_dbus_conn* conn, const char* eth, const char* essid)
{
    DBusPendingCall * ret;
    DBusMessage *msg;
    DBusMessageIter args;
    const char* s;
    int i;
    Exalt_Connection *c;

    if(!conn || !eth || !essid)
    {
        exalt_dbus_print_error("ERROR", __FILE__,__LINE__,__func__,"conn==%p, eth==%p essid=%p",conn, eth, essid);
        return NULL;
    }

    msg = exalt_dbus_read_call_new("NETWORK_GET_DEFAULT_CONN");
    dbus_message_iter_init_append(msg, &args);
    if (!dbus_message_iter_append_basic(&args, DBUS_TYPE_STRING, &eth)) {
        exalt_dbus_print_error("ERROR", __FILE__,__LINE__,__func__,"Out Of Memory!");
        dbus_message_unref(msg);
        return NULL;
    }

    if (!dbus_message_iter_append_basic(&args, DBUS_TYPE_STRING, &essid)) {
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
    c = exalt_conn_new();
    if(!dbus_message_iter_init(msg, &args))
    {
        exalt_dbus_print_error("ERROR", __FILE__, __LINE__,__func__, "no argument");
        return NULL;
    }
    if (DBUS_TYPE_INT32 != dbus_message_iter_get_arg_type(&args))
    {
        exalt_dbus_print_error("ERROR", __FILE__, __LINE__,__func__, "Argument is not a int");
        return NULL;
    }
    else
        dbus_message_iter_get_basic(&args, &i);
    exalt_conn_set_mode(c,i);
    dbus_message_iter_next(&args);

    if(!exalt_conn_is_dhcp(c))
    {
        if (DBUS_TYPE_STRING != dbus_message_iter_get_arg_type(&args))
        {
            exalt_dbus_print_error("ERROR", __FILE__, __LINE__,__func__, "Argument is not a string");
            return NULL;
        }
        else
            dbus_message_iter_get_basic(&args, &s);
        exalt_conn_set_ip(c,s);
        dbus_message_iter_next(&args);

        if (DBUS_TYPE_STRING != dbus_message_iter_get_arg_type(&args))
        {
            exalt_dbus_print_error("ERROR", __FILE__, __LINE__,__func__, "Argument is not a string");
            return NULL;
        }
        else
            dbus_message_iter_get_basic(&args, &s);
        exalt_conn_set_netmask(c,s);
        dbus_message_iter_next(&args);

        if (DBUS_TYPE_STRING != dbus_message_iter_get_arg_type(&args))
        {
            exalt_dbus_print_error("ERROR", __FILE__, __LINE__,__func__, "Argument is not a string");
            return NULL;
        }
        else
            dbus_message_iter_get_basic(&args, &s);
        exalt_conn_set_gateway(c,s);
        dbus_message_iter_next(&args);
    }

    if (DBUS_TYPE_INT32 != dbus_message_iter_get_arg_type(&args))
    {
        exalt_dbus_print_error("ERROR", __FILE__, __LINE__,__func__, "Argument is not a int");
        return NULL;
    }
    else
        dbus_message_iter_get_basic(&args, &i);
    exalt_conn_set_wireless(c,i);
    dbus_message_iter_next(&args);

    if(exalt_conn_is_wireless(c))
    {
        if (DBUS_TYPE_INT32 != dbus_message_iter_get_arg_type(&args))
        {
            exalt_dbus_print_error("ERROR", __FILE__, __LINE__,__func__, "Argument is not a int");
            return NULL;
        }
        else
            dbus_message_iter_get_basic(&args, &i);
        exalt_conn_set_encryption_mode(c,i);
        dbus_message_iter_next(&args);

        if(exalt_conn_get_encryption_mode(c)!= EXALT_ENCRYPTION_NONE)
        {
            if (DBUS_TYPE_STRING != dbus_message_iter_get_arg_type(&args))
            {
                exalt_dbus_print_error("ERROR", __FILE__, __LINE__,__func__, "Argument is not a string");
                return NULL;
            }
            else
                dbus_message_iter_get_basic(&args, &s);
            exalt_conn_set_key(c,s);
            dbus_message_iter_next(&args);
        }

        if (DBUS_TYPE_INT32 != dbus_message_iter_get_arg_type(&args))
        {
            exalt_dbus_print_error("ERROR", __FILE__, __LINE__,__func__, "Argument is not a int");
            return NULL;
        }
        else
            dbus_message_iter_get_basic(&args, &i);
        exalt_conn_set_connection_mode(c,i);
        dbus_message_iter_next(&args);


        if (DBUS_TYPE_INT32 != dbus_message_iter_get_arg_type(&args))
        {
            exalt_dbus_print_error("ERROR", __FILE__, __LINE__,__func__, "Argument is not a int");
            return NULL;
        }
        else
            dbus_message_iter_get_basic(&args, &i);
        exalt_conn_set_security_mode(c,i);
        dbus_message_iter_next(&args);
    }
    dbus_message_unref(msg);
    return c;
}

