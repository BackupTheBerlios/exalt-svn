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



DBusMessage * dbus_cb_wirelessinfo_get_quality(E_DBus_Object *obj, DBusMessage *msg)
{
    DBusMessage *reply;
    DBusMessageIter args;
    exalt_wireless_info* wi;
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
        print_error("WARNING", __FILE__, __LINE__,__func__, "Out Of Memory");
        return reply;
    }

    return reply;
}

DBusMessage * dbus_cb_wirelessinfo_get_addr(E_DBus_Object *obj, DBusMessage *msg)
{
    DBusMessage *reply;
    DBusMessageIter args;
    exalt_wireless_info* wi;
    char* addr;

    reply = dbus_message_new_method_return(msg);

    //search the interface
    wi = dbus_get_wirelessinfo(msg);
    if(!wi)
        return reply;

    dbus_message_iter_init_append(reply, &args);
    addr = exalt_wirelessinfo_get_addr(wi);
   if(!addr)
    {
        print_error("WARNING", __FILE__, __LINE__,__func__, "addr=%p",addr);
        return reply;
    }
    if (!dbus_message_iter_append_basic(&args, DBUS_TYPE_STRING, &addr))
    {
        print_error("ERROR", __FILE__,__LINE__,__func__,"Out Of Memory!");
        return reply;
    }

    return reply;
}

DBusMessage * dbus_cb_wirelessinfo_get_protocol(E_DBus_Object *obj, DBusMessage *msg)
{
    DBusMessage *reply;
    DBusMessageIter args;
    exalt_wireless_info* wi;
    char* protocol;

    reply = dbus_message_new_method_return(msg);

    //search the interface
    wi = dbus_get_wirelessinfo(msg);
    if(!wi)
        return reply;

    dbus_message_iter_init_append(reply, &args);
    protocol = exalt_wirelessinfo_get_protocol(wi);

  if(!protocol)
    {
        print_error("WARNING", __FILE__, __LINE__,__func__, "protocol=%p",protocol);
        return reply;
    }
    if (!dbus_message_iter_append_basic(&args, DBUS_TYPE_STRING, &protocol))
    {
        print_error("ERROR", __FILE__,__LINE__,__func__,"Out Of Memory!");
        return reply;
    }

    return reply;
}


DBusMessage * dbus_cb_wirelessinfo_get_mode(E_DBus_Object *obj, DBusMessage *msg)
{
    DBusMessage *reply;
    DBusMessageIter args;
    exalt_wireless_info* wi;
    char* mode;

    reply = dbus_message_new_method_return(msg);

    //search the interface
    wi = dbus_get_wirelessinfo(msg);
    if(!wi)
        return reply;

    dbus_message_iter_init_append(reply, &args);
    mode = exalt_wirelessinfo_get_mode(wi);

      if(!mode)
    {
        print_error("WARNING", __FILE__, __LINE__,__func__, "mode=%p",mode);
        return reply;
    }
    if (!dbus_message_iter_append_basic(&args, DBUS_TYPE_STRING, &mode))
    {
        print_error("ERROR", __FILE__,__LINE__,__func__,"Out Of Memory!");
        return reply;
    }

    return reply;
}

DBusMessage * dbus_cb_wirelessinfo_get_channel(E_DBus_Object *obj, DBusMessage *msg)
{
    DBusMessage *reply;
    DBusMessageIter args;
    exalt_wireless_info* wi;
    char* channel;

    reply = dbus_message_new_method_return(msg);

    //search the interface
    wi = dbus_get_wirelessinfo(msg);
    if(!wi)
        return reply;

    dbus_message_iter_init_append(reply, &args);
    channel = exalt_wirelessinfo_get_channel(wi);

    if(!channel)
    {
        print_error("WARNING", __FILE__, __LINE__,__func__, "channel=%p",channel);
        return reply;
    }
    if (!dbus_message_iter_append_basic(&args, DBUS_TYPE_STRING, &channel))
    {
        print_error("ERROR", __FILE__,__LINE__,__func__,"Out Of Memory!");
        return reply;
    }

    return reply;
}

