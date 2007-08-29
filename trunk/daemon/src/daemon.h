/*
 * =====================================================================================
 *
 *       Filename:  daemon.h
 *
 *    Description:  define globals functions
 *
 *        Version:  1.0
 *        Created:  08/28/2007 04:26:56 PM CEST
 *       Revision:  none
 *       Compiler:  gcc
 *
 *         Author:   (Watchwolf), Atton Jonathan <watchwolf@watchwolf.fr>
 *        Company:
 *
 * =====================================================================================
 */

#ifndef  DAEMON_INC
#define  DAEMON_INC

#include <stdio.h>
#include <libexalt.h>

#include "define.h"
#include "cb_functions.h"

int main(int argc, char** argv);
int setup(E_DBus_Connection *conn);
exalt_ethernet* dbus_get_eth(DBusMessage* msg);

#endif   /* ----- #ifndef DAEMON_INC  ----- */

