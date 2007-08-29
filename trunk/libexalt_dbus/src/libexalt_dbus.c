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
    return conn;
}

void exalt_dbus_shutdown()
{
    e_dbus_shutdown();
    ecore_shutdown();
}

char* exalt_dbus_response_string(DBusMessage *msg)
{
    DBusMessageIter args;
    char* res;

    if(!dbus_message_iter_init(msg, &args))
        printf("exalt_dbus_response_string(): no argument ! \n");

    if (DBUS_TYPE_STRING != dbus_message_iter_get_arg_type(&args))
        fprintf(stderr, "exalt_dbus_response_string(): Argument is not a string!\n");
    else
    {
        dbus_message_iter_get_basic(&args, &res);
        return strdup(res);
    }
    return NULL;
}

Ecore_List* exalt_dbus_response_strings(DBusMessage *msg)
{
    DBusMessageIter args;
    Ecore_List* res;
    char* val;

    if(!dbus_message_iter_init(msg, &args))
        printf("exalt_dbus_response_strings(): no argument ! \n");

    res = ecore_list_new();
    res ->free_func = ECORE_FREE_CB(exalt_dbus_string_free);

    while (DBUS_TYPE_STRING == dbus_message_iter_get_arg_type(&args))
    {
        dbus_message_iter_get_basic(&args, &val);
        ecore_list_append(res,(void *)strdup(val));
        dbus_message_iter_next(&args);
    }

    return res;
}

int exalt_dbus_response_boolean(DBusMessage *msg)
{
    DBusMessageIter args;
    int res;

    if(!dbus_message_iter_init(msg, &args))
        printf("exalt_dbus_response_boolean(): no argument ! \n");

    if (DBUS_TYPE_BOOLEAN != dbus_message_iter_get_arg_type(&args))
        fprintf(stderr, "exalt_dbus_response_boolean(): Argument is not a boolean!\n");
    else
    {
        dbus_message_iter_get_basic(&args, &res);
        return res;
    }
    return 0;
}

void exalt_dbus_string_free(void* data)
{
    EXALT_DBUS_FREE((char*)data);
}
