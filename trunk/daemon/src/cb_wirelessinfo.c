/*
 * =====================================================================================
 *
 *       Filename:  cb_wirelessinfo.c
 *
 *    Description:  All cb about a wireless network
 *
 *        Version:  1.0
 *        Created:  09/01/2007 12:07:06 PM CEST
 *       Revision:  none
 *       Compiler:  gcc
 *
 *         Author:   (Watchwolf), Atton Jonathan <watchwolf@watchwolf.fr>
 *        Company:
 *
 * =====================================================================================
 */

#include "cb_wirelessinfo.h"



DBusMessage * dbus_cb_wirelessinfo_get_addr(E_DBus_Object *obj __UNUSED__, DBusMessage *msg)
{
    DBusMessage *reply;
    DBusMessageIter args;
    Exalt_Wireless_Info* wi;
    const char* addr;

    reply = dbus_message_new_method_return(msg);


    wi = dbus_get_wirelessinfo(msg);
    EXALT_ASSERT_ADV(wi!=NULL,
            dbus_args_error_append(reply,
                EXALT_DBUS_WIRELESS_NETWORK_ERROR_ID,
                EXALT_DBUS_WIRELESS_NETWORK_ERROR);
            return reply,
            "wi!=NULL failed");


    addr = exalt_wirelessinfo_get_addr(wi);
    EXALT_ASSERT_ADV(addr!=NULL,
            dbus_args_error_append(reply,
                EXALT_DBUS_WIRELESS_NETWORK_ADDR_ERROR_ID,
                EXALT_DBUS_WIRELESS_NETWORK_ADDR_ERROR);
            return reply,
            "addr!=NULL failed");


    dbus_args_valid_append(reply);
    dbus_message_iter_init_append(reply, &args);
    EXALT_ASSERT_ADV(dbus_message_iter_append_basic(&args, DBUS_TYPE_STRING, &addr),
            return reply,
            "dbus_message_iter_append_basic(&args, DBUS_TYPE_STRING, &addr) failed");

    return reply;
}

DBusMessage * dbus_cb_wirelessinfo_get_protocol(E_DBus_Object *obj __UNUSED__, DBusMessage *msg)
{
    DBusMessage *reply;
    DBusMessageIter args;
    Exalt_Wireless_Info* wi;
    const char* protocol;

    reply = dbus_message_new_method_return(msg);


    wi = dbus_get_wirelessinfo(msg);
    EXALT_ASSERT_ADV(wi!=NULL,
            dbus_args_error_append(reply,
                EXALT_DBUS_WIRELESS_NETWORK_ERROR_ID,
                EXALT_DBUS_WIRELESS_NETWORK_ERROR);
            return reply,
            "wi!=NULL failed");


    protocol = exalt_wirelessinfo_get_protocol(wi);
    EXALT_ASSERT_ADV(protocol!=NULL,
            dbus_args_error_append(reply,
                EXALT_DBUS_WIRELESS_NETWORK_PROTOCOL_ERROR_ID,
                EXALT_DBUS_WIRELESS_NETWORK_PROTOCOL_ERROR);
            return reply,
            "protocol!=NULL failed");


    dbus_args_valid_append(reply);
    dbus_message_iter_init_append(reply, &args);
    EXALT_ASSERT_ADV(dbus_message_iter_append_basic(&args, DBUS_TYPE_STRING, &protocol),
            return reply,
            "dbus_message_iter_append_basic(&args, DBUS_TYPE_STRING, &protocol) failed");

    return reply;
}

