#ifndef ETHPANEL_H
#define ETHPANEL_H


#include <Etk.h>
#include <stdio.h>
#include <sys/types.h>
#include <sys/wait.h>
#include <libexalt_dbus.h>


typedef struct _eth_panel eth_panel;

#include "exalt.h"

struct _eth_panel
{
    char* interface;

    main_window* win;
    Ewl_Widget *frame;

    Ewl_Widget* box_configuration;

    //configuration panel
    Ewl_Widget *entry_ip;
    Ewl_Widget *entry_mask;
    Ewl_Widget *entry_gateway;
    Ewl_Widget *entry_cmd;
    Ewl_Widget *lbl_cmd;

    Ewl_Widget *check_static;
    Ewl_Widget *check_dhcp;

    Ewl_Widget *btn_apply;

    int apply;

    //pbar pannel
    Ewl_Widget* hbox_pbar;
    Ewl_Widget* pbar;

    //box (dis)activate
    Ewl_Widget* box_activate;
    Ewl_Widget* btn_activate;
    Ewl_Widget* btn_deactivate;
};

eth_panel* ethpanel_create(main_window* win);
void ethpanel_update_advanced_mode(eth_panel *pnl);



void ethpanel_show(eth_panel* pnl);
void ethpanel_free(eth_panel** pnl);
void ethpanel_hide(eth_panel* pnl);
void ethpanel_set_eth(eth_panel* pnl, char* interface);
void ethpanel_disabled_set(eth_panel* pnl);

void ethpanel_set_static_dhcp_clicked_cb(Ewl_Widget *w __UNUSED__,
        void *ev_data __UNUSED__,
        void *data);
void ethpanel_textchanged_entry_cb(Ewl_Widget *w __UNUSED__,
        void *ev_data __UNUSED__,
        void *data);
void ethpanel_btn_apply_clicked_cb(Ewl_Widget *w __UNUSED__,
        void *ev_data __UNUSED__,
        void *data);
void ethpanel_conn_apply_done(eth_panel* pnl);
void ethpanel_btn_deactivate_clicked_cb(Ewl_Widget *w __UNUSED__,
        void *ev_data __UNUSED__,
        void *user_data);
void ethpanel_btn_activate_clicked_cb(Ewl_Widget *w __UNUSED__,
        void *ev_data __UNUSED__,
        void *user_data);


#endif
