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



DBusMessage * dbus_cb_wirelessinfo_get_quality(E_DBus_Object *obj __UNUSED__, DBusMessage *msg)
{
    DBusMessage *reply;
    DBusMessageIter args;
    Exalt_Wireless_Info* wi;
    int quality;

    reply = dbus_message_new_method_return(msg);

    //search the interface
    wi = dbus_get_wirelessinfo(msg);
    if(!wi)
        return reply;

    dbus_message_iter_init_append(reply, &args);
    quality = exalt_wirelessinfo_get_quality(wi);
    if (!dbus_message_iter_append_basic(&args, DBUS_TYPE_INT32, &quality))
    {
        print_error("WARNING", __FILE__,__func__, "Out Of Memory");
        return reply;
    }

    return reply;
}

DBusMessage * dbus_cb_wirelessinfo_get_addr(E_DBus_Object *obj __UNUSED__, DBusMessage *msg)
{
    DBusMessage *reply;
    DBusMessageIter args;
    Exalt_Wireless_Info* wi;
    const char* addr;

    reply = dbus_message_new_method_return(msg);

    //search the interface
    wi = dbus_get_wirelessinfo(msg);
    if(!wi)
        return reply;

    dbus_message_iter_init_append(reply, &args);
    addr = exalt_wirelessinfo_get_addr(wi);
   if(!addr)
    {
        print_error("WARNING", __FILE__,__func__, "addr=%p",addr);
        return reply;
    }
    if (!dbus_message_iter_append_basic(&args, DBUS_TYPE_STRING, &addr))
    {
        print_error("ERROR", __FILE__,__func__,"Out Of Memory!");
        return reply;
    }

    return reply;
}

DBusMessage * dbus_cb_wirelessinfo_get_protocol(E_DBus_Object *obj __UNUSED__, DBusMessage *msg)
{
    DBusMessage *reply;
    DBusMessageIter args;
    Exalt_Wireless_Info* wi;
    const char* protocol;

    reply = dbus_message_new_method_return(msg);

    //search the interface
    wi = dbus_get_wirelessinfo(msg);
    if(!wi)
        return reply;

    dbus_message_iter_init_append(reply, &args);
    protocol = exalt_wirelessinfo_get_protocol(wi);

  if(!protocol)
    {
        print_error("WARNING", __FILE__,__func__, "protocol=%p",protocol);
        return reply;
    }
    if (!dbus_message_iter_append_basic(&args, DBUS_TYPE_STRING, &protocol))
    {
        print_error("ERROR", __FILE__,__func__,"Out Of Memory!");
        return reply;
    }

    return reply;
}


DBusMessage * dbus_cb_wirelessinfo_get_mode(E_DBus_Object *obj __UNUSED__, DBusMessage *msg)
{
    DBusMessage *reply;
    DBusMessageIter args;
    Exalt_Wireless_Info* wi;
    const char* mode;

    reply = dbus_message_new_method_return(msg);

    //search the interface
    wi = dbus_get_wirelessinfo(msg);
    if(!wi)
        return reply;

    dbus_message_iter_init_append(reply, &args);
    mode = exalt_wirelessinfo_get_mode(wi);

      if(!mode)
    {
        print_error("WARNING", __FILE__,__func__, "mode=%p",mode);
        return reply;
    }
    if (!dbus_message_iter_append_basic(&args, DBUS_TYPE_STRING, &mode))
    {
        print_error("ERROR", __FILE__,__func__,"Out Of Memory!");
        return reply;
    }

    return reply;
}

DBusMessage * dbus_cb_wirelessinfo_get_channel(E_DBus_Object *obj __UNUSED__, DBusMessage *msg)
{
    DBusMessage *reply;
    DBusMessageIter args;
    Exalt_Wireless_Info* wi;
    const char* channel;

    reply = dbus_message_new_method_return(msg);

    //search the interface
    wi = dbus_get_wirelessinfo(msg);
    if(!wi)
        return reply;

    dbus_message_iter_init_append(reply, &args);
    channel = exalt_wirelessinfo_get_channel(wi);

    if(!channel)
    {
        print_error("WARNING", __FILE__,__func__, "channel=%p",channel);
        return reply;
    }
    if (!dbus_message_iter_append_basic(&args, DBUS_TYPE_STRING, &channel))
    {
        print_error("ERROR", __FILE__,__func__,"Out Of Memory!");
        return reply;
    }

    return reply;
}

