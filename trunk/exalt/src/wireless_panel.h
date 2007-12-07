#ifndef WIRELESSPANEL_H
#define WIRELESSPANEL_H

#include <Etk.h>
#include <stdio.h>
#include <pthread.h>
#include <libexalt.h>

typedef struct _wireless_panel wireless_panel;

#include "exalt.h"

#define WIRELESS_ENCRYPTION_TEXT_NONE _("none")
#define WIRELESS_ENCRYPTION_TEXT_WEP_ASCII _("WEP (ASCII)")
#define WIRELESS_ENCRYPTION_TEXT_WEP_HEXA _("WEP (10 or 26 hexadecimals characters)")
#define WIRELESS_ENCRYPTION_TEXT_WPA_PSK_CCMP_ASCII _("WPA Personal (PSK-CCMP)")
#define WIRELESS_ENCRYPTION_TEXT_WPA_PSK_TKIP_ASCII ("WPA Personal (PSK-TKIP)")
#define WIRELESS_ENCRYPTION_TEXT_WPA2_PSK_CCMP_ASCII _("WPA2 Personal (PSK-CCMP)")
#define WIRELESS_ENCRYPTION_TEXT_WPA2_PSK_TKIP_ASCII ("WPA2 Personal (PSK-TKIP)")

#define WIRELESS_SECURITY_TEXT_OPEN _("Open")
#define WIRELESS_SECURITY_TEXT_SHARED _("Shared")

#define WIRELESS_MODE_TEXT_AUTO _("Auto")
#define WIRELESS_MODE_TEXT_ADHOC _("Ad-Hoc")
#define WIRELESS_MODE_TEXT_MANAGED _("Managed")

struct _wireless_panel
{
	main_window* win;
	char* interface;

	Etk_Widget *frame;

	//box current configuraton (display essid / ip / mask & gateway)
	Etk_Widget* lbl_essid;
	Etk_Widget* lbl_ip;
	Etk_Widget* lbl_mask;
	Etk_Widget* lbl_gateway;

	//box (dis)activate
	Etk_Widget* box_activate;
	Etk_Widget* btn_activate;
	Etk_Widget* btn_disactivate;

	//network scan list
	Etk_Widget* scan_list;
	Etk_Tree_Col* scan_quality;
	Etk_Tree_Col* scan_encryption;
	Etk_Tree_Col* scan_essid;

	//network connection
	Etk_Widget *box_connection;
        Etk_Widget *cmbox_driver;
        Etk_Widget *lbl_driver;
        Etk_Widget *entry_conn_essid;
	Etk_Widget *entry_conn_pwd;
	Etk_Widget *entry_conn_ip;
	Etk_Widget *entry_conn_mask;
	Etk_Widget *entry_conn_gateway;
	Etk_Widget *check_static;
	Etk_Widget *check_dhcp;
	Etk_Widget *cmbox_encryption;
	Etk_Widget *cmbox_security;
        Etk_Widget *cmbox_mode;
        Etk_Widget *btn_apply;

        //pbar pannel
	Etk_Widget* hbox_pbar;
	Etk_Widget* pbar;
	Ecore_Timer* pulsebar_timer;
};

void wirelesspanel_scan_networks_cb(char* interface, Ecore_List* new_networks, Ecore_List* old_networks, void* data);

wireless_panel* wirelesspanel_create(main_window* win);
void wirelesspanel_free(wireless_panel** pnl);
void wirelesspanel_show(wireless_panel* pnl);
void wirelesspanel_hide(wireless_panel* pnl);
void wirelesspanel_set_eth(wireless_panel* pnl, char* interface);
void wirelesspanel_load_scan(wireless_panel* pnl);

void wirelesspanel_update_current_conf(wireless_panel* pnl);


Etk_Widget* wirelesspanel_pageconnection_create(wireless_panel* pnl);

void wirelesspanel_disabled_widget_activate(wireless_panel* pnl);

void wirelesspanel_scanlist_row_clicked_cb(Etk_Object *object, Etk_Tree_Row *row, Etk_Event_Mouse_Up *event, void *data);
void wirelesspanel_btn_disactivate_clicked_cb(void *data);
void wirelesspanel_btn_activate_clicked_cb(void *data);
void wirelesspanel_set_static_dhcp_clicked_cb(Etk_Object *object, void *data);
void wirelesspanel_cmboxencryption_active_item_changed_cb(Etk_Object *object, void *data);
void wirelesspanel_btn_apply_clicked_cb(void *data);
void wirelesspanel_textchanged_entry_cb(Etk_Object *object, void *data);
void wirelesspanel_cmboxdriver_active_item_changed_cb(Etk_Object *object __UNUSED__, void *data);


int wirelesspanel_apply_pulsebar_timer(void* data);
void wirelesspanel_conn_apply_done(wireless_panel* pnl);

Etk_Combobox_Item * exalt_etk_combobox_data_item_get (Etk_Combobox *combobox, void *data);

void wirelesspanel_update_advanced_mode(wireless_panel *pnl);

#endif

