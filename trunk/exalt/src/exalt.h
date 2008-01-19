#ifndef EXALT_H
#define EXALT_H

#include "config.h"
#include <libintl.h>
#include <locale.h>
#include <libexalt_dbus.h>
#include <libexalt.h>

//#define _(STRING)    gettext(STRING)

#define __UNUSED__ __attribute__((unused))

#define ICONS_NETWORK_CONFIG "/icons/network-config.png"
#define ICONS_ABOUT "/icons/about.png"
#define ICONS_ETHERNET_ACTIVATE "/icons/ethernet.png"
#define ICONS_WIRELESS_ACTIVATE "/icons/wireless.png"
#define ICONS_ETHERNET_NOT_ACTIVATE "/icons/ethernet_not_activate.png"
#define ICONS_WIRELESS_NOT_ACTIVATE "/icons/wireless_not_activate.png"

#define ICONS_ENCRYPTION "/icons/encryption.png"
#define ICONS_QUALITY_LESS_100 "/icons/quality_100-.png"
#define ICONS_QUALITY_LESS_25 "/icons/quality_25-.png"
#define ICONS_QUALITY_LESS_50 "/icons/quality_50-.png"
#define ICONS_QUALITY_LESS_75 "/icons/quality_75-.png"

#define APPLY_PULSE_TIMER 0.05
#define APPLY_PULSE 0.04
#define APPLY_TEXT _("Updating interface configuration ...")


#define DELETE_TIMER(a) if(a) {ecore_timer_del(a); a=NULL; }

extern char* exalt_default_interface;
extern char* exalt_default_network;
extern exalt_dbus_conn* exalt_conn;

#include "main_window.h"
#include "eth_panel.h"
#include "wireless_panel.h"
#include "general_panel.h"
#include "boot_panel.h"
#include "dns_panel.h"

#endif

