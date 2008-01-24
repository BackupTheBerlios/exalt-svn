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

    EXALT_ASSERT_RETURN(conn!=NULL);
    EXALT_ASSERT_RETURN(eth!=NULL);
    EXALT_ASSERT_RETURN(essid!=NULL);

    msg = exalt_dbus_read_call_new("NETWORK_GET_QUALITY");
    dbus_message_iter_init_append(msg, &args);
    EXALT_ASSERT_ADV(dbus_message_iter_append_basic(&args, DBUS_TYPE_STRING, &eth),
            dbus_message_unref(msg); return 0,
            "bus_message_iter_append_basic(&args, DBUS_TYPE_STRING, &eth");

    EXALT_ASSERT_ADV(dbus_message_iter_append_basic(&args, DBUS_TYPE_STRING, &essid),
            dbus_message_unref(msg); return 0,
            "bus_message_iter_append_basic(&args, DBUS_TYPE_STRING, &essid");

    EXALT_ASSERT_ADV(dbus_connection_send_with_reply (conn->conn, msg, &ret, -1),
            dbus_message_unref(msg); return 0,
            "dbus_connection_send_with_reply (conn->conn, msg, &ret, -1)");

    dbus_pending_call_block(ret);
    msg = dbus_pending_call_steal_reply(ret);
    EXALT_ASSERT_RETURN(msg!=NULL);
    dbus_pending_call_unref(ret);
    EXALT_ASSERT_ADV(exalt_dbus_valid_is(msg),
            return 0,
            "exalt_dbus_valid_is(msg) failed, error=%d (%s)",
            exalt_dbus_error_get_id(msg),
            exalt_dbus_error_get_msg(msg));

    res = exalt_dbus_response_integer(msg,1);
    dbus_message_unref(msg);
    return res;
}


int exalt_dbus_wirelessinfo_get_encryption(const exalt_dbus_conn* conn, const char* eth, const char* essid)
{
    DBusPendingCall * ret;
    DBusMessage *msg;
    DBusMessageIter args;
    int res;

    EXALT_ASSERT_RETURN(conn!=NULL);
    EXALT_ASSERT_RETURN(eth!=NULL);
    EXALT_ASSERT_RETURN(essid!=NULL);

    msg = exalt_dbus_read_call_new("NETWORK_GET_ENCRYPTION");
    dbus_message_iter_init_append(msg, &args);
    EXALT_ASSERT_ADV(dbus_message_iter_append_basic(&args, DBUS_TYPE_STRING, &eth),
            dbus_message_unref(msg); return 0,
            "bus_message_iter_append_basic(&args, DBUS_TYPE_STRING, &eth");

    EXALT_ASSERT_ADV(dbus_message_iter_append_basic(&args, DBUS_TYPE_STRING, &essid),
            dbus_message_unref(msg); return 0,
            "bus_message_iter_append_basic(&args, DBUS_TYPE_STRING, &essid");

    EXALT_ASSERT_ADV(dbus_connection_send_with_reply (conn->conn, msg, &ret, -1),
            dbus_message_unref(msg); return 0,
            "dbus_connection_send_with_reply (conn->conn, msg, &ret, -1)");

    dbus_pending_call_block(ret);
    msg = dbus_pending_call_steal_reply(ret);
    EXALT_ASSERT_RETURN(msg!=NULL);
    dbus_pending_call_unref(ret);
    EXALT_ASSERT_ADV(exalt_dbus_valid_is(msg),
            return 0,
            "exalt_dbus_valid_is(msg) failed, error=%d (%s)",
            exalt_dbus_error_get_id(msg),
            exalt_dbus_error_get_msg(msg));

    res = exalt_dbus_response_integer(msg,1);
    dbus_message_unref(msg);
    return res;
}

