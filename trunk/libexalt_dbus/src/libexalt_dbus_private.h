/*
 * =====================================================================================
 *
 *       Filename:  libexalt_dbus_private.h
 *
 *    Description:  private functions/types
 *
 *        Version:  1.0
 *        Created:  09/03/2007 09:34:07 PM CEST
 *       Revision:  none
 *       Compiler:  gcc
 *
 *         Author:   (Watchwolf), Atton Jonathan <watchwolf@watchwolf.fr>
 *        Company:
 *
 * =====================================================================================
 */

#ifndef  LIBEXALT_DBUS_PRIVATE_INC
#define  LIBEXALT_DBUS_PRIVATE_INC

#include "libexalt_dbus.h"


void exalt_dbus_print_error(const char* type, const char* file, int line,const char* fct, const char* msg, ...);
char* exalt_dbus_response_string(DBusMessage *msg);
Ecore_List* exalt_dbus_response_strings(DBusMessage *msg);
void exalt_dbus_string_free(void* data);
int exalt_dbus_response_boolean(DBusMessage *msg);
int exalt_dbus_response_integer(DBusMessage *msg);

#endif   /* ----- #ifndef LIBEXALT_DBUS_PRIVATE_INC  ----- */

