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

    EXALT_ASSERT_RETURN(conn!=NULL);
    EXALT_ASSERT_RETURN(eth!=NULL);

    msg = exalt_dbus_read_call_new("IFACE_GET_IP");
    dbus_message_iter_init_append(msg, &args);

    EXALT_ASSERT_ADV(dbus_message_iter_append_basic(&args, DBUS_TYPE_STRING, &eth),
            dbus_message_unref(msg); return 0,
            "dbus_message_iter_append_basic(&args, DBUS_TYPE_STRING, &eth");

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

    EXALT_STRDUP(res , exalt_dbus_response_string(msg,1));
    dbus_message_unref(msg);
    return res;
}

char* exalt_dbus_eth_get_netmask(const exalt_dbus_conn* conn, const char* eth)
{
    DBusPendingCall * ret;
    DBusMessage *msg;
    DBusMessageIter args;
    char* res;

    EXALT_ASSERT_RETURN(conn!=NULL);
    EXALT_ASSERT_RETURN(eth!=NULL);

    msg = exalt_dbus_read_call_new("IFACE_GET_NETMASK");
    dbus_message_iter_init_append(msg, &args);

    EXALT_ASSERT_ADV(dbus_message_iter_append_basic(&args, DBUS_TYPE_STRING, &eth),
            dbus_message_unref(msg); return 0,
            "dbus_message_iter_append_basic(&args, DBUS_TYPE_STRING, &eth");

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

    EXALT_STRDUP(res , exalt_dbus_response_string(msg,1));
    dbus_message_unref(msg);
    return res;
}

char* exalt_dbus_eth_get_gateway(const exalt_dbus_conn* conn, const char* eth)
{
    DBusPendingCall * ret;
    DBusMessage *msg;
    DBusMessageIter args;
    char* res;

    EXALT_ASSERT_RETURN(conn!=NULL);
    EXALT_ASSERT_RETURN(eth!=NULL);

    msg = exalt_dbus_read_call_new("IFACE_GET_GATEWAY");
    dbus_message_iter_init_append(msg, &args);

    EXALT_ASSERT_ADV(dbus_message_iter_append_basic(&args, DBUS_TYPE_STRING, &eth),
            dbus_message_unref(msg); return 0,
            "dbus_message_iter_append_basic(&args, DBUS_TYPE_STRING, &eth");

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

    EXALT_STRDUP(res , exalt_dbus_response_string(msg,1));
    dbus_message_unref(msg);
    return res;
}

Ecore_List* exalt_dbus_eth_get_list(const exalt_dbus_conn* conn)
{
    DBusPendingCall * ret;
    DBusMessage *msg;
    Ecore_List* res;

    EXALT_ASSERT_RETURN(conn!=NULL);

    msg = exalt_dbus_read_call_new("IFACE_GET_ETH_LIST");
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


    res = exalt_dbus_response_strings(msg,1);
    dbus_message_unref(msg);
    return res;
}

int exalt_dbus_eth_is_wireless(const exalt_dbus_conn* conn, const char* eth)
{
    DBusPendingCall * ret;
    DBusMessage *msg;
    DBusMessageIter args;
    int res;

    EXALT_ASSERT_RETURN(conn!=NULL);
    EXALT_ASSERT_RETURN(eth!=NULL);

    msg = exalt_dbus_read_call_new("IFACE_IS_WIRELESS");
    dbus_message_iter_init_append(msg, &args);

    EXALT_ASSERT_ADV(dbus_message_iter_append_basic(&args, DBUS_TYPE_STRING, &eth),
            dbus_message_unref(msg); return 0,
            "dbus_message_iter_append_basic(&args, DBUS_TYPE_STRING, &eth");

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

    res = exalt_dbus_response_boolean(msg,1);
    dbus_message_unref(msg);
    return res;
}