int exalt_dbus_wirelessinfo_get_signallvl(const exalt_dbus_conn* conn, const char* eth, const char* essid)
{
    DBusPendingCall * ret;
    DBusMessage *msg;
    DBusMessageIter args;
    int res;

    EXALT_ASSERT_RETURN(conn!=NULL);
    EXALT_ASSERT_RETURN(eth!=NULL);
    EXALT_ASSERT_RETURN(essid!=NULL);

    msg = exalt_dbus_read_call_new("NETWORK_GET_SIGNALLVL");
    dbus_message_iter_init_append(msg, &args);
    EXALT_ASSERT_ADV(dbus_message_iter_append_basic(&args, DBUS_TYPE_STRING, &eth),
            dbus_message_unref(msg); return 0,
            "bus_message_iter_append_basic(&args, DBUS_TYPE_STRING, &eth");

    EXALT_ASSERT_ADV(dbus_message_iter_append_basic(&args, DBUS_TYPE_STRING, &essid),
            dbus_message_unref(msg); return 0,
            "bus_message_iter_append_basic(&args, DBUS_TYPE_STRING, &essid");

    EXALT_ASSERT_ADV(dbus_connection_send_with_reply (conn->conn, msg, &ret, -1),
            dbus_message_unref(msg); return 0,
            "dbus_connection_send_with_reply (conn->conn, msg, &ret, -1)");

    dbus_pending_call_block(ret);
    msg = dbus_pending_call_steal_reply(ret);
    EXALT_ASSERT_RETURN(msg!=NULL);
    dbus_pending_call_unref(ret);
    EXALT_ASSERT_ADV(exalt_dbus_valid_is(msg),
            return 0,
            "exalt_dbus_valid_is(msg) failed, error=%d (%s)",
            exalt_dbus_error_get_id(msg),
            exalt_dbus_error_get_msg(msg));

    res = exalt_dbus_response_integer(msg,1);
    dbus_message_unref(msg);
    return res;
}

int exalt_dbus_wirelessinfo_get_noiselvl(const exalt_dbus_conn* conn, const char* eth, const char* essid)
{
    DBusPendingCall * ret;
    DBusMessage *msg;
    DBusMessageIter args;
    int res;

    EXALT_ASSERT_RETURN(conn!=NULL);
    EXALT_ASSERT_RETURN(eth!=NULL);
    EXALT_ASSERT_RETURN(essid!=NULL);

    msg = exalt_dbus_read_call_new("NETWORK_GET_NOISELVL");
    dbus_message_iter_init_append(msg, &args);
    EXALT_ASSERT_ADV(dbus_message_iter_append_basic(&args, DBUS_TYPE_STRING, &eth),
            dbus_message_unref(msg); return 0,
            "bus_message_iter_append_basic(&args, DBUS_TYPE_STRING, &eth");

    EXALT_ASSERT_ADV(dbus_message_iter_append_basic(&args, DBUS_TYPE_STRING, &essid),
            dbus_message_unref(msg); return 0,
            "bus_message_iter_append_basic(&args, DBUS_TYPE_STRING, &essid");

    EXALT_ASSERT_ADV(dbus_connection_send_with_reply (conn->conn, msg, &ret, -1),
            dbus_message_unref(msg); return 0,
            "dbus_connection_send_with_reply (conn->conn, msg, &ret, -1)");

    dbus_pending_call_block(ret);
    msg = dbus_pending_call_steal_reply(ret);
    EXALT_ASSERT_RETURN(msg!=NULL);
    dbus_pending_call_unref(ret);
    EXALT_ASSERT_ADV(exalt_dbus_valid_is(msg),
            return 0,
            "exalt_dbus_valid_is(msg) failed, error=%d (%s)",
            exalt_dbus_error_get_id(msg),
            exalt_dbus_error_get_msg(msg));

    res = exalt_dbus_response_integer(msg,1);
    dbus_message_unref(msg);
    return res;
}


char* exalt_dbus_wirelessinfo_get_addr(const exalt_dbus_conn* conn, const char* eth, const char* essid)
{
    DBusPendingCall * ret;
    DBusMessage *msg;
    DBusMessageIter args;
    char* res;

    EXALT_ASSERT_RETURN(conn!=NULL);
    EXALT_ASSERT_RETURN(eth!=NULL);
    EXALT_ASSERT_RETURN(essid!=NULL);

    msg = exalt_dbus_read_call_new("NETWORK_GET_ADDR");
    dbus_message_iter_init_append(msg, &args);
    EXALT_ASSERT_ADV(dbus_message_iter_append_basic(&args, DBUS_TYPE_STRING, &eth),
            dbus_message_unref(msg); return 0,
            "bus_message_iter_append_basic(&args, DBUS_TYPE_STRING, &eth");

    EXALT_ASSERT_ADV(dbus_message_iter_append_basic(&args, DBUS_TYPE_STRING, &essid),
            dbus_message_unref(msg); return 0,
            "bus_message_iter_append_basic(&args, DBUS_TYPE_STRING, &essid");

    EXALT_ASSERT_ADV(dbus_connection_send_with_reply (conn->conn, msg, &ret, -1),
            dbus_message_unref(msg); return 0,
            "dbus_connection_send_with_reply (conn->conn, msg, &ret, -1)");

    dbus_pending_call_block(ret);
    msg = dbus_pending_call_steal_reply(ret);
    EXALT_ASSERT_RETURN(msg!=NULL);
    dbus_pending_call_unref(ret);
    EXALT_ASSERT_ADV(exalt_dbus_valid_is(msg),
            return 0,
            "exalt_dbus_valid_is(msg) failed, error=%d (%s)",
            exalt_dbus_error_get_id(msg),
            exalt_dbus_error_get_msg(msg));

    EXALT_STRDUP(res, exalt_dbus_response_string(msg,1));
    dbus_message_unref(msg);
    return res;
}



