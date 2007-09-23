/*
 * =====================================================================================
 *
 *       Filename:  exalt_dbus_ethernet.c
 *
 *    Description:  all functions about a ethernet interface (no wireless)
 *
 *        Version:  1.0
 *        Created:  08/29/2007 02:19:12 PM CEST
 *       Revision:  none
 *       Compiler:  gcc
 *
 *         Author:   (Watchwolf), Atton Jonathan <watchwolf@watchwolf.fr>
 *        Company:
 *
 * =====================================================================================
 */

#include "exalt_dbus_ethernet.h"
#include "libexalt_dbus_private.h"

char* exalt_dbus_eth_get_ip(const exalt_dbus_conn* conn, const char* eth)
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

    msg = exalt_dbus_read_call_new("IFACE_GET_IP");
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
    res = exalt_dbus_response_string(msg);
    dbus_message_unref(msg);
    return res;
}

char* exalt_dbus_eth_get_netmask(const exalt_dbus_conn* conn, const char* eth)
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

    msg = exalt_dbus_read_call_new("IFACE_GET_NETMASK");
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
    res = exalt_dbus_response_string(msg);
    dbus_message_unref(msg);
    return res;
}

char* exalt_dbus_eth_get_gateway(const exalt_dbus_conn* conn, const char* eth)
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

    msg = exalt_dbus_read_call_new("IFACE_GET_GATEWAY");
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
    res = exalt_dbus_response_string(msg);
    dbus_message_unref(msg);
    return res;
}

Ecore_List* exalt_dbus_eth_get_list(const exalt_dbus_conn* conn)
{
    DBusPendingCall * ret;
    DBusMessage *msg;
    Ecore_List* res;

    if(!conn)
    {
        exalt_dbus_print_error("ERROR", __FILE__,__LINE__,__func__,"conn==%p ",conn);
        return NULL;
    }

    msg = exalt_dbus_read_call_new("IFACE_GET_ETH_LIST");
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
    }dbus_pending_call_unref(ret);

    //read the response
    res = exalt_dbus_response_strings(msg);
    dbus_message_unref(msg);
    return res;
}

int exalt_dbus_eth_is_wireless(const exalt_dbus_conn* conn, const char* eth)
{
    DBusPendingCall * ret;
    DBusMessage *msg;
    DBusMessageIter args;
    int res;

    if(!conn || !eth)
    {
        exalt_dbus_print_error("ERROR", __FILE__,__LINE__,__func__,"conn==%p, eth==%p ",conn, eth);
        return 0;
    }

    msg = exalt_dbus_read_call_new("IFACE_IS_WIRELESS");
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
    res = exalt_dbus_response_boolean(msg);
    dbus_message_unref(msg);
    return res;
}

int exalt_dbus_eth_is_link(const exalt_dbus_conn* conn, const char* eth)
{
    DBusPendingCall * ret;
    DBusMessage *msg;
    DBusMessageIter args;
    int res;

    if(!conn || !eth)
    {
        exalt_dbus_print_error("ERROR", __FILE__,__LINE__,__func__,"conn==%p, eth==%p ",conn, eth);
        return 0;
    }

    msg = exalt_dbus_read_call_new("IFACE_IS_LINK");
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
    res = exalt_dbus_response_boolean(msg);
    dbus_message_unref(msg);
    return res;
}

int exalt_dbus_eth_is_up(const exalt_dbus_conn* conn, const char* eth)
{
    DBusPendingCall * ret;
    DBusMessage *msg;
    DBusMessageIter args;
    int res;

    if(!conn || !eth)
    {
        exalt_dbus_print_error("ERROR", __FILE__,__LINE__,__func__,"conn==%p, eth==%p ",conn, eth);
        return 0;
    }

    msg = exalt_dbus_read_call_new("IFACE_IS_UP");
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
    res = exalt_dbus_response_boolean(msg);
    dbus_message_unref(msg);
    return res;
}


