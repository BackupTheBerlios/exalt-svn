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

Ecore_List* waiting_card_list;
Ecore_Timer* waiting_card_timer;

void print_error(const char* type, const char*file, int line, const char* fct, const char* msg, ...);

int main(int argc, char** argv);
int setup(E_DBus_Connection *conn);
Exalt_Ethernet* dbus_get_eth(DBusMessage* msg);
Exalt_Wireless_Info* dbus_get_wirelessinfo(DBusMessage* msg);
Exalt_Wireless_Info* get_wirelessinfo(Exalt_Ethernet* eth, char* essid);
void eth_cb(Exalt_Ethernet* eth, Exalt_Enum_Action action, void* data);
void wireless_scan_cb(Exalt_Ethernet* eth, Ecore_List* new_networks, Ecore_List* old_networks, void* data);

void waiting_card_load(const char* file);
int waiting_card_is(const Exalt_Ethernet* eth);
void waiting_card_remove(const Exalt_Ethernet* eth);
int waiting_card_is_done();
int waiting_card_stop(void* data);


#endif   /* ----- #ifndef DAEMON_INC  ----- */


