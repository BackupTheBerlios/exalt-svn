/*
 * =====================================================================================
 *
 *       Filename:  cb_bootprocess.c
 *
 *    Description:  dbus callback about the boot process
 *
 *        Version:  1.0
 *        Created:  01/02/2008 06:44:24 PM CET
 *       Revision:  none
 *       Compiler:  gcc
 *
 *         Author:  (Watchwolf), Atton Jonathan <watchwolf@watchwolf.fr>
 *        Company:
 *
 * =====================================================================================
 */
#include "cb_bootprocess.h"

DBusMessage * dbus_cb_bootprocess_iface_add(E_DBus_Object *obj __UNUSED__, DBusMessage *msg)
{
    DBusMessage *reply;
    DBusMessageIter args;
    char* iface;

    reply = dbus_message_new_method_return(msg);

    if(!dbus_message_iter_init(msg, &args))
    {
        print_error("ERROR", __FILE__,__func__, "no argument");
        return reply;
    }
    if (DBUS_TYPE_STRING != dbus_message_iter_get_arg_type(&args))
    {
        print_error("ERROR", __FILE__,__func__, "Argument is not a string");
        return reply;
    }
    else
        dbus_message_iter_get_basic(&args, &iface);

    waiting_iface_add(iface,CONF_FILE);
    return reply;
}


DBusMessage * dbus_cb_bootprocess_iface_remove(E_DBus_Object *obj __UNUSED__, DBusMessage *msg)
{
    DBusMessage *reply;
    DBusMessageIter args;
    char* iface;

    reply = dbus_message_new_method_return(msg);

    if(!dbus_message_iter_init(msg, &args))
    {
        print_error("ERROR", __FILE__,__func__, "no argument");
        return reply;
    }
    if (DBUS_TYPE_STRING != dbus_message_iter_get_arg_type(&args))
    {
        print_error("ERROR", __FILE__,__func__, "Argument is not a string");
        return reply;
    }
    else
        dbus_message_iter_get_basic(&args, &iface);

    waiting_iface_remove(iface,CONF_FILE);
    return reply;
}

DBusMessage * dbus_cb_bootprocess_iface_is(E_DBus_Object *obj __UNUSED__, DBusMessage *msg)
{
    DBusMessage *reply;
    DBusMessageIter args;
    char* iface;
    int is =  0;

    reply = dbus_message_new_method_return(msg);

    if(!dbus_message_iter_init(msg, &args))
    {
        print_error("ERROR", __FILE__,__func__, "no argument");
        return reply;
    }
    if (DBUS_TYPE_STRING != dbus_message_iter_get_arg_type(&args))
    {
        print_error("ERROR", __FILE__,__func__, "Argument is not a string");
        return reply;
    }
    else
        dbus_message_iter_get_basic(&args, &iface);


    is = waiting_iface_is_inconf(iface,CONF_FILE);
    dbus_message_iter_init_append(reply, &args);
    if (!dbus_message_iter_append_basic(&args, DBUS_TYPE_BOOLEAN, &is))
    {
        print_error("ERROR", __FILE__,__func__, "Out Of Memory");
        return reply;
    }

    return reply;
}