DBusMessage * dbus_cb_wirelessinfo_get_mode(E_DBus_Object *obj __UNUSED__, DBusMessage *msg)
{
    DBusMessage *reply;
    DBusMessageIter args;
    Exalt_Wireless_Info* wi;
    const char* mode;

    reply = dbus_message_new_method_return(msg);


    wi = dbus_get_wirelessinfo(msg);
    EXALT_ASSERT_ADV(wi!=NULL,
            dbus_args_error_append(reply,
                EXALT_DBUS_WIRELESS_NETWORK_ERROR_ID,
                EXALT_DBUS_WIRELESS_NETWORK_ERROR);
            return reply,
            "wi!=NULL failed");


    mode = exalt_wirelessinfo_get_mode(wi);
    EXALT_ASSERT_ADV(mode!=NULL,
            dbus_args_error_append(reply,
                EXALT_DBUS_WIRELESS_NETWORK_MODE_ERROR_ID,
                EXALT_DBUS_WIRELESS_NETWORK_MODE_ERROR);
            return reply,
            "mode!=NULL failed");


    dbus_args_valid_append(reply);
    dbus_message_iter_init_append(reply, &args);
    EXALT_ASSERT_ADV(dbus_message_iter_append_basic(&args, DBUS_TYPE_STRING, &mode),
            return reply,
            "dbus_message_iter_append_basic(&args, DBUS_TYPE_STRING, &mode) failed");

    return reply;
}

DBusMessage * dbus_cb_wirelessinfo_get_channel(E_DBus_Object *obj __UNUSED__, DBusMessage *msg)
{
    DBusMessage *reply;
    DBusMessageIter args;
    Exalt_Wireless_Info* wi;
    const char* channel;

    reply = dbus_message_new_method_return(msg);


    wi = dbus_get_wirelessinfo(msg);
    EXALT_ASSERT_ADV(wi!=NULL,
            dbus_args_error_append(reply,
                EXALT_DBUS_WIRELESS_NETWORK_ERROR_ID,
                EXALT_DBUS_WIRELESS_NETWORK_ERROR);
            return reply,
            "wi!=NULL failed");


    channel = exalt_wirelessinfo_get_channel(wi);
    EXALT_ASSERT_ADV(channel!=NULL,
            dbus_args_error_append(reply,
                EXALT_DBUS_WIRELESS_NETWORK_CHANNEL_ERROR_ID,
                EXALT_DBUS_WIRELESS_NETWORK_CHANNEL_ERROR);
            return reply,
            "channel!=NULL failed");


    dbus_args_valid_append(reply);
    dbus_message_iter_init_append(reply, &args);
    EXALT_ASSERT_ADV(dbus_message_iter_append_basic(&args, DBUS_TYPE_STRING, &channel),
            return reply,
            "dbus_message_iter_append_basic(&args, DBUS_TYPE_STRING, &channel) failed");

    return reply;
}

DBusMessage * dbus_cb_wirelessinfo_get_bitrates(E_DBus_Object *obj __UNUSED__, DBusMessage *msg)
{
    DBusMessage *reply;
    DBusMessageIter args;
    Exalt_Wireless_Info* wi;
    const char* bitrates;

    reply = dbus_message_new_method_return(msg);


    wi = dbus_get_wirelessinfo(msg);
    EXALT_ASSERT_ADV(wi!=NULL,
            dbus_args_error_append(reply,
                EXALT_DBUS_WIRELESS_NETWORK_ERROR_ID,
                EXALT_DBUS_WIRELESS_NETWORK_ERROR);
            return reply,
            "wi!=NULL failed");


    bitrates = exalt_wirelessinfo_get_bitrates(wi);
    EXALT_ASSERT_ADV(bitrates!=NULL,
            dbus_args_error_append(reply,
                EXALT_DBUS_WIRELESS_NETWORK_BITRATES_ERROR_ID,
                EXALT_DBUS_WIRELESS_NETWORK_BITRATES_ERROR);
            return reply,
            "bitrates!=NULL failed");


    dbus_args_valid_append(reply);
    dbus_message_iter_init_append(reply, &args);
    EXALT_ASSERT_ADV(dbus_message_iter_append_basic(&args, DBUS_TYPE_STRING, &bitrates),
            return reply,
            "dbus_message_iter_append_basic(&args, DBUS_TYPE_STRING, &bitrates) failed");

    return reply;
}

