#ifndef DNSPANEL_H
#define DNSPANEL_H


#include <Etk.h>
#include <stdio.h>

typedef struct _dns_panel dns_panel;

#include "exalt.h"


struct _dns_panel
{
	Ewl_Widget *frame;

        Ewl_Widget *dns_tree;
	Ecore_List *dns_list;

	Ewl_Widget* hbox_dns_conf;
	Ewl_Widget *entry_dns;
	Ewl_Widget* btn_dns_add;
	Ewl_Widget* btn_dns_modify;
	Ewl_Widget* btn_dns_delete;
	char* select_dns;
};

dns_panel* dnspanel_create();
void dnspanel_free(dns_panel** pnl);
void dnspanel_show(dns_panel* pnl);
void dnspanel_hide(dns_panel* pnl);

void dnspanel_load_dns_list(dns_panel* pnl);

void dnspanel_cmboxethernet_active_item_changed_cb(Etk_Object *object, void *data);
void dnspanel_btn_dns_add_clicked_cb(Ewl_Widget *w __UNUSED__,
        void *ev_data __UNUSED__,
        void *user_data);
void dnspanel_btn_dns_modify_clicked_cb(Ewl_Widget *w __UNUSED__,
        void *ev_data __UNUSED__,
        void *user_data);
void dnspanel_btn_dns_delete_clicked_cb(Ewl_Widget *w __UNUSED__,
        void *ev_data __UNUSED__,
        void *user_data);

void dnspanel_dns_tree_changed_cb(Ewl_Widget *w,
        void *ev_data __UNUSED__,
        void *user_data);

void dnspanel_dns_entry_changed_cb(Ewl_Widget *w __UNUSED__,
        void *ev_data __UNUSED__,
        void *user_data);





unsigned int dnspanel_dns_tree_data_count (void *data);
void* dnspanel_dns_tree_data_fetch (
        void        *data,
        unsigned int row,
        unsigned int column __UNUSED__);
void * dnspanel_dns_header_data_fetch(void *data __UNUSED__, unsigned int column __UNUSED__);





#endif
