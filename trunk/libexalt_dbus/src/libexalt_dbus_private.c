/*
 * =====================================================================================
 *
 *       Filename:  libexalt_dbus_private.c
 *
 *    Description:  private functions/types
 *
 *        Version:  1.0
 *        Created:  09/03/2007 09:35:27 PM CEST
 *       Revision:  none
 *       Compiler:  gcc
 *
 *         Author:   (Watchwolf), Atton Jonathan <watchwolf@watchwolf.fr>
 *        Company:
 *
 * =====================================================================================
 */

#include "libexalt_dbus_private.h"


char* exalt_dbus_response_string(DBusMessage *msg)
{
    DBusMessageIter args;
    char* res;

    if(!dbus_message_iter_init(msg, &args))
    {
        exalt_dbus_print_error("WARNING", __FILE__, __LINE__,__func__, "no argument");
        return NULL;
    }
    if (DBUS_TYPE_STRING != dbus_message_iter_get_arg_type(&args))
    {
        exalt_dbus_print_error("ERROR", __FILE__, __LINE__,__func__, "the argument is not a string");
        return NULL;
    }
    else
    {
        dbus_message_iter_get_basic(&args, &res);
        return strdup(res);
    }
}

Ecore_List* exalt_dbus_response_strings(DBusMessage *msg)
{
    DBusMessageIter args;
    Ecore_List* res;
    char* val;

    res = ecore_list_new();
    res ->free_func = ECORE_FREE_CB(exalt_dbus_string_free);

    if(!dbus_message_iter_init(msg, &args))
    {
        exalt_dbus_print_error("WARNING", __FILE__, __LINE__,__func__, "no argument");
        return res;
    }

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
    {
        exalt_dbus_print_error("WARNING", __FILE__, __LINE__,__func__, "no argument");
        return -1;
    }

    if (DBUS_TYPE_BOOLEAN != dbus_message_iter_get_arg_type(&args))
    {
        exalt_dbus_print_error("ERROR", __FILE__, __LINE__,__func__, "the argument is not a boolean");
        return -1;
    }
    else
    {
        dbus_message_iter_get_basic(&args, &res);
        return res;
    }
}

int exalt_dbus_response_integer(DBusMessage *msg)
{
    DBusMessageIter args;
    int res;

    if(!dbus_message_iter_init(msg, &args))
    {
        exalt_dbus_print_error("WARNING", __FILE__, __LINE__,__func__, "no argument");
        return -1;
    }

    if (DBUS_TYPE_INT32 != dbus_message_iter_get_arg_type(&args))
    {
        exalt_dbus_print_error("ERROR", __FILE__, __LINE__,__func__, "the argument is not an integer");
        return -1;
    }
    else
    {
        dbus_message_iter_get_basic(&args, &res);
        return res;
    }
}

void exalt_dbus_print_error(const char* type, const char* file, int line,const char* fct, const char* msg, ...)
{
    va_list ap;
    va_start(ap,msg);
    fprintf(stderr,"LIBEXALT_DBUS:%s: (%d)%s: %s\n",type,line,file,fct);
    fprintf(stderr,"\t");
    vfprintf(stderr,msg,ap);
    fprintf(stderr,"\n\n");
    va_end(ap);
}

void exalt_dbus_string_free(void* data)
{
    EXALT_DBUS_FREE((char*)data);
}