char* exalt_dbus_wirelessinfo_get_bitrates(const exalt_dbus_conn* conn, const char* eth, const char* essid)
{
    DBusPendingCall * ret;
    DBusMessage *msg;
    DBusMessageIter args;
    char* res;

    EXALT_ASSERT_RETURN(conn!=NULL);
    EXALT_ASSERT_RETURN(eth!=NULL);
    EXALT_ASSERT_RETURN(essid!=NULL);

    msg = exalt_dbus_read_call_new("NETWORK_GET_BITRATES");
    dbus_message_iter_init_append(msg, &args);
    EXALT_ASSERT_ADV(dbus_message_iter_append_basic(&args, DBUS_TYPE_STRING, &eth),
            dbus_message_unref(msg); return 0,
            "bus_message_iter_append_basic(&args, DBUS_TYPE_STRING, &eth");

    EXALT_ASSERT_ADV(dbus_message_iter_append_basic(&args, DBUS_TYPE_STRING, &essid),
            dbus_message_unref(msg); return 0,
            "bus_message_iter_append_basic(&args, DBUS_TYPE_STRING, &essid");

    EXALT_ASSERT_ADV(dbus_connection_send_with_reply (conn->conn, msg, &ret, -1),
            dbus_message_unref(msg); return 0,
            "dbus_connection_send_with_reply (conn->conn, msg, &ret, -1)");

    dbus_pending_call_block(ret);
    msg = dbus_pending_call_steal_reply(ret);
    EXALT_ASSERT_RETURN(msg!=NULL);
    dbus_pending_call_unref(ret);
    EXALT_ASSERT_ADV(exalt_dbus_valid_is(msg),
            return 0,
            "exalt_dbus_valid_is(msg) failed, error=%d (%s)",
            exalt_dbus_error_get_id(msg),
            exalt_dbus_error_get_msg(msg));

    EXALT_STRDUP(res, exalt_dbus_response_string(msg,1));
    dbus_message_unref(msg);
    return res;
}

char* exalt_dbus_wirelessinfo_get_mode(const exalt_dbus_conn* conn, const char* eth, const char* essid)
{
    DBusPendingCall * ret;
    DBusMessage *msg;
    DBusMessageIter args;
    char* res;

    EXALT_ASSERT_RETURN(conn!=NULL);
    EXALT_ASSERT_RETURN(eth!=NULL);
    EXALT_ASSERT_RETURN(essid!=NULL);

    msg = exalt_dbus_read_call_new("NETWORK_GET_MODE");
    dbus_message_iter_init_append(msg, &args);
    EXALT_ASSERT_ADV(dbus_message_iter_append_basic(&args, DBUS_TYPE_STRING, &eth),
            dbus_message_unref(msg); return 0,
            "bus_message_iter_append_basic(&args, DBUS_TYPE_STRING, &eth");

    EXALT_ASSERT_ADV(dbus_message_iter_append_basic(&args, DBUS_TYPE_STRING, &essid),
            dbus_message_unref(msg); return 0,
            "bus_message_iter_append_basic(&args, DBUS_TYPE_STRING, &essid");

    EXALT_ASSERT_ADV(dbus_connection_send_with_reply (conn->conn, msg, &ret, -1),
            dbus_message_unref(msg); return 0,
            "dbus_connection_send_with_reply (conn->conn, msg, &ret, -1)");

    dbus_pending_call_block(ret);
    msg = dbus_pending_call_steal_reply(ret);
    EXALT_ASSERT_RETURN(msg!=NULL);
    dbus_pending_call_unref(ret);
    EXALT_ASSERT_ADV(exalt_dbus_valid_is(msg),
            return 0,
            "exalt_dbus_valid_is(msg) failed, error=%d (%s)",
            exalt_dbus_error_get_id(msg),
            exalt_dbus_error_get_msg(msg));

    EXALT_STRDUP(res, exalt_dbus_response_string(msg,1));
    dbus_message_unref(msg);
    return res;
}

