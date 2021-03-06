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

    dnss = exalt_dns_get_list();
    EXALT_ASSERT_ADV(dnss!=NULL,
            dbus_args_error_append(reply,
                EXALT_DBUS_DNS_ERROR_ID,
                EXALT_DBUS_DNS_ERROR);
            return reply,
            "dnss!=NULL failed");

    dbus_args_valid_append(reply);

    dbus_message_iter_init_append(reply, &args);

    ecore_list_first_goto(dnss);
    while( (dns=ecore_list_next(dnss)))
    {
        EXALT_ASSERT_ADV(dns!=NULL,
                dbus_args_error_append(reply,
                    EXALT_DBUS_DNS_ERROR_ID,
                    EXALT_DBUS_DNS_ERROR);
                return reply,
                "dns!=NULL failed");

        EXALT_ASSERT_ADV(dbus_message_iter_append_basic(&args, DBUS_TYPE_STRING, &dns),
                ecore_list_destroy(dnss);return reply,
                "dbus_message_iter_append_basic(&args, DBUS_TYPE_STRING, &dns) failed");
    }

    ecore_list_destroy(dnss);
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
        dbus_args_error_append(reply,
                EXALT_DBUS_NO_ARGUMENT_ID,
                EXALT_DBUS_NO_ARGUMENT);
        return reply;
    }

    if (DBUS_TYPE_STRING != dbus_message_iter_get_arg_type(&args))
    {
        dbus_args_error_append(reply,
                EXALT_DBUS_ARGUMENT_NOT_STRING_ID,
                EXALT_DBUS_ARGUMENT_NOT_STRING);
        return reply;
    }
    else
        dbus_message_iter_get_basic(&args, &dns);

    exalt_dns_add(dns);

    dbus_args_valid_append(reply);

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
        dbus_args_error_append(reply,
                EXALT_DBUS_NO_ARGUMENT_ID,
                EXALT_DBUS_NO_ARGUMENT);
        return reply;
    }

    if (DBUS_TYPE_STRING != dbus_message_iter_get_arg_type(&args))
    {
        dbus_args_error_append(reply,
                EXALT_DBUS_ARGUMENT_NOT_STRING_ID,
                EXALT_DBUS_ARGUMENT_NOT_STRING);
        return reply;
    }
    else
        dbus_message_iter_get_basic(&args, &dns);

    exalt_dns_delete(dns);

    dbus_args_valid_append(reply);

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
        dbus_args_error_append(reply,
                EXALT_DBUS_NO_ARGUMENT_ID,
                EXALT_DBUS_NO_ARGUMENT);
        return reply;
    }

    if (DBUS_TYPE_STRING != dbus_message_iter_get_arg_type(&args))
    {
        dbus_args_error_append(reply,
                EXALT_DBUS_ARGUMENT_NOT_STRING_ID,
                EXALT_DBUS_ARGUMENT_NOT_STRING);
        return reply;
    }
    else
        dbus_message_iter_get_basic(&args, &old_dns);

    dbus_message_iter_next(&args);

    if (DBUS_TYPE_STRING != dbus_message_iter_get_arg_type(&args))
    {
        dbus_args_error_append(reply,
                EXALT_DBUS_ARGUMENT_NOT_STRING_ID,
                EXALT_DBUS_ARGUMENT_NOT_STRING);
        return reply;
    }
    else
        dbus_message_iter_get_basic(&args, &new_dns);

    exalt_dns_replace(old_dns, new_dns);

    dbus_args_valid_append(reply);
    return reply;
}
