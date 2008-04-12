#ifndef DNSPANEL_H
#define DNSPANEL_H


#include <Etk.h>
#include <stdio.h>

typedef struct _dns_panel dns_panel;

#include "exalt.h"


struct _dns_panel
{
	Etk_Widget *frame;

	Etk_Widget *dns_list;
	Etk_Tree_Col* dns_col;
	Etk_Widget* hbox_dns_conf;
	Etk_Widget *entry_dns;
	Etk_Widget* btn_dns_add;
	Etk_Widget* btn_dns_modify;
	Etk_Widget* btn_dns_delete;
	char* select_dns;
};

dns_panel* dnspanel_create();
void dnspanel_free(dns_panel** pnl);
void dnspanel_show(dns_panel* pnl);
void dnspanel_hide(dns_panel* pnl);

void dnspanel_load_dns_list(dns_panel* pnl);

void dnspanel_textchanged_entry_cb(Etk_Object *object, void *data);
void dnspanel_cmboxethernet_active_item_changed_cb(Etk_Object *object, void *data);
void dnspanel_btn_dns_add_clicked_cb(void *data);
void dnspanel_btn_dns_modify_clicked_cb(void *data);
void dnspanel_btn_dns_delete_clicked_cb(void *data);
void dnspanel_list_dns_row_clicked_cb(Etk_Object *object, Etk_Tree_Row *row, Etk_Event_Mouse_Up *event, void *data);





#endif
