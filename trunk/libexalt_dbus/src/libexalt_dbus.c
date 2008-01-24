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
    conn->scan_notify=NULL;
    return conn;
}

void exalt_dbus_free(exalt_dbus_conn** conn)
{
    e_dbus_connection_close((*conn)->e_conn);
    EXALT_FREE((*conn)->notify);
    EXALT_FREE((*conn)->scan_notify);
    EXALT_FREE(*conn);
}

void exalt_dbus_shutdown()
{
    e_dbus_shutdown();
    ecore_shutdown();
}

void exalt_dbus_notify_set(exalt_dbus_conn* conn, exalt_notify_cb *cb, void* user_data)
{
    EXALT_ASSERT_RETURN_VOID(conn!=NULL);
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
    EXALT_ASSERT_RETURN_VOID(conn!=NULL);

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

void _exalt_dbus_notify(void *data, DBusMessage *msg)
{
    char* eth;
    int action;
    exalt_dbus_conn *conn;

    conn = (exalt_dbus_conn*)data;
    EXALT_ASSERT_RETURN_VOID(conn!=NULL);

    EXALT_ASSERT_ADV(exalt_dbus_valid_is(msg),
            ,
            "exalt_dbus_valid_is(msg) failed, error=%d (%s)",
            exalt_dbus_error_get_id(msg),
            exalt_dbus_error_get_msg(msg));

    EXALT_STRDUP(eth , exalt_dbus_response_string(msg,1));
    action = exalt_dbus_response_integer(msg,2);

    if(conn->notify->cb)
        conn->notify->cb(eth,action,conn->notify->user_data);
    EXALT_FREE(eth);
}

void _exalt_dbus_scan_notify(void *data, DBusMessage *msg)
{
    char* eth;
    Ecore_List* networks;
    exalt_dbus_conn *conn;

    conn = (exalt_dbus_conn*)data;
    EXALT_ASSERT_RETURN_VOID(conn!=NULL);

    EXALT_ASSERT_ADV(exalt_dbus_valid_is(msg),
            ,
            "exalt_dbus_valid_is(msg) failed, error=%d (%s)",
            exalt_dbus_error_get_id(msg),
            exalt_dbus_error_get_msg(msg));

    EXALT_STRDUP(eth,exalt_dbus_response_string(msg,1));

    networks = exalt_dbus_response_strings(msg,2);

    if(conn->scan_notify->cb)
        conn->scan_notify->cb(eth,networks,conn->scan_notify->user_data);

    EXALT_FREE(eth);
    ecore_list_destroy(networks);
}



