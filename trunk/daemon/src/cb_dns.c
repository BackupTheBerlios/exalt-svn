/*
 * =====================================================================================
 *
 *       Filename:  cb_dns.c
 *
 *    Description:  DNS functions
 *
 *        Version:  1.0
 *        Created:  09/21/2007 09:20:59 PM CEST
 *       Revision:  none
 *       Compiler:  gcc
 *
 *         Author:   (Watchwolf), Atton Jonathan <watchwolf@watchwolf.fr>
 *        Company:
 *
 * =====================================================================================
 */

#include "cb_dns.h"

DBusMessage * dbus_cb_dns_get_list(E_DBus_Object *obj __UNUSED__, DBusMessage *msg)
{
    DBusMessage *reply;
    DBusMessageIter args;
    char* dns;
    Ecore_List *dnss;

    reply = dbus_message_new_method_return(msg);

    dbus_message_iter_init_append(reply, &args);
    dnss = exalt_dns_get_list();
    if(!dnss)
    {
        print_error("WARNING", __FILE__, __LINE__,__func__, "dnss=%p",dnss);
        return reply;
    }

    ecore_list_first_goto(dnss);
    while( (dns=ecore_list_next(dnss)))
    {
        if (!dns || !dbus_message_iter_append_basic(&args, DBUS_TYPE_STRING, &dns))
        {
            print_error("ERROR", __FILE__, __LINE__,__func__, "Out Of Memory");
            return reply;
        }
    }
    return reply;
}

DBusMessage * dbus_cb_dns_add(E_DBus_Object *obj __UNUSED__, DBusMessage *msg)
{
    DBusMessage *reply;
    DBusMessageIter args;
    char* dns;

    reply = dbus_message_new_method_return(msg);

    if(!dbus_message_iter_init(msg, &args))
    {
        print_error("ERROR", __FILE__, __LINE__,__func__, "no argument");
        return reply;
    }
    if (DBUS_TYPE_STRING != dbus_message_iter_get_arg_type(&args))
    {
        print_error("ERROR", __FILE__, __LINE__,__func__, "Argument is not a string");
        return reply;
    }
    else
        dbus_message_iter_get_basic(&args, &dns);

    exalt_dns_add(dns);

    return reply;
}


DBusMessage * dbus_cb_dns_delete(E_DBus_Object *obj __UNUSED__, DBusMessage *msg)
{
    DBusMessage *reply;
    DBusMessageIter args;
    char* dns;

    reply = dbus_message_new_method_return(msg);

    if(!dbus_message_iter_init(msg, &args))
    {
        print_error("ERROR", __FILE__, __LINE__,__func__, "no argument");
        return reply;
    }
    if (DBUS_TYPE_STRING != dbus_message_iter_get_arg_type(&args))
    {
        print_error("ERROR", __FILE__, __LINE__,__func__, "Argument is not a string");
        return reply;
    }
    else
        dbus_message_iter_get_basic(&args, &dns);

    exalt_dns_delete(dns);

    return reply;
}


DBusMessage * dbus_cb_dns_replace(E_DBus_Object *obj __UNUSED__, DBusMessage *msg)
{
    DBusMessage *reply;
    DBusMessageIter args;
    char* old_dns, *new_dns;

    reply = dbus_message_new_method_return(msg);

    if(!dbus_message_iter_init(msg, &args))
    {
        print_error("ERROR", __FILE__, __LINE__,__func__, "no argument");
        return reply;
    }
    if (DBUS_TYPE_STRING != dbus_message_iter_get_arg_type(&args))
    {
        print_error("ERROR", __FILE__, __LINE__,__func__, "Argument is not a string");
        return reply;
    }
    else
        dbus_message_iter_get_basic(&args, &old_dns);

    dbus_message_iter_next(&args);

    if (DBUS_TYPE_STRING != dbus_message_iter_get_arg_type(&args))
    {
        print_error("ERROR", __FILE__, __LINE__,__func__, "Argument is not a string");
        return reply;
    }
    else
        dbus_message_iter_get_basic(&args, &new_dns);

    exalt_dns_replace(old_dns, new_dns);

    return reply;
}
