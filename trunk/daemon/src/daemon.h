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
#include "cb_dns.h"
#include "boot_process.h"
#include "cb_bootprocess.h"

/*
 * @brief The dbus connection of the daemon
 */
E_DBus_Connection* exaltd_conn;

void print_error(const char* type, const char*file,  const char* fct, const char* msg, ...);

int main(int argc, char** argv);
int setup(E_DBus_Connection *conn);
Exalt_Ethernet* dbus_get_eth(DBusMessage* msg);
Exalt_Wireless_Info* dbus_get_wirelessinfo(DBusMessage* msg);
Exalt_Wireless_Info* get_wirelessinfo(Exalt_Ethernet* eth, char* essid);
void eth_cb(Exalt_Ethernet* eth, Exalt_Enum_Action action, void* data);
void wireless_scan_cb(Exalt_Ethernet* eth, Ecore_List* new_networks, Ecore_List* old_networks, void* data);


#endif   /* ----- #ifndef DAEMON_INC  ----- */