DBusMessage * dbus_cb_wirelessinfo_get_bitrates(E_DBus_Object *obj __UNUSED__, DBusMessage *msg)
{
    DBusMessage *reply;
    DBusMessageIter args;
    Exalt_Wireless_Info* wi;
    const char* bitrates;

    reply = dbus_message_new_method_return(msg);

    //search the interface
    wi = dbus_get_wirelessinfo(msg);
    if(!wi)
        return reply;

    dbus_message_iter_init_append(reply, &args);
    bitrates = exalt_wirelessinfo_get_bitrates(wi);

    if(!bitrates)
    {
        print_error("WARNING", __FILE__,__func__, "bitrates=%p",bitrates);
        return reply;
    }
    if (!dbus_message_iter_append_basic(&args, DBUS_TYPE_STRING, &bitrates))
    {
        print_error("ERROR", __FILE__,__func__,"Out Of Memory!");
        return reply;
    }

    return reply;
}


DBusMessage * dbus_cb_wirelessinfo_get_encryption(E_DBus_Object *obj __UNUSED__, DBusMessage *msg)
{
    DBusMessage *reply;
    DBusMessageIter args;
    Exalt_Wireless_Info* wi;
    int encryption;

    reply = dbus_message_new_method_return(msg);

    //search the interface
    wi = dbus_get_wirelessinfo(msg);
    if(!wi)
        return reply;

    dbus_message_iter_init_append(reply, &args);
    encryption = exalt_wirelessinfo_get_encryption(wi);
    if (!dbus_message_iter_append_basic(&args, DBUS_TYPE_INT32, &encryption))
    {
        print_error("ERROR", __FILE__,__func__, "Out Of Memory");
        return reply;
    }

    return reply;
}

DBusMessage * dbus_cb_wirelessinfo_get_signallvl(E_DBus_Object *obj __UNUSED__, DBusMessage *msg)
{
    DBusMessage *reply;
    DBusMessageIter args;
    Exalt_Wireless_Info* wi;
    int signallvl;

    reply = dbus_message_new_method_return(msg);

    //search the interface
    wi = dbus_get_wirelessinfo(msg);
    if(!wi)
        return reply;

    dbus_message_iter_init_append(reply, &args);
    signallvl = exalt_wirelessinfo_get_signallvl(wi);
    if (!dbus_message_iter_append_basic(&args, DBUS_TYPE_INT32, &signallvl))
    {
        print_error("ERROR", __FILE__,__func__, "Out Of Memory");
        return reply;
    }

    return reply;
}

DBusMessage * dbus_cb_wirelessinfo_get_noiselvl(E_DBus_Object *obj __UNUSED__, DBusMessage *msg)
{
    DBusMessage *reply;
    DBusMessageIter args;
    Exalt_Wireless_Info* wi;
    int noiselvl;

    reply = dbus_message_new_method_return(msg);

    //search the interface
    wi = dbus_get_wirelessinfo(msg);
    if(!wi)
        return reply;

    dbus_message_iter_init_append(reply, &args);
    noiselvl = exalt_wirelessinfo_get_noiselvl(wi);
    if (!dbus_message_iter_append_basic(&args, DBUS_TYPE_INT32, &noiselvl))
    {
        print_error("ERROR", __FILE__,__func__, "Out Of Memory");
        return reply;
    }

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

    //search the interface
    wi = dbus_get_wirelessinfo(msg);

    //try to load the configuration
    if(!wi || ! (c=exalt_wireless_conn_load(CONF_FILE, exalt_wirelessinfo_get_essid(wi))))
        c = exalt_conn_new();
    exalt_conn_set_wireless(c,1);

    dbus_message_iter_init_append(reply, &args);
    i=exalt_conn_get_mode(c);
    dbus_message_iter_append_basic(&args, DBUS_TYPE_INT32, &i);
    if(!exalt_conn_is_dhcp(c))
    {
        s = exalt_conn_get_ip(c);
        if(!s)
            s="";
        dbus_message_iter_append_basic(&args, DBUS_TYPE_STRING, &s);
        s = exalt_conn_get_netmask(c);
        if(!s)
            s="";
        dbus_message_iter_append_basic(&args, DBUS_TYPE_STRING, &s);
        s = exalt_conn_get_gateway(c);
        if(!s)
            s="";
        dbus_message_iter_append_basic(&args, DBUS_TYPE_STRING, &s);
    }

    i=exalt_conn_is_wireless(c);
    dbus_message_iter_append_basic(&args, DBUS_TYPE_INT32, &i);
    if(exalt_conn_is_wireless(c))
    {
        i=exalt_conn_get_encryption_mode(c);
        dbus_message_iter_append_basic(&args, DBUS_TYPE_INT32, &i);
        if(exalt_conn_get_encryption_mode(c)!=EXALT_ENCRYPTION_NONE)
        {
            s = exalt_conn_get_key(c);
            if(!s)
                s="";
            dbus_message_iter_append_basic(&args, DBUS_TYPE_STRING, &s);
        }
        i=exalt_conn_get_connection_mode(c);
        dbus_message_iter_append_basic(&args, DBUS_TYPE_INT32, &i);
        i=exalt_conn_get_security_mode(c);
        dbus_message_iter_append_basic(&args, DBUS_TYPE_INT32, &i);
    }
    return reply;
}