char* exalt_dbus_wirelessinfo_get_protocol(const exalt_dbus_conn* conn, const char* eth, const char* essid)
{
    DBusPendingCall * ret;
    DBusMessage *msg;
    DBusMessageIter args;
    char* res;

    EXALT_ASSERT_RETURN(conn!=NULL);
    EXALT_ASSERT_RETURN(eth!=NULL);
    EXALT_ASSERT_RETURN(essid!=NULL);

    msg = exalt_dbus_read_call_new("NETWORK_GET_PROTOCOL");
    dbus_message_iter_init_append(msg, &args);
    EXALT_ASSERT_ADV(dbus_message_iter_append_basic(&args, DBUS_TYPE_STRING, &eth),
            dbus_message_unref(msg); return 0,
            "bus_message_iter_append_basic(&args, DBUS_TYPE_STRING, &eth");

    EXALT_ASSERT_ADV(dbus_message_iter_append_basic(&args, DBUS_TYPE_STRING, &essid),
            dbus_message_unref(msg); return 0,
            "bus_message_iter_append_basic(&args, DBUS_TYPE_STRING, &essid");

    EXALT_ASSERT_ADV(dbus_connection_send_with_reply (conn->conn, msg, &ret, -1),
            dbus_message_unref(msg); return 0,
            "dbus_connection_send_with_reply (conn->conn, msg, &ret, -1)");

    dbus_pending_call_block(ret);
    msg = dbus_pending_call_steal_reply(ret);
    EXALT_ASSERT_RETURN(msg!=NULL);
    dbus_pending_call_unref(ret);
    EXALT_ASSERT_ADV(exalt_dbus_valid_is(msg),
            return 0,
            "exalt_dbus_valid_is(msg) failed, error=%d (%s)",
            exalt_dbus_error_get_id(msg),
            exalt_dbus_error_get_msg(msg));

    EXALT_STRDUP(res, exalt_dbus_response_string(msg,1));
    dbus_message_unref(msg);
    return res;
}




Exalt_Connection * exalt_dbus_wirelessinfo_get_default_conn(const exalt_dbus_conn* conn, const char* eth, const char* essid)
{
    DBusPendingCall * ret;
    DBusMessage *msg;
    DBusMessageIter args;
    int pos;
    Exalt_Connection *c;

    EXALT_ASSERT_RETURN(conn!=NULL);
    EXALT_ASSERT_RETURN(eth!=NULL);
    EXALT_ASSERT_RETURN(essid!=NULL);

    msg = exalt_dbus_read_call_new("NETWORK_GET_DEFAULT_CONN");
    dbus_message_iter_init_append(msg, &args);
    EXALT_ASSERT_ADV(dbus_message_iter_append_basic(&args, DBUS_TYPE_STRING, &eth),
            dbus_message_unref(msg); return 0,
            "bus_message_iter_append_basic(&args, DBUS_TYPE_STRING, &eth");

    EXALT_ASSERT_ADV(dbus_message_iter_append_basic(&args, DBUS_TYPE_STRING, &essid),
            dbus_message_unref(msg); return 0,
            "bus_message_iter_append_basic(&args, DBUS_TYPE_STRING, &essid");

    EXALT_ASSERT_ADV(dbus_connection_send_with_reply (conn->conn, msg, &ret, -1),
            dbus_message_unref(msg); return 0,
            "dbus_connection_send_with_reply (conn->conn, msg, &ret, -1)");

    dbus_message_unref(msg);

    dbus_pending_call_block(ret);
    msg = dbus_pending_call_steal_reply(ret);
    EXALT_ASSERT_RETURN(msg!=NULL);
    dbus_pending_call_unref(ret);

    EXALT_ASSERT_ADV(exalt_dbus_valid_is(msg),
            return 0,
            "exalt_dbus_valid_is(msg) failed, error=%d (%s)",
            exalt_dbus_error_get_id(msg),
            exalt_dbus_error_get_msg(msg));



    c = exalt_conn_new();
    pos = 1;
    exalt_conn_set_mode(c,exalt_dbus_response_integer(msg,pos++));
    if(!exalt_conn_is_dhcp(c))
    {
        exalt_conn_set_ip(c,exalt_dbus_response_string(msg,pos++));
        exalt_conn_set_netmask(c,exalt_dbus_response_string(msg,pos++));
        exalt_conn_set_gateway(c,exalt_dbus_response_string(msg,pos++));
    }

    exalt_conn_set_wireless(c,exalt_dbus_response_integer(msg,pos++));

    if(exalt_conn_is_wireless(c))
    {
        exalt_conn_set_encryption_mode(c,exalt_dbus_response_integer(msg,pos++));

        if(exalt_conn_get_encryption_mode(c)!= EXALT_ENCRYPTION_NONE)
            exalt_conn_set_key(c,exalt_dbus_response_string(msg,pos++));

        exalt_conn_set_connection_mode(c,exalt_dbus_response_integer(msg,pos++));

        exalt_conn_set_security_mode(c,exalt_dbus_response_integer(msg,pos++));
    }

    dbus_message_unref(msg);
    return c;
}