int exalt_dbus_eth_is_dhcp(const exalt_dbus_conn* conn, const char* eth)
{
    DBusPendingCall * ret;
    DBusMessage *msg;
    DBusMessageIter args;
    int res;

    if(!conn || !eth)
    {
        exalt_dbus_print_error("ERROR", __FILE__,__LINE__,__func__,"conn==%p, eth==%p ",conn, eth);
        return 0;
    }

    msg = exalt_dbus_read_call_new("IFACE_IS_DHCP");
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
    res = exalt_dbus_response_boolean(msg);
    dbus_message_unref(msg);
    return res;
}


void exalt_dbus_eth_up(const exalt_dbus_conn* conn, const char* eth)
{
    DBusPendingCall * ret;
    DBusMessage *msg;
    DBusMessageIter args;

    if(!conn || !eth)
    {
        exalt_dbus_print_error("ERROR", __FILE__,__LINE__,__func__,"conn==%p, eth==%p ",conn, eth);
        return ;
    }

    msg = exalt_dbus_write_call_new("IFACE_UP");
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

    dbus_message_unref(msg);
}

void exalt_dbus_eth_down(const exalt_dbus_conn* conn, const char* eth)
{
    DBusPendingCall * ret;
    DBusMessage *msg;
    DBusMessageIter args;

    if(!conn || !eth)
    {
        exalt_dbus_print_error("ERROR", __FILE__,__LINE__,__func__,"conn==%p, eth==%p ",conn, eth);
        return ;
    }

    msg = exalt_dbus_write_call_new("IFACE_DOWN");
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

    dbus_message_unref(msg);
}