DBusMessage * dbus_cb_wirelessinfo_get_encryption(E_DBus_Object *obj __UNUSED__, DBusMessage *msg)
{
    DBusMessage *reply;
    DBusMessageIter args;
    Exalt_Wireless_Info* wi;
    int encryption;

    reply = dbus_message_new_method_return(msg);


    wi = dbus_get_wirelessinfo(msg);
    EXALT_ASSERT_ADV(wi!=NULL,
            dbus_args_error_append(reply,
                EXALT_DBUS_WIRELESS_NETWORK_ERROR_ID,
                EXALT_DBUS_WIRELESS_NETWORK_ERROR);
            return reply,
            "wi!=NULL failed");

    encryption = exalt_wirelessinfo_get_encryption(wi);

    dbus_args_valid_append(reply);
    dbus_message_iter_init_append(reply, &args);
    EXALT_ASSERT_ADV(dbus_message_iter_append_basic(&args, DBUS_TYPE_INT32, &encryption),
            return reply,
            "dbus_message_iter_append_basic(&args, DBUS_TYPE_INT32, &encryption) failed");

    return reply;
}



DBusMessage * dbus_cb_wirelessinfo_get_signallvl(E_DBus_Object *obj __UNUSED__, DBusMessage *msg)
{
    DBusMessage *reply;
    DBusMessageIter args;
    Exalt_Wireless_Info* wi;
    int signallvl;

    reply = dbus_message_new_method_return(msg);


    wi = dbus_get_wirelessinfo(msg);
    EXALT_ASSERT_ADV(wi!=NULL,
            dbus_args_error_append(reply,
                EXALT_DBUS_WIRELESS_NETWORK_ERROR_ID,
                EXALT_DBUS_WIRELESS_NETWORK_ERROR);
            return reply,
            "wi!=NULL failed");

    signallvl = exalt_wirelessinfo_get_signallvl(wi);

    dbus_args_valid_append(reply);
    dbus_message_iter_init_append(reply, &args);
    EXALT_ASSERT_ADV(dbus_message_iter_append_basic(&args, DBUS_TYPE_INT32, &signallvl),
            return reply,
            "dbus_message_iter_append_basic(&args, DBUS_TYPE_INT32, &signallvl) failed");

    return reply;
}

DBusMessage * dbus_cb_wirelessinfo_get_noiselvl(E_DBus_Object *obj __UNUSED__, DBusMessage *msg)
{
    DBusMessage *reply;
    DBusMessageIter args;
    Exalt_Wireless_Info* wi;
    int noiselvl;

    reply = dbus_message_new_method_return(msg);


    wi = dbus_get_wirelessinfo(msg);
    EXALT_ASSERT_ADV(wi!=NULL,
            dbus_args_error_append(reply,
                EXALT_DBUS_WIRELESS_NETWORK_ERROR_ID,
                EXALT_DBUS_WIRELESS_NETWORK_ERROR);
            return reply,
            "wi!=NULL failed");

    noiselvl = exalt_wirelessinfo_get_noiselvl(wi);

    dbus_args_valid_append(reply);
    dbus_message_iter_init_append(reply, &args);
    EXALT_ASSERT_ADV(dbus_message_iter_append_basic(&args, DBUS_TYPE_INT32, &noiselvl),
            return reply,
            "dbus_message_iter_append_basic(&args, DBUS_TYPE_INT32, &noiselvl) failed");

    return reply;
}

DBusMessage * dbus_cb_wirelessinfo_get_quality(E_DBus_Object *obj __UNUSED__, DBusMessage *msg)
{
    DBusMessage *reply;
    DBusMessageIter args;
    Exalt_Wireless_Info* wi;
    int quality;

    reply = dbus_message_new_method_return(msg);


    wi = dbus_get_wirelessinfo(msg);
    EXALT_ASSERT_ADV(wi!=NULL,
            dbus_args_error_append(reply,
                EXALT_DBUS_WIRELESS_NETWORK_ERROR_ID,
                EXALT_DBUS_WIRELESS_NETWORK_ERROR);
            return reply,
            "wi!=NULL failed");

    quality = exalt_wirelessinfo_get_quality(wi);

    dbus_args_valid_append(reply);
    dbus_message_iter_init_append(reply, &args);
    EXALT_ASSERT_ADV(dbus_message_iter_append_basic(&args, DBUS_TYPE_INT32, &quality),
            return reply,
            "dbus_message_iter_append_basic(&args, DBUS_TYPE_INT32, &quality) failed");

    return reply;
}



