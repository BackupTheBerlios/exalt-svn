/*
 * =====================================================================================
 *
 *       Filename:  libexalt_dbus.c
 *
 *    Description:  defines generals functions
 *
 *        Version:  1.0
 *        Created:  08/28/2007 04:17:06 PM CEST
 *       Revision:  none
 *       Compiler:  gcc
 *
 *         Author:   (Watchwolf), Atton Jonathan <watchwolf@watchwolf.fr>
 *        Company:
 *
 * =====================================================================================
 */

#include "libexalt_dbus.h"
#include "libexalt_dbus_private.h"

void _exalt_dbus_notify(void *data,DBusMessage *msg);
void _exalt_dbus_scan_notify(void *data, DBusMessage *msg);


void exalt_dbus_init()
{
    ecore_init();
    e_dbus_init();
}

exalt_dbus_conn* exalt_dbus_connect()
{
    exalt_dbus_conn* conn;
    DBusError err;
    conn = malloc(sizeof(exalt_dbus_conn));
    //initialise the errors
    dbus_error_init(&err);
    // connect to the bus
    conn->conn = dbus_bus_get(DBUS_BUS_SYSTEM, &err);
    conn->e_conn = e_dbus_connection_setup(conn->conn);
    conn->notify=NULL;
    return conn;
}

void exalt_dbus_shutdown()
{
    e_dbus_shutdown();
    ecore_shutdown();
}

void exalt_dbus_notify_set(exalt_dbus_conn* conn, exalt_notify_cb *cb, void* user_data)
{
    if(!conn)
    {
        exalt_dbus_print_error("ERROR", __FILE__, __LINE__,__func__, "conn=%p",conn);
        return ;
    }

    //better to delete the handler
    //but e_dbus doesn't have a function ...

    if(!conn->notify)
    {
        conn -> notify = malloc(sizeof(exalt_dbus_notify_data));
        conn -> notify -> cb = cb;
        conn -> notify -> user_data = user_data;

        e_dbus_signal_handler_add(conn->e_conn, EXALTD_SERVICE, EXALTD_PATH,
                EXALTD_INTERFACE_READ, "NOTIFY",
                _exalt_dbus_notify, conn);
    }
    else
    {
        conn -> notify -> cb = cb;
        conn -> notify -> user_data = user_data;
    }
}

void exalt_dbus_scan_notify_set(exalt_dbus_conn* conn, exalt_scan_notify_cb *cb, void* user_data)
{
    if(!conn)
    {
        exalt_dbus_print_error("ERROR", __FILE__, __LINE__,__func__, "conn=%p",conn);
        return ;
    }

    //better to delete the handler
    //but e_dbus doesn't have a function ...

    if(!conn->scan_notify)
    {
        conn -> scan_notify = malloc(sizeof(exalt_dbus_scan_notify_data));
        conn -> scan_notify -> cb = cb;
        conn -> scan_notify -> user_data = user_data;

        e_dbus_signal_handler_add(conn->e_conn, EXALTD_SERVICE, EXALTD_PATH,
                EXALTD_INTERFACE_READ, "SCAN_NOTIFY",
                _exalt_dbus_scan_notify, conn);
    }
    else
    {
        conn -> scan_notify -> cb = cb;
        conn -> scan_notify -> user_data = user_data;
    }
}



int exalt_dbus_is_address(const exalt_dbus_conn* conn, const char* address)
{
    DBusPendingCall * ret;
    DBusMessage *msg;
    DBusMessageIter args;
    int res;

    if(!conn || !address)
    {
        exalt_dbus_print_error("ERROR", __FILE__,__LINE__,__func__,"conn=%p, address=%p ",conn, address);
        return 0;
    }

    msg = exalt_dbus_read_call_new("IS_ADDRESS");
    dbus_message_iter_init_append(msg, &args);
    if (!dbus_message_iter_append_basic(&args, DBUS_TYPE_STRING, &address)) {
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

int exalt_dbus_is_essid(const exalt_dbus_conn* conn, const char* essid)
{
    DBusPendingCall * ret;
    DBusMessage *msg;
    DBusMessageIter args;
    int res;

    if(!conn || !essid)
    {
        exalt_dbus_print_error("ERROR", __FILE__,__LINE__,__func__,"conn=%p, essid=%p ",conn, essid);
        return 0;
    }

    msg = exalt_dbus_read_call_new("IS_ESSID");
    dbus_message_iter_init_append(msg, &args);
    if (!dbus_message_iter_append_basic(&args, DBUS_TYPE_STRING, &essid)) {
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


int exalt_dbus_is_passwd(const exalt_dbus_conn* conn, const char* passwd, int mode)
{
    DBusPendingCall * ret;
    DBusMessage *msg;
    DBusMessageIter args;
    int res;

    if(!conn || !passwd)
    {
        exalt_dbus_print_error("ERROR", __FILE__,__LINE__,__func__,"conn=%p, passwd=%p ",conn, passwd);
        return 0;
    }

    msg = exalt_dbus_read_call_new("IS_PASSWD");
    dbus_message_iter_init_append(msg, &args);
    if (!dbus_message_iter_append_basic(&args, DBUS_TYPE_STRING, &passwd)) {
        exalt_dbus_print_error("ERROR", __FILE__,__LINE__,__func__,"Out Of Memory!");
        dbus_message_unref(msg);
        return 0;
    }
    if (!dbus_message_iter_append_basic(&args, DBUS_TYPE_INT32, &mode)) {
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



void _exalt_dbus_notify(void *data, DBusMessage *msg)
{
    DBusMessageIter args;
    char* eth;
    int action;
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

    dbus_message_iter_next(&args);
    if (DBUS_TYPE_UINT32 != dbus_message_iter_get_arg_type(&args))
    {
        exalt_dbus_print_error("ERROR", __FILE__, __LINE__,__func__, "the second argument is not an integer");
        return ;
    }

    dbus_message_iter_get_basic(&args, &action);

    if(conn->notify->cb)
        conn->notify->cb(eth,action,conn->notify->user_data);
}

void _exalt_dbus_scan_notify(void *data, DBusMessage *msg)
{
    DBusMessageIter args;
    char* eth, *essid;
    Ecore_List* new_networks, *old_networks, *l;
    exalt_dbus_conn *conn;

    new_networks = ecore_list_new();
    new_networks ->free_func = ECORE_FREE_CB(exalt_dbus_string_free);

    old_networks = ecore_list_new();
    old_networks ->free_func = ECORE_FREE_CB(exalt_dbus_string_free);

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

    l = new_networks;
    while(l && dbus_message_iter_next(&args))
    {
        if (DBUS_TYPE_STRING != dbus_message_iter_get_arg_type(&args))
        {
            if (DBUS_TYPE_INT32 != dbus_message_iter_get_arg_type(&args))
            {
                exalt_dbus_print_error("ERROR", __FILE__, __LINE__,__func__, "the argument is not a string and not an integer");
                EXALT_DBUS_FREE(eth);
                ecore_list_destroy(new_networks);
                ecore_list_destroy(old_networks);
                return ;
            }
            //we are in the separator
            if(l == old_networks)
                    l = NULL;
            else
                l = old_networks;
        }
        else
        {
            dbus_message_iter_get_basic(&args, &essid);
            ecore_list_append(l,strdup(essid));
        }

    }

    if(conn->scan_notify->cb)
        conn->scan_notify->cb(eth,new_networks, old_networks,conn->scan_notify->user_data);

    EXALT_DBUS_FREE(eth);
    ecore_list_destroy(new_networks);
    ecore_list_destroy(old_networks);
}