DBusMessage * dbus_cb_wirelessinfo_get_bitrates(E_DBus_Object *obj, DBusMessage *msg)
{
    DBusMessage *reply;
    DBusMessageIter args;
    exalt_wireless_info* wi;
    char* bitrates;

    reply = dbus_message_new_method_return(msg);

    //search the interface
    wi = dbus_get_wirelessinfo(msg);
    if(!wi)
        return reply;

    dbus_message_iter_init_append(reply, &args);
    bitrates = exalt_wirelessinfo_get_bitrates(wi);

    if(!bitrates)
    {
        print_error("WARNING", __FILE__, __LINE__,__func__, "bitrates=%p",bitrates);
        return reply;
    }
    if (!dbus_message_iter_append_basic(&args, DBUS_TYPE_STRING, &bitrates))
    {
        print_error("ERROR", __FILE__,__LINE__,__func__,"Out Of Memory!");
        return reply;
    }

    return reply;
}


DBusMessage * dbus_cb_wirelessinfo_get_encryption(E_DBus_Object *obj, DBusMessage *msg)
{
    DBusMessage *reply;
    DBusMessageIter args;
    exalt_wireless_info* wi;
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
        print_error("ERROR", __FILE__, __LINE__,__func__, "Out Of Memory");
        return reply;
    }

    return reply;
}

DBusMessage * dbus_cb_wirelessinfo_get_signallvl(E_DBus_Object *obj, DBusMessage *msg)
{
    DBusMessage *reply;
    DBusMessageIter args;
    exalt_wireless_info* wi;
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
        print_error("ERROR", __FILE__, __LINE__,__func__, "Out Of Memory");
        return reply;
    }

    return reply;
}

DBusMessage * dbus_cb_wirelessinfo_get_noiselvl(E_DBus_Object *obj, DBusMessage *msg)
{
    DBusMessage *reply;
    DBusMessageIter args;
    exalt_wireless_info* wi;
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
        print_error("ERROR", __FILE__, __LINE__,__func__, "Out Of Memory");
        return reply;
    }

    return reply;
}



DBusMessage * dbus_cb_wirelessinfo_get_default_passwd(E_DBus_Object *obj, DBusMessage *msg)
{
    DBusMessage *reply;
    DBusMessageIter args;
    exalt_wireless_info* wi;
    char* default_passwd;

    reply = dbus_message_new_method_return(msg);

    //search the interface
    wi = dbus_get_wirelessinfo(msg);
    if(!wi)
        return reply;

    dbus_message_iter_init_append(reply, &args);
    default_passwd = exalt_wirelessinfo_get_default_passwd(wi);
    if(!default_passwd)
    {
        print_error("WARNING", __FILE__, __LINE__,__func__, "default_passwd=%p",default_passwd);
        return reply;
    }
    if (!dbus_message_iter_append_basic(&args, DBUS_TYPE_STRING, &default_passwd))
    {
        print_error("ERROR", __FILE__,__LINE__,__func__,"Out Of Memory!");
        return reply;
    }

    return reply;
}

DBusMessage * dbus_cb_wirelessinfo_get_default_ip(E_DBus_Object *obj, DBusMessage *msg)
{
    DBusMessage *reply;
    DBusMessageIter args;
    exalt_wireless_info* wi;
    char* default_ip;

    reply = dbus_message_new_method_return(msg);

    //search the interface
    wi = dbus_get_wirelessinfo(msg);
    if(!wi)
        return reply;

    dbus_message_iter_init_append(reply, &args);
    default_ip = exalt_wirelessinfo_get_default_ip(wi);
    if(!default_ip)
    {
        print_error("WARNING", __FILE__, __LINE__,__func__, "default_ip=%p",default_ip);
        return reply;
    }
    if (!dbus_message_iter_append_basic(&args, DBUS_TYPE_STRING, &default_ip))
    {
        print_error("ERROR", __FILE__,__LINE__,__func__,"Out Of Memory!");
        return reply;
    }

    return reply;
}

DBusMessage * dbus_cb_wirelessinfo_get_default_netmask(E_DBus_Object *obj, DBusMessage *msg)
{
    DBusMessage *reply;
    DBusMessageIter args;
    exalt_wireless_info* wi;
    char* default_netmask;

    reply = dbus_message_new_method_return(msg);

    //search the interface
    wi = dbus_get_wirelessinfo(msg);
    if(!wi)
        return reply;

    dbus_message_iter_init_append(reply, &args);
    default_netmask = exalt_wirelessinfo_get_default_netmask(wi);

    if(!default_netmask)
    {
        print_error("WARNING", __FILE__, __LINE__,__func__, "default_netmask=%p",default_netmask);
        return reply;
    }
    if (!dbus_message_iter_append_basic(&args, DBUS_TYPE_STRING, &default_netmask))
    {
        print_error("ERROR", __FILE__,__LINE__,__func__,"Out Of Memory!");
        return reply;
    }

    return reply;
}