int exalt_dbus_eth_is_up(const exalt_dbus_conn* conn, const char* eth)
{
    DBusPendingCall * ret;
    DBusMessage *msg;
    DBusMessageIter args;
    int res;

    EXALT_ASSERT_RETURN(conn!=NULL);
    EXALT_ASSERT_RETURN(eth!=NULL);

    msg = exalt_dbus_read_call_new("IFACE_IS_UP");
    dbus_message_iter_init_append(msg, &args);

    EXALT_ASSERT_ADV(dbus_message_iter_append_basic(&args, DBUS_TYPE_STRING, &eth),
            dbus_message_unref(msg); return 0,
            "dbus_message_iter_append_basic(&args, DBUS_TYPE_STRING, &eth");

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

    res = exalt_dbus_response_boolean(msg,1);
    dbus_message_unref(msg);
    return res;
}

int exalt_dbus_eth_is_dhcp(const exalt_dbus_conn* conn, const char* eth)
{
    DBusPendingCall * ret;
    DBusMessage *msg;
    DBusMessageIter args;
    int res;

    EXALT_ASSERT_RETURN(conn!=NULL);
    EXALT_ASSERT_RETURN(eth!=NULL);

    msg = exalt_dbus_read_call_new("IFACE_IS_DHCP");
    dbus_message_iter_init_append(msg, &args);

    EXALT_ASSERT_ADV(dbus_message_iter_append_basic(&args, DBUS_TYPE_STRING, &eth),
            dbus_message_unref(msg); return 0,
            "dbus_message_iter_append_basic(&args, DBUS_TYPE_STRING, &eth");

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

    res = exalt_dbus_response_boolean(msg,1);
    dbus_message_unref(msg);
    return res;
}

int exalt_dbus_eth_is_link(const exalt_dbus_conn* conn, const char* eth)
{
    DBusPendingCall * ret;
    DBusMessage *msg;
    DBusMessageIter args;
    int res;

    EXALT_ASSERT_RETURN(conn!=NULL);
    EXALT_ASSERT_RETURN(eth!=NULL);

    msg = exalt_dbus_read_call_new("IFACE_IS_LINK");
    dbus_message_iter_init_append(msg, &args);

    EXALT_ASSERT_ADV(dbus_message_iter_append_basic(&args, DBUS_TYPE_STRING, &eth),
            dbus_message_unref(msg); return 0,
            "dbus_message_iter_append_basic(&args, DBUS_TYPE_STRING, &eth");

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

    res = exalt_dbus_response_boolean(msg,1);
    dbus_message_unref(msg);
    return res;
}




int exalt_dbus_eth_up(const exalt_dbus_conn* conn, const char* eth)
{
    DBusPendingCall * ret;
    DBusMessage *msg;
    DBusMessageIter args;

    EXALT_ASSERT_RETURN(conn!=NULL);
    EXALT_ASSERT_RETURN(eth!=NULL);

    msg = exalt_dbus_write_call_new("IFACE_UP");
    dbus_message_iter_init_append(msg, &args);

    EXALT_ASSERT_ADV(dbus_message_iter_append_basic(&args, DBUS_TYPE_STRING, &eth),
            dbus_message_unref(msg); return 0,
            "dbus_message_iter_append_basic(&args, DBUS_TYPE_STRING, &eth");

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

    dbus_message_unref(msg);
    return 1;
}

int exalt_dbus_eth_down(const exalt_dbus_conn* conn, const char* eth)
{
    DBusPendingCall * ret;
    DBusMessage *msg;
    DBusMessageIter args;

    EXALT_ASSERT_RETURN(conn!=NULL);
    EXALT_ASSERT_RETURN(eth!=NULL);

    msg = exalt_dbus_write_call_new("IFACE_DOWN");
    dbus_message_iter_init_append(msg, &args);
    EXALT_ASSERT_ADV(dbus_message_iter_append_basic(&args, DBUS_TYPE_STRING, &eth),
            dbus_message_unref(msg); return 0,
            "dbus_message_iter_append_basic(&args, DBUS_TYPE_STRING, &eth");

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


    dbus_message_unref(msg);
    return 1;
}

