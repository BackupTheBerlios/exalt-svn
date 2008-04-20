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
#define WIRELESS_ENCRYPTION_TEXT_WPA_PSK_TKIP_ASCII _("WPA Personal (PSK-TKIP)")
#define WIRELESS_ENCRYPTION_TEXT_WPA2_PSK_CCMP_ASCII _("WPA2 Personal (PSK-CCMP)")
#define WIRELESS_ENCRYPTION_TEXT_WPA2_PSK_TKIP_ASCII _("WPA2 Personal (PSK-TKIP)")

#define WIRELESS_SECURITY_TEXT_OPEN _("Open")
#define WIRELESS_SECURITY_TEXT_SHARED _("Shared")

#define WIRELESS_MODE_TEXT_AUTO _("Auto")
#define WIRELESS_MODE_TEXT_ADHOC _("Ad-Hoc")
#define WIRELESS_MODE_TEXT_MANAGED _("Managed")

struct _wireless_panel
{
	main_window* win;
	char* interface;

	Ewl_Widget *frame;

	//box current configuraton (display essid / ip / mask & gateway)
	Ewl_Widget* lbl_essid;
	Ewl_Widget* lbl_ip;
	Ewl_Widget* lbl_mask;
	Ewl_Widget* lbl_gateway;

	//box (dis)activate
	Ewl_Widget* box_activate;
	Ewl_Widget* btn_activate;
	Ewl_Widget* btn_deactivate;

	//network scan list
	Ewl_Widget* scan_tree;
        Ecore_List * scan_list;

	//network connection
	Ewl_Widget *box_connection;
        Ewl_Widget *cmbox_driver;
        Ewl_Widget *lbl_driver;
        Ewl_Widget *entry_conn_essid;
	Ewl_Widget *entry_conn_pwd;
	Ewl_Widget *entry_conn_ip;
	Ewl_Widget *entry_conn_mask;
	Ewl_Widget *entry_conn_gateway;
	Ewl_Widget *check_static;
	Ewl_Widget *check_dhcp;
	Ewl_Widget *cmbox_encryption;
	Ewl_Widget *cmbox_security;
        Ewl_Widget *cmbox_mode;
        Ewl_Widget *btn_apply;
        Ewl_Widget *entry_conn_cmd;
        Ewl_Widget *lbl_conn_cmd;
        //pbar pannel
	Ewl_Widget* hbox_pbar;
	Ewl_Widget* pbar;
        int apply;

        Ecore_Timer* scan_timer;
};

void wirelesspanel_scan_networks_cb(char* interface, Ecore_List* networks, void* data);

wireless_panel* wirelesspanel_create(main_window* win);
void wirelesspanel_free(wireless_panel** pnl);
void wirelesspanel_show(wireless_panel* pnl);
void wirelesspanel_hide(wireless_panel* pnl);
void wirelesspanel_set_eth(wireless_panel* pnl, char* interface);

void wirelesspanel_update_current_conf(wireless_panel* pnl);


Ewl_Widget* wirelesspanel_pageconnection_create(wireless_panel* pnl);

void wirelesspanel_disabled_widget_activate(Ewl_Widget *w ,
        void *ev_data __UNUSED__,
        void *user_data);

void wirelesspanel_scanlist_row_clicked_cb(Ewl_Widget *w __UNUSED__,
        void *ev_data __UNUSED__,
        void *user_data);
void wirelesspanel_btn_deactivate_clicked_cb(Ewl_Widget *w __UNUSED__,
        void *ev_data __UNUSED__,
        void *user_data);
void wirelesspanel_btn_activate_clicked_cb(Ewl_Widget *w __UNUSED__,
        void *ev_data __UNUSED__,
        void *user_data);
void wirelesspanel_set_static_dhcp_clicked_cb(Ewl_Widget *w __UNUSED__,
        void *ev_data __UNUSED__,
        void *user_data);
void wirelesspanel_cmboxencryption_active_item_changed_cb(Ewl_Widget *w __UNUSED__,
        void *ev_data __UNUSED__,
        void *user_data);
void wirelesspanel_btn_apply_clicked_cb(Ewl_Widget *w __UNUSED__,
        void *ev_data __UNUSED__,
        void *user_data);
void wirelesspanel_textchanged_entry_cb(Ewl_Widget *w ,
        void *ev_data __UNUSED__,
        void *user_data);

void wirelesspanel_callback_append(wireless_panel *pnl);
void wirelesspanel_callback_del(wireless_panel *pnl);

int wirelesspanel_apply_pulsebar_timer(void* data);
void wirelesspanel_conn_apply_done(wireless_panel* pnl);

Etk_Combobox_Item * exalt_etk_combobox_data_item_get (Etk_Combobox *combobox, void *data);

void wirelesspanel_update_advanced_mode(wireless_panel *pnl);
int wirelesspanel_scan_timer_cb(void *data);



typedef struct
{
    int* encryption;
    char* essid;
    int* quality;
}Scan_Elt;

void scan_elt_free(void* elt);
unsigned int wirelesspanel_scan_package_tree_data_count (void *data);
void* wirelesspanel_scan_package_tree_data_fetch (
        void        *data,
        unsigned int row,
        unsigned int column);
void * wirelesspanel_scan_cb_header_data_fetch(void *data __UNUSED__, unsigned int column);
Ewl_Widget * wirelesspanel_scan_cb_widget_fetch(void *data, unsigned int row __UNUSED__,
        unsigned int column);

typedef struct
{
    char* label;
    int* value;
} Default_Elt;

void default_elt_free(void* elt);
unsigned int wirelesspanel_default_tree_data_count (void *data);
void* wirelesspanel_default_tree_data_fetch (
        void        *data,
        unsigned int row,
        unsigned int column);

Ewl_Widget *wirelesspanel_default_editable_header_fetch(void *data,
        unsigned int col __UNUSED__);



#endif

