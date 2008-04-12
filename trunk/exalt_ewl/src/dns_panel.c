#include "dns_panel.h"


dns_panel* dnspanel_create()
{
	Etk_Widget *vbox,*hbox,*scroll;
	dns_panel* pnl;

	pnl=(dns_panel*)malloc((unsigned int)sizeof(dns_panel));

	vbox = etk_vbox_new(ETK_FALSE, 5);
        pnl->frame = vbox;
	hbox = etk_hbox_new(ETK_FALSE, 5);
	etk_box_append(ETK_BOX(vbox), hbox, ETK_BOX_START, ETK_BOX_FILL, 0);

	//#######################
	//## DNS CONFIGURATION ##
	//######################

	//## dns list
	scroll = etk_scrolled_view_new ();
	etk_box_append(ETK_BOX(vbox), scroll, ETK_BOX_START, ETK_BOX_FILL, 0);
	etk_widget_show(scroll);

	pnl->dns_list = etk_tree_new();
	pnl->dns_col = etk_tree_col_new(ETK_TREE(pnl->dns_list), _("DNS list"), 0.0, 0.0);
	etk_tree_col_model_add(pnl->dns_col, etk_tree_model_text_new());
	etk_tree_mode_set(ETK_TREE(pnl->dns_list), ETK_TREE_MODE_LIST);
	etk_signal_connect("row-clicked", ETK_OBJECT(pnl->dns_list),ETK_CALLBACK(dnspanel_list_dns_row_clicked_cb), pnl);
	etk_tree_build(ETK_TREE(pnl->dns_list));
	etk_container_add(ETK_CONTAINER(scroll), pnl->dns_list);

	//load the dns list
	dnspanel_load_dns_list(pnl);

	//## dns configuration (buttons & entry)
	pnl->hbox_dns_conf = etk_hbox_new(ETK_FALSE,5);
	etk_box_append(ETK_BOX(vbox), pnl->hbox_dns_conf, ETK_BOX_START, ETK_BOX_FILL, 0);
	pnl->entry_dns = etk_entry_new();
	etk_box_append(ETK_BOX(pnl->hbox_dns_conf), pnl->entry_dns, ETK_BOX_START, ETK_BOX_EXPAND_FILL, 0);
	pnl->btn_dns_add = etk_button_new_from_stock(ETK_STOCK_DOCUMENT_NEW);
	pnl->btn_dns_modify = etk_button_new_from_stock(ETK_STOCK_EDIT_FIND_REPLACE);
	pnl->btn_dns_delete = etk_button_new_from_stock(ETK_STOCK_EDIT_DELETE);
	etk_box_append(ETK_BOX(pnl->hbox_dns_conf), pnl->btn_dns_add, ETK_BOX_START, ETK_BOX_EXPAND_FILL, 0);
	etk_box_append(ETK_BOX(pnl->hbox_dns_conf), pnl->btn_dns_modify, ETK_BOX_START, ETK_BOX_EXPAND_FILL, 0);
	etk_box_append(ETK_BOX(pnl->hbox_dns_conf), pnl->btn_dns_delete, ETK_BOX_START, ETK_BOX_EXPAND_FILL, 0);


	etk_signal_connect("text-changed",ETK_OBJECT(pnl->entry_dns),ETK_CALLBACK(dnspanel_textchanged_entry_cb),pnl);
	etk_signal_connect_swapped("clicked", ETK_OBJECT(pnl->btn_dns_modify),ETK_CALLBACK(dnspanel_btn_dns_modify_clicked_cb), pnl);
	etk_signal_connect_swapped("clicked", ETK_OBJECT(pnl->btn_dns_delete),ETK_CALLBACK(dnspanel_btn_dns_delete_clicked_cb), pnl);
	etk_signal_connect_swapped("clicked", ETK_OBJECT(pnl->btn_dns_add),ETK_CALLBACK(dnspanel_btn_dns_add_clicked_cb), pnl);
	pnl->select_dns = NULL;

	return pnl;
}

