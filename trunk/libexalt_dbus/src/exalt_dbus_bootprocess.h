/*
 * =====================================================================================
 *
 *       Filename:  exalt_dbus_bootprocess.h
 *
 *    Description:  manage the boot process, during the boot process the system can wait a card.
 *
 *        Version:  1.0
 *        Created:  01/04/2008 01:02:05 PM CET
 *       Revision:  none
 *       Compiler:  gcc
 *
 *         Author:  (Watchwolf), Atton Jonathan <watchwolf@watchwolf.fr>
 *        Company:
 *
 * =====================================================================================
 */

#ifndef  EXALT_DBUS_BOOTPROCESS_INC
#define  EXALT_DBUS_BOOTPROCESS_INC

#include "libexalt_dbus.h"

void exalt_dbus_bootprocess_iface_add(const exalt_dbus_conn* conn, const char* interface);
void exalt_dbus_bootprocess_iface_remove(const exalt_dbus_conn* conn, const char* interface);
int exalt_dbus_bootprocess_iface_is(const exalt_dbus_conn* conn, const char* interface);

#endif   /* ----- #ifndef EXALT_DBUS_BOOTPROCESS_INC  ----- */