DBusMessage * dbus_cb_wirelessinfo_get_default_conn(E_DBus_Object *obj __UNUSED__, DBusMessage *msg)
{
    DBusMessage *reply;
    DBusMessageIter args;
    Exalt_Wireless_Info* wi;
    Exalt_Connection *c;
    int i;
    const char *s;

    reply = dbus_message_new_method_return(msg);


    wi = dbus_get_wirelessinfo(msg);


    if(!wi || ! (c=exalt_wireless_conn_load(CONF_FILE, exalt_wirelessinfo_get_essid(wi))))
        c = exalt_conn_new();

    exalt_conn_set_wireless(c,1);

    dbus_args_valid_append(reply);
    dbus_message_iter_init_append(reply, &args);
    i=exalt_conn_get_mode(c);
    EXALT_ASSERT_ADV(dbus_message_iter_append_basic(&args, DBUS_TYPE_INT32, &i),
            return reply,
            "dbus_message_iter_append_basic(&args, DBUS_TYPE_INT32, &i) failed");
    if(!exalt_conn_is_dhcp(c))
    {
        s = exalt_conn_get_ip(c);
        if(!s)
            s="";
        EXALT_ASSERT_ADV(dbus_message_iter_append_basic(&args, DBUS_TYPE_STRING, &s),
                return reply,
                "dbus_message_iter_append_basic(&args, DBUS_TYPE_STRING, &s) failed");
        s = exalt_conn_get_netmask(c);
        if(!s)
            s="";
        EXALT_ASSERT_ADV(dbus_message_iter_append_basic(&args, DBUS_TYPE_STRING, &s),
                return reply,
                "dbus_message_iter_append_basic(&args, DBUS_TYPE_STRING, &s) failed");
        s = exalt_conn_get_gateway(c);
        if(!s)
            s="";
        EXALT_ASSERT_ADV(dbus_message_iter_append_basic(&args, DBUS_TYPE_STRING, &s),
                return reply,
                "dbus_message_iter_append_basic(&args, DBUS_TYPE_STRING, &s) failed");
    }

    i=exalt_conn_is_wireless(c);
    EXALT_ASSERT_ADV(dbus_message_iter_append_basic(&args, DBUS_TYPE_INT32, &i),
            return reply,
            "dbus_message_iter_append_basic(&args, DBUS_TYPE_INT32, &i) failed");
    if(exalt_conn_is_wireless(c))
    {
        i=exalt_conn_get_encryption_mode(c);
        EXALT_ASSERT_ADV(dbus_message_iter_append_basic(&args, DBUS_TYPE_INT32, &i),
                return reply,
                "dbus_message_iter_append_basic(&args, DBUS_TYPE_INT32, &i) failed");
        if(exalt_conn_get_encryption_mode(c)!=EXALT_ENCRYPTION_NONE)
        {
            s = exalt_conn_get_key(c);
            if(!s)
                s="";
            EXALT_ASSERT_ADV(dbus_message_iter_append_basic(&args, DBUS_TYPE_STRING, &s),
                    return reply,
                    "dbus_message_iter_append_basic(&args, DBUS_TYPE_STRING, &s) failed");
        }
        i=exalt_conn_get_connection_mode(c);
        EXALT_ASSERT_ADV(dbus_message_iter_append_basic(&args, DBUS_TYPE_INT32, &i),
                return reply,
                "dbus_message_iter_append_basic(&args, DBUS_TYPE_INT32, &i) failed");
        i=exalt_conn_get_security_mode(c);
        EXALT_ASSERT_ADV(dbus_message_iter_append_basic(&args, DBUS_TYPE_INT32, &i),
                return reply,
                "dbus_message_iter_append_basic(&args, DBUS_TYPE_INT32, &i) failed");
    }
    return reply;
}