void dnspanel_load_dns_list(dns_panel* pnl)
{
	Ecore_List* l;
        char* dns;

	etk_tree_clear(ETK_TREE(pnl->dns_list));
	l = exalt_dbus_dns_get_list(exalt_conn);
        ecore_list_first_goto(l);
        while( (dns=ecore_list_next(l)))
		etk_tree_row_append(ETK_TREE(pnl->dns_list), NULL,
		    pnl->dns_col,dns,NULL);
                ecore_list_destroy(l);
}

void dnspanel_free(dns_panel** pnl)
{
    if(pnl && *pnl)
    {
        EXALT_FREE((*pnl)->select_dns);
        EXALT_FREE(*pnl);
    }
}

void dnspanel_show(dns_panel* pnl)
{
	etk_widget_show_all(pnl->frame);
	dnspanel_load_dns_list(pnl);
 	dnspanel_textchanged_entry_cb(NULL,pnl);
}

void dnspanel_hide(dns_panel* pnl)
{
	etk_widget_hide_all(pnl->frame);
}

void dnspanel_btn_dns_add_clicked_cb(void *data)
{
	if(!data)
	{
		fprintf(stderr,"ethpanel_btn_dns_add_clicked_cb(), data==null !\n");
		return ;
	}
	dns_panel* pnl = (dns_panel*)data;
	exalt_dbus_dns_add(exalt_conn,etk_entry_text_get(ETK_ENTRY(pnl->entry_dns)));
	etk_entry_text_set(ETK_ENTRY(pnl->entry_dns),"");
	EXALT_FREE(pnl->select_dns);
	dnspanel_load_dns_list(pnl);
}

void dnspanel_btn_dns_modify_clicked_cb(void *data)
{
	if(!data)
	{
		fprintf(stderr,"ethpanel_btn_dns_modify_clicked_cb(), data==null !\n");
		return ;
	}
	dns_panel* pnl = (dns_panel*)data;
	exalt_dbus_dns_replace(exalt_conn, pnl->select_dns,etk_entry_text_get(ETK_ENTRY(pnl->entry_dns)));
	etk_entry_text_set(ETK_ENTRY(pnl->entry_dns),"");
	EXALT_FREE(pnl->select_dns);
	dnspanel_load_dns_list(pnl);
}

void dnspanel_btn_dns_delete_clicked_cb(void *data)
{
	if(!data)
	{
		fprintf(stderr,"ethpanel_btn_dns_delete_clicked_cb(), data==null !\n");
		return ;
	}
	dns_panel* pnl = (dns_panel*)data;
	if(pnl->select_dns)
	{
		exalt_dbus_dns_delete(exalt_conn,pnl->select_dns);
		etk_entry_text_set(ETK_ENTRY(pnl->entry_dns),"");
		dnspanel_load_dns_list(pnl);
		EXALT_FREE(pnl->select_dns);
	}
}


void dnspanel_list_dns_row_clicked_cb(Etk_Object *object, Etk_Tree_Row *row, Etk_Event_Mouse_Up *event, void *data)
{
	Etk_Tree *tree;
	char *row_name;
	dns_panel *pnl;

	if (!(tree = ETK_TREE(object)) || !row || !event || !data)
		return;

	pnl = (dns_panel*)data;

	etk_tree_row_fields_get(row, etk_tree_nth_col_get(tree, 0),  &row_name, NULL);
	EXALT_FREE(pnl->select_dns);
        pnl->select_dns = strdup(row_name);

        etk_entry_text_set(ETK_ENTRY(pnl->entry_dns),row_name);
}


void dnspanel_textchanged_entry_cb(Etk_Object *object __UNUSED__, void *data)
{
 	dns_panel* pnl;
	if(!data)
	{
	 	fprintf(stderr,"dnspanel_textchanged_entry_cb(): data==null ! \n");
		return ;
	}

	pnl = (dns_panel*) data;
        if(!exalt_is_address(etk_entry_text_get(ETK_ENTRY(pnl->entry_dns))))
	{
	 	etk_widget_disabled_set(pnl->btn_dns_add,ETK_TRUE);
		etk_widget_disabled_set(pnl->btn_dns_modify,ETK_TRUE);
	}
	else
	{
	 	etk_widget_disabled_set(pnl->btn_dns_add,ETK_FALSE);
		etk_widget_disabled_set(pnl->btn_dns_modify,ETK_FALSE);
	}
}