DBusMessage * dbus_cb_wirelessinfo_get_default_gateway(E_DBus_Object *obj, DBusMessage *msg)
{
    DBusMessage *reply;
    DBusMessageIter args;
    exalt_wireless_info* wi;
    char* default_gateway;

    reply = dbus_message_new_method_return(msg);

    //search the interface
    wi = dbus_get_wirelessinfo(msg);
    if(!wi)
        return reply;

    dbus_message_iter_init_append(reply, &args);
    default_gateway = exalt_wirelessinfo_get_default_gateway(wi);

    if(!default_gateway)
    {
        print_error("WARNING", __FILE__, __LINE__,__func__, "default_gateway=%p",default_gateway);
        return reply;
    }
    if (!dbus_message_iter_append_basic(&args, DBUS_TYPE_STRING, &default_gateway))
    {
        print_error("ERROR", __FILE__,__LINE__,__func__,"Out Of Memory!");
        return reply;
    }

    return reply;
}

DBusMessage * dbus_cb_wirelessinfo_get_default_passwd_mode(E_DBus_Object *obj, DBusMessage *msg)
{
    DBusMessage *reply;
    DBusMessageIter args;
    exalt_wireless_info* wi;
    int passwd_mode;

    reply = dbus_message_new_method_return(msg);

    //search the interface
    wi = dbus_get_wirelessinfo(msg);
    if(!wi)
        return reply;

    dbus_message_iter_init_append(reply, &args);
    passwd_mode = exalt_wirelessinfo_get_default_passwd_mode(wi);
    if (!dbus_message_iter_append_basic(&args, DBUS_TYPE_INT32, &passwd_mode))
    {
        print_error("ERROR", __FILE__, __LINE__,__func__, "Out Of Memory");
        return reply;
    }

    return reply;
}

DBusMessage * dbus_cb_wirelessinfo_get_default_security_mode(E_DBus_Object *obj, DBusMessage *msg)
{
    DBusMessage *reply;
    DBusMessageIter args;
    exalt_wireless_info* wi;
    int security_mode;

    reply = dbus_message_new_method_return(msg);

    //search the interface
    wi = dbus_get_wirelessinfo(msg);
    if(!wi)
        return reply;

    dbus_message_iter_init_append(reply, &args);
    security_mode = exalt_wirelessinfo_get_default_security_mode(wi);
    if (!dbus_message_iter_append_basic(&args, DBUS_TYPE_INT32, &security_mode))
    {
        print_error("ERROR", __FILE__, __LINE__,__func__, "Out Of Memory");
        return reply;
    }

    return reply;
}

DBusMessage * dbus_cb_wirelessinfo_get_default_mode(E_DBus_Object *obj, DBusMessage *msg)
{
    DBusMessage *reply;
    DBusMessageIter args;
    exalt_wireless_info* wi;
    int mode;

    reply = dbus_message_new_method_return(msg);

    //search the interface
    wi = dbus_get_wirelessinfo(msg);
    if(!wi)
        return reply;

    dbus_message_iter_init_append(reply, &args);
    mode = exalt_wirelessinfo_get_default_mode(wi);
    if (!dbus_message_iter_append_basic(&args, DBUS_TYPE_INT32, &mode))
    {
        print_error("ERROR", __FILE__, __LINE__,__func__, "Out Of Memory");
        return reply;
    }

    return reply;
}

DBusMessage * dbus_cb_wirelessinfo_is_default_dhcp(E_DBus_Object *obj, DBusMessage *msg)
{
    DBusMessage *reply;
    DBusMessageIter args;
    exalt_wireless_info* wi;
    int dhcp;

    reply = dbus_message_new_method_return(msg);

    //search the interface
    wi = dbus_get_wirelessinfo(msg);
    if(!wi)
        return reply;

    dbus_message_iter_init_append(reply, &args);
    dhcp = exalt_wirelessinfo_is_default_dhcp(wi);
    if (!dbus_message_iter_append_basic(&args, DBUS_TYPE_BOOLEAN, &dhcp))
    {
        print_error("ERROR", __FILE__, __LINE__,__func__, "Out Of Memory");
        return reply;
    }

    return reply;
}

