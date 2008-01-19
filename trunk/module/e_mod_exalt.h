/*
 * vim:ts=8:sw=3:sts=8:noexpandtab:cino=>5n-3f0^-2{2
 */
#ifndef E_MOD_EXALT_H
#define E_MOD_EXALT_H

#include "e_mod_main.h"
#include "e_mod_gadcon.h"
#include "e_mod_config.h"
#include <libexalt_dbus.h>
#include <e.h>

#define ICONS_ETHERNET_ACTIVATE "/module_exalt/icons/ethernet.png"
#define ICONS_WIRELESS_ACTIVATE "/module_exalt/icons/wireless.png"
#define ICONS_ETHERNET_NOT_ACTIVATE "/module_exalt/icons/ethernet_not_activate.png"
#define ICONS_WIRELESS_NOT_ACTIVATE "/module_exalt/icons/wireless_not_activate.png"

#define ICONS_QUALITY_LESS_100 "/module_exalt/icons/quality_100-.png"
#define ICONS_QUALITY_LESS_25 "/module_exalt/icons/quality_25-.png"
#define ICONS_QUALITY_LESS_50 "/module_exalt/icons/quality_50-.png"
#define ICONS_QUALITY_LESS_75 "/module_exalt/icons/quality_75-.png"

void _cb_mouse_down (void *data, Evas * e, Evas_Object * obj,void *event_info);
void _popup_show(Instance *inst, char* interface, Exalt_Enum_Action action);
int _popup_timer_cb(void* data);

static void _menu_cb_post(void *data, E_Menu *m);
static E_Menu * _exalt_menu_new(Instance *inst);
static void _exalt_menu_pre_cb(void *data, E_Menu *m);

void _exalt_notify_cb(char* interface, Exalt_Enum_Action action, void* user_data);
void _exalt_menu_item_interfaces_load(exalt_dbus_conn* conn, E_Menu *m);
void _exalt_menu_item_wireless_load(exalt_dbus_conn *conn, E_Menu *mi, char* interface);
static void _exalt_interface_cb(void *data, E_Menu *m, E_Menu_Item *mi);
static void _exalt_wireless_cb(void *data, E_Menu *m, E_Menu_Item *mi);
static void _exalt_cb_menu_configure(void *data, E_Menu *m, E_Menu_Item *mi);

int str_istr (const char *cs, const char *ct);
char *str_remplace (const char *s, int start, int lenght, const char *ct);



#endif