void exalt_dbus_eth_set_new_ip(const exalt_dbus_conn* conn, const char* eth, const char* ip)
{
    DBusPendingCall * ret;
    DBusMessage *msg;
    DBusMessageIter args;

    if(!conn || !eth || !ip)
    {
        exalt_dbus_print_error("ERROR", __FILE__,__LINE__,__func__,"conn=%p, eth=%p ip=%p",conn, eth, ip);
        return ;
    }

    msg = exalt_dbus_write_call_new("IFACE_SET_NEW_IP");
    dbus_message_iter_init_append(msg, &args);
    if (!dbus_message_iter_append_basic(&args, DBUS_TYPE_STRING, &eth))
    {
        exalt_dbus_print_error("ERROR", __FILE__,__LINE__,__func__,"Out Of Memory!");
        dbus_message_unref(msg);
        return ;
    }
    if (!dbus_message_iter_append_basic(&args, DBUS_TYPE_STRING, &ip))
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

void exalt_dbus_eth_set_new_netmask(const exalt_dbus_conn* conn, const char* eth, const char* netmask)
{
    DBusPendingCall * ret;
    DBusMessage *msg;
    DBusMessageIter args;

    if(!conn || !eth || !netmask)
    {
        exalt_dbus_print_error("ERROR", __FILE__,__LINE__,__func__,"conn=%p, eth=%p netmask=%p",conn, eth, netmask);
        return ;
    }

    msg = exalt_dbus_write_call_new("IFACE_SET_NEW_NETMASK");
    dbus_message_iter_init_append(msg, &args);
    if (!dbus_message_iter_append_basic(&args, DBUS_TYPE_STRING, &eth))
    {
        exalt_dbus_print_error("ERROR", __FILE__,__LINE__,__func__,"Out Of Memory!");
        dbus_message_unref(msg);
        return ;
    }
    if (!dbus_message_iter_append_basic(&args, DBUS_TYPE_STRING, &netmask))
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

void exalt_dbus_eth_set_new_gateway(const exalt_dbus_conn* conn, const char* eth, const char* gateway)
{
    DBusPendingCall * ret;
    DBusMessage *msg;
    DBusMessageIter args;

    if(!conn || !eth || !gateway)
    {
        exalt_dbus_print_error("ERROR", __FILE__,__LINE__,__func__,"conn=%p, eth=%p gateway=%p",conn, eth, gateway);
        return ;
    }

    msg = exalt_dbus_write_call_new("IFACE_SET_NEW_GATEWAY");
    dbus_message_iter_init_append(msg, &args);
    if (!dbus_message_iter_append_basic(&args, DBUS_TYPE_STRING, &eth))
    {
        exalt_dbus_print_error("ERROR", __FILE__,__LINE__,__func__,"Out Of Memory!");
        dbus_message_unref(msg);
        return ;
    }
    if (!dbus_message_iter_append_basic(&args, DBUS_TYPE_STRING, &gateway))
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

void exalt_dbus_eth_set_new_dhcp(const exalt_dbus_conn* conn, const char* eth, short dhcp)
{
    DBusPendingCall * ret;
    DBusMessage *msg;
    DBusMessageIter args;
    int dhcp_int = dhcp;

    if(!conn || !eth)
    {
        exalt_dbus_print_error("ERROR", __FILE__,__LINE__,__func__,"conn=%p, eth=%p",conn, eth);
        return ;
    }

    msg = exalt_dbus_write_call_new("IFACE_SET_NEW_DHCP");
    dbus_message_iter_init_append(msg, &args);
    if (!dbus_message_iter_append_basic(&args, DBUS_TYPE_STRING, &eth))
    {
        exalt_dbus_print_error("ERROR", __FILE__,__LINE__,__func__,"Out Of Memory!");
        dbus_message_unref(msg);
        return ;
    }
    if (!dbus_message_iter_append_basic(&args, DBUS_TYPE_BOOLEAN, &dhcp_int))
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



void exalt_dbus_eth_apply_conf(exalt_dbus_conn* conn, const char* eth, exalt_notify_conf_applied_cb cb, void* user_data)
{
    DBusPendingCall * ret;
    DBusMessage *msg;
    DBusMessageIter args;

    if(!conn || !eth)
    {
        exalt_dbus_print_error("ERROR", __FILE__,__LINE__,__func__,"conn=%p, eth=%p",conn, eth);
        return ;
    }

    //first, we starting to wait the response
    if(!conn->notify_conf_applied)
    {
        conn -> notify_conf_applied = malloc(sizeof(exalt_dbus_notify_conf_applied_data));
        conn -> notify_conf_applied -> cb = cb;
        conn -> notify_conf_applied -> user_data = user_data;

        e_dbus_signal_handler_add(conn->e_conn, EXALTD_SERVICE, EXALTD_PATH,
                EXALTD_INTERFACE_READ, "NOTIFY_CONF_APPLIED",
                _exalt_dbus_notify_conf_applied, conn);
    }
    else
    {
        conn -> notify_conf_applied -> cb = cb;
        conn -> notify_conf_applied -> user_data = user_data;
    }


    msg = exalt_dbus_write_call_new("IFACE_APPLY_CONF");
    dbus_message_iter_init_append(msg, &args);
    if (!dbus_message_iter_append_basic(&args, DBUS_TYPE_STRING, &eth))
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

void _exalt_dbus_notify_conf_applied(void *data, DBusMessage *msg)
{
    DBusMessageIter args;
    char* eth;
    exalt_dbus_conn *conn;

    conn = (exalt_dbus_conn*)data;

    if(!dbus_message_iter_init(msg, &args))
    {
        exalt_dbus_print_error("ERROR", __FILE__, __LINE__,__func__, "no argument");
        return ;
    }
    if (DBUS_TYPE_STRING != dbus_message_iter_get_arg_type(&args))
    {
        exalt_dbus_print_error("ERROR", __FILE__, __LINE__,__func__, "the first argument is not a string");
        return ;
    }

    dbus_message_iter_get_basic(&args, &eth);
    eth = strdup(eth);

    if(conn->notify_conf_applied->cb)
        conn->notify_conf_applied->cb(eth,conn->notify_conf_applied->user_data);
}
