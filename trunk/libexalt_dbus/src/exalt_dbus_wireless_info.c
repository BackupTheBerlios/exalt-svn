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




char* exalt_dbus_wirelessinfo_get_default_passwd(const exalt_dbus_conn* conn, const char* eth, const char* essid)
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

    msg = exalt_dbus_read_call_new("NETWORK_GET_DEFAULT_PASSWD");
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

char* exalt_dbus_wirelessinfo_get_default_ip(const exalt_dbus_conn* conn, const char* eth, const char* essid)
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

    msg = exalt_dbus_read_call_new("NETWORK_GET_DEFAULT_IP");
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

char* exalt_dbus_wirelessinfo_get_default_netmask(const exalt_dbus_conn* conn, const char* eth, const char* essid)
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

    msg = exalt_dbus_read_call_new("NETWORK_GET_DEFAULT_NETMASK");
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

char* exalt_dbus_wirelessinfo_get_default_gateway(const exalt_dbus_conn* conn, const char* eth, const char* essid)
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

    msg = exalt_dbus_read_call_new("NETWORK_GET_DEFAULT_GATEWAY");
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

int exalt_dbus_wirelessinfo_get_default_passwd_mode(const exalt_dbus_conn* conn, const char* eth, const char* essid)
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

    msg = exalt_dbus_read_call_new("NETWORK_GET_DEFAULT_PASSWD_MODE");
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

int exalt_dbus_wirelessinfo_get_default_security_mode(const exalt_dbus_conn* conn, const char* eth, const char* essid)
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

    msg = exalt_dbus_read_call_new("NETWORK_GET_DEFAULT_SECURITY_MODE");
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

int exalt_dbus_wirelessinfo_get_default_mode(const exalt_dbus_conn* conn, const char* eth, const char* essid)
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

    msg = exalt_dbus_read_call_new("NETWORK_GET_DEFAULT_MODE");
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

int exalt_dbus_wirelessinfo_is_default_dhcp(const exalt_dbus_conn* conn, const char* eth, const char* essid)
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

    msg = exalt_dbus_read_call_new("NETWORK_IS_DEFAULT_DHCP");
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
    res = exalt_dbus_response_boolean(msg);
    dbus_message_unref(msg);
    return res;
}


