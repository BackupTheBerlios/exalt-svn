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

char* exalt_dbus_eth_get_ip(exalt_dbus_conn* conn, char* eth)
{
    DBusPendingCall * ret;
    DBusMessage *msg;
    DBusMessageIter args;
    char* res;

    if(!conn || !eth)
    {
        fprintf(stderr,"exalt_dbus_eth_get_ip(): conn==%p, eth==%p \n",conn, eth);
        return NULL;
    }

    msg = exalt_dbus_read_call_new("GET_IP");
    dbus_message_iter_init_append(msg, &args);
    if (!dbus_message_iter_append_basic(&args, DBUS_TYPE_STRING, &eth)) {
        fprintf(stderr, "exalt_dbus_eth_get_ip(): Out Of Memory!\n");
        dbus_message_unref(msg);
        return NULL;
    }
    dbus_connection_send_with_reply (conn->conn, msg, &ret, -1);
    dbus_pending_call_block(ret);
    msg = dbus_pending_call_steal_reply(ret);
    dbus_pending_call_unref(ret);

    //read the response
    res = exalt_dbus_response_string(msg);
    dbus_message_unref(msg);
    return res;
}

char* exalt_dbus_eth_get_netmask(exalt_dbus_conn* conn, char* eth)
{
    DBusPendingCall * ret;
    DBusMessage *msg;
    DBusMessageIter args;
    char* res;

    if(!conn || !eth)
    {
        fprintf(stderr,"exalt_dbus_eth_get_ip(): conn==%p, eth==%p \n",conn, eth);
        return NULL;
    }

    msg = exalt_dbus_read_call_new("GET_NETMASK");
    dbus_message_iter_init_append(msg, &args);
    if (!dbus_message_iter_append_basic(&args, DBUS_TYPE_STRING, &eth)) {
        fprintf(stderr, "exalt_dbus_eth_get_ip(): Out Of Memory!\n");
        dbus_message_unref(msg);
        return NULL;
    }
    dbus_connection_send_with_reply (conn->conn, msg, &ret, -1);
    dbus_pending_call_block(ret);
    msg = dbus_pending_call_steal_reply(ret);
    dbus_pending_call_unref(ret);

    //read the response
    res = exalt_dbus_response_string(msg);
    dbus_message_unref(msg);
    return res;
}

char* exalt_dbus_eth_get_gateway(exalt_dbus_conn* conn, char* eth)
{
    DBusPendingCall * ret;
    DBusMessage *msg;
    DBusMessageIter args;
    char* res;

    if(!conn || !eth)
    {
        fprintf(stderr,"exalt_dbus_eth_get_ip(): conn==%p, eth==%p \n",conn, eth);
        return NULL;
    }

    msg = exalt_dbus_read_call_new("GET_GATEWAY");
    dbus_message_iter_init_append(msg, &args);
    if (!dbus_message_iter_append_basic(&args, DBUS_TYPE_STRING, &eth)) {
        fprintf(stderr, "exalt_dbus_eth_get_ip(): Out Of Memory!\n");
        dbus_message_unref(msg);
        return NULL;
    }
    dbus_connection_send_with_reply (conn->conn, msg, &ret, -1);
    dbus_pending_call_block(ret);
    msg = dbus_pending_call_steal_reply(ret);
    dbus_pending_call_unref(ret);

    //read the response
    res = exalt_dbus_response_string(msg);
    dbus_message_unref(msg);
    return res;
}

Ecore_List* exalt_dbus_eth_get_list(exalt_dbus_conn* conn)
{
    DBusPendingCall * ret;
    DBusMessage *msg;
    Ecore_List* res;

    if(!conn)
    {
        fprintf(stderr,"exalt_dbus_eth_get_ip(): conn==%p \n",conn);
        return NULL;
    }

    msg = exalt_dbus_read_call_new("GET_ETH_LIST");
    dbus_connection_send_with_reply (conn->conn, msg, &ret, -1);
    dbus_pending_call_block(ret);
    msg = dbus_pending_call_steal_reply(ret);
    dbus_pending_call_unref(ret);

    //read the response
    res = exalt_dbus_response_strings(msg);
    dbus_message_unref(msg);
    return res;
}

int exalt_dbus_eth_is_wireless(exalt_dbus_conn* conn, char* eth)
{
    DBusPendingCall * ret;
    DBusMessage *msg;
    DBusMessageIter args;
    int res;

    if(!conn || !eth)
    {
        fprintf(stderr,"exalt_dbus_eth_is_wireless(): conn==%p, eth==%p \n",conn, eth);
        return 0;
    }

    msg = exalt_dbus_read_call_new("IS_WIRELESS");
    dbus_message_iter_init_append(msg, &args);
    if (!dbus_message_iter_append_basic(&args, DBUS_TYPE_STRING, &eth)) {
        fprintf(stderr, "exalt_dbus_eth_is_wireless(): Out Of Memory!\n");
        dbus_message_unref(msg);
        return 0;
    }
    dbus_connection_send_with_reply (conn->conn, msg, &ret, -1);
    dbus_pending_call_block(ret);
    msg = dbus_pending_call_steal_reply(ret);
    dbus_pending_call_unref(ret);

    //read the response
    res = exalt_dbus_response_boolean(msg);
    dbus_message_unref(msg);
    return res;
}

int exalt_dbus_eth_is_link(exalt_dbus_conn* conn, char* eth)
{
    DBusPendingCall * ret;
    DBusMessage *msg;
    DBusMessageIter args;
    int res;

    if(!conn || !eth)
    {
        fprintf(stderr,"exalt_dbus_eth_is_link(): conn==%p, eth==%p \n",conn, eth);
        return 0;
    }

    msg = exalt_dbus_read_call_new("IS_LINK");
    dbus_message_iter_init_append(msg, &args);
    if (!dbus_message_iter_append_basic(&args, DBUS_TYPE_STRING, &eth)) {
        fprintf(stderr, "exalt_dbus_eth_is_link(): Out Of Memory!\n");
        dbus_message_unref(msg);
        return 0;
    }
    dbus_connection_send_with_reply (conn->conn, msg, &ret, -1);
    dbus_pending_call_block(ret);
    msg = dbus_pending_call_steal_reply(ret);
    dbus_pending_call_unref(ret);

    //read the response
    res = exalt_dbus_response_boolean(msg);
    dbus_message_unref(msg);
    return res;
}

int exalt_dbus_eth_is_up(exalt_dbus_conn* conn, char* eth)
{
    DBusPendingCall * ret;
    DBusMessage *msg;
    DBusMessageIter args;
    int res;

    if(!conn || !eth)
    {
        fprintf(stderr,"exalt_dbus_eth_is_up(): conn==%p, eth==%p \n",conn, eth);
        return 0;
    }

    msg = exalt_dbus_read_call_new("IS_UP");
    dbus_message_iter_init_append(msg, &args);
    if (!dbus_message_iter_append_basic(&args, DBUS_TYPE_STRING, &eth)) {
        fprintf(stderr, "exalt_dbus_eth_is_up(): Out Of Memory!\n");
        dbus_message_unref(msg);
        return 0;
    }
    dbus_connection_send_with_reply (conn->conn, msg, &ret, -1);
    dbus_pending_call_block(ret);
    msg = dbus_pending_call_steal_reply(ret);
    dbus_pending_call_unref(ret);

    //read the response
    res = exalt_dbus_response_boolean(msg);
    dbus_message_unref(msg);
    return res;
}
