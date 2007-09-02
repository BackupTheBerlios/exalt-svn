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
#include "cb_ethernet.h"
#include "cb_wireless.h"
#include "cb_wirelessinfo.h"

void print_error(const char* type, const char*file, int line, const char* fct, const char* msg, ...);

int main(int argc, char** argv);
int setup(E_DBus_Connection *conn);
exalt_ethernet* dbus_get_eth(DBusMessage* msg);
exalt_wireless_info* dbus_get_wirelessinfo(DBusMessage* msg);
exalt_wireless_info* get_wirelessinfo(exalt_ethernet* eth, char* essid);
void eth_cb(exalt_ethernet* eth, int action, void* data);


#endif   /* ----- #ifndef DAEMON_INC  ----- */