int exalt_dbus_eth_apply_conn(exalt_dbus_conn* conn, const char* eth, Exalt_Connection* c)
{
    DBusPendingCall * ret;
    DBusMessage *msg;
    DBusMessageIter args;
    int i;
    const char *s;

    EXALT_ASSERT_RETURN(conn!=NULL);
    EXALT_ASSERT_RETURN(eth!=NULL);

    EXALT_ASSERT_RETURN(exalt_conn_is_valid(c));

    msg = exalt_dbus_write_call_new("IFACE_APPLY_CONN");
    dbus_message_iter_init_append(msg, &args);


    EXALT_ASSERT_ADV(dbus_message_iter_append_basic(&args, DBUS_TYPE_STRING, &eth),
            dbus_message_unref(msg); return 0,
            "dbus_message_iter_append_basic(&args, DBUS_TYPE_STRING, &eth");


    //add the connection
    i=exalt_conn_get_mode(c);
    EXALT_ASSERT_ADV(dbus_message_iter_append_basic(&args, DBUS_TYPE_INT32, &i),
            dbus_message_unref(msg); return 0,
            "dbus_message_iter_append_basic(&args, DBUS_TYPE_INT32, &i");


    if(!exalt_conn_is_dhcp(c))
    {
        s = exalt_conn_get_ip(c);
        if(!s)
            s="";
        EXALT_ASSERT_ADV(dbus_message_iter_append_basic(&args, DBUS_TYPE_STRING, &s),
                dbus_message_unref(msg); return 0,
                "dbus_message_iter_append_basic(&args, DBUS_TYPE_STRING, &s");


        s = exalt_conn_get_netmask(c);
        if(!s)
            s="";
        EXALT_ASSERT_ADV(dbus_message_iter_append_basic(&args, DBUS_TYPE_STRING, &s),
                dbus_message_unref(msg); return 0,
                "dbus_message_iter_append_basic(&args, DBUS_TYPE_STRING, &s");

        s = exalt_conn_get_gateway(c);
        if(!s)
            s="";
        EXALT_ASSERT_ADV(dbus_message_iter_append_basic(&args, DBUS_TYPE_STRING, &s),
                dbus_message_unref(msg); return 0,
                "dbus_message_iter_append_basic(&args, DBUS_TYPE_STRING, &s");
    }

    i=exalt_conn_is_wireless(c);
    EXALT_ASSERT_ADV(dbus_message_iter_append_basic(&args, DBUS_TYPE_INT32, &i),
            dbus_message_unref(msg); return 0,
            "dbus_message_iter_append_basic(&args, DBUS_TYPE_INT32, &i");


    if(exalt_conn_is_wireless(c))
    {
        s = exalt_conn_get_essid(c);
        if(!s)
            s="";
        EXALT_ASSERT_ADV(dbus_message_iter_append_basic(&args, DBUS_TYPE_STRING, &s),
                dbus_message_unref(msg); return 0,
                "dbus_message_iter_append_basic(&args, DBUS_TYPE_STRING, &s");

        i=exalt_conn_get_encryption_mode(c);
        dbus_message_iter_append_basic(&args, DBUS_TYPE_INT32, &i);

        if(exalt_conn_get_encryption_mode(c)!=EXALT_ENCRYPTION_NONE)
        {
            s = exalt_conn_get_key(c);
            if(!s)
                s="";
            EXALT_ASSERT_ADV(dbus_message_iter_append_basic(&args, DBUS_TYPE_STRING, &s),
                    dbus_message_unref(msg); return 0,
                    "dbus_message_iter_append_basic(&args, DBUS_TYPE_STRING, &s");
        }
        i=exalt_conn_get_connection_mode(c);
        EXALT_ASSERT_ADV(dbus_message_iter_append_basic(&args, DBUS_TYPE_INT32, &i),
                dbus_message_unref(msg); return 0,
                "dbus_message_iter_append_basic(&args, DBUS_TYPE_INT32, &i");
        i=exalt_conn_get_security_mode(c);
        EXALT_ASSERT_ADV(dbus_message_iter_append_basic(&args, DBUS_TYPE_INT32, &i),
                dbus_message_unref(msg); return 0,
                "dbus_message_iter_append_basic(&args, DBUS_TYPE_INT32, &i");
    }


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

    dbus_message_unref(msg);

    return 1;
}

