#include "dns_panel.h"


dns_panel* dnspanel_create()
{
	Ewl_Widget *vbox,*scroll;
	dns_panel* pnl;
        Ewl_Model *model;
        Ewl_View *view;
	pnl=(dns_panel*)malloc((unsigned int)sizeof(dns_panel));

	vbox = ewl_vbox_new();
        pnl->frame = vbox;
        pnl->dns_list = NULL;
	pnl->select_dns = NULL;
	//#######################
	//## DNS CONFIGURATION ##
	//######################

	//## dns list
        scroll = ewl_scrollpane_new();
        ewl_container_child_append(EWL_CONTAINER(vbox), scroll);

        ewl_object_custom_h_set(EWL_OBJECT(scroll),100);
        ewl_widget_show(scroll);

        pnl->dns_tree = ewl_tree_new();
        ewl_container_child_append(EWL_CONTAINER(scroll), pnl->dns_tree);
        ewl_object_fill_policy_set(EWL_OBJECT(pnl->dns_tree), EWL_FLAG_FILL_FILL);
        ewl_tree_column_count_set (EWL_TREE (pnl->dns_tree), 1);
        ewl_widget_show(pnl->dns_tree);
        ewl_callback_append(pnl->dns_tree, EWL_CALLBACK_VALUE_CHANGED,
                dnspanel_dns_tree_changed_cb, pnl);
        /* model */
        model = ewl_model_new();
        ewl_model_data_fetch_set (model,EWL_MODEL_DATA_FETCH (dnspanel_dns_tree_data_fetch));
        ewl_model_data_count_set (model,EWL_MODEL_DATA_COUNT (dnspanel_dns_tree_data_count));
        ewl_model_data_header_fetch_set(model, dnspanel_dns_header_data_fetch);

        /* view */
        view = ewl_label_view_get();
        /* MVC */
        ewl_mvc_selection_mode_set (EWL_MVC (pnl->dns_tree), EWL_SELECTION_MODE_SINGLE);
        ewl_mvc_model_set (EWL_MVC (pnl->dns_tree), model);
        ewl_mvc_view_set (EWL_MVC(pnl->dns_tree), view);



        //## dns configuration (buttons & entry)
        pnl->hbox_dns_conf = ewl_hbox_new();
        ewl_object_fill_policy_set(EWL_OBJECT(pnl->hbox_dns_conf),EWL_FLAG_FILL_HSHRINK);
        ewl_object_fill_policy_set(EWL_OBJECT(pnl->hbox_dns_conf),EWL_FLAG_FILL_HFILL);

        ewl_object_alignment_set(EWL_OBJECT(pnl->hbox_dns_conf), EWL_FLAG_ALIGN_LEFT);
        ewl_container_child_append(EWL_CONTAINER(vbox), pnl->hbox_dns_conf);

        pnl->entry_dns = ewl_entry_new();
        ewl_container_child_append(EWL_CONTAINER(pnl->hbox_dns_conf), pnl->entry_dns);

        pnl->btn_dns_add = ewl_button_new();
        ewl_button_label_set(EWL_BUTTON(pnl->btn_dns_add),_("New"));

        pnl->btn_dns_modify = ewl_button_new();
        ewl_button_label_set(EWL_BUTTON(pnl->btn_dns_modify),_("Replace"));

        pnl->btn_dns_delete = ewl_button_new();
        ewl_button_label_set(EWL_BUTTON(pnl->btn_dns_delete),_("Delete"));

        ewl_container_child_append(EWL_CONTAINER(pnl->hbox_dns_conf), pnl->btn_dns_add);
        ewl_container_child_append(EWL_CONTAINER(pnl->hbox_dns_conf), pnl->btn_dns_modify);
        ewl_container_child_append(EWL_CONTAINER(pnl->hbox_dns_conf), pnl->btn_dns_delete);

        ewl_callback_append(pnl->entry_dns, EWL_CALLBACK_KEY_UP,
                dnspanel_dns_entry_changed_cb, pnl);
        ewl_callback_append(pnl->btn_dns_modify, EWL_CALLBACK_CLICKED,
                dnspanel_btn_dns_modify_clicked_cb, pnl);
        ewl_callback_append(pnl->btn_dns_delete, EWL_CALLBACK_CLICKED,
                dnspanel_btn_dns_delete_clicked_cb, pnl);
        ewl_callback_append(pnl->btn_dns_add, EWL_CALLBACK_CLICKED,
                dnspanel_btn_dns_add_clicked_cb, pnl);


	dnspanel_load_dns_list(pnl);

        ewl_widget_show(pnl->hbox_dns_conf);
        ewl_widget_show(pnl->entry_dns);
        ewl_widget_show(pnl->btn_dns_add);
        ewl_widget_show(pnl->btn_dns_delete);
        ewl_widget_show(pnl->btn_dns_modify);
        ewl_widget_show(pnl->dns_tree);
        ewl_widget_show(pnl->frame);

        dnspanel_dns_entry_changed_cb(NULL,NULL,pnl);
	return pnl;
}

void dnspanel_load_dns_list(dns_panel* pnl)
{
        if(pnl->dns_list)
            ecore_list_destroy(pnl->dns_list);

	pnl->dns_list = exalt_dbus_dns_get_list(exalt_conn);
        ewl_mvc_data_set(EWL_MVC(pnl->dns_tree),pnl->dns_list);
        ewl_mvc_dirty_set(EWL_MVC(pnl->dns_tree), TRUE);
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
	ewl_widget_show(pnl->frame);
	dnspanel_load_dns_list(pnl);
 	dnspanel_dns_entry_changed_cb(NULL,NULL,pnl);
}

void dnspanel_hide(dns_panel* pnl)
{
	ewl_widget_hide(pnl->frame);
}

void dnspanel_btn_dns_add_clicked_cb(Ewl_Widget *w __UNUSED__,
        void *ev_data __UNUSED__,
        void *user_data)
{
	if(!user_data)
	{
		fprintf(stderr,"ethpanel_btn_dns_add_clicked_cb(), data==null !\n");
		return ;
	}
	dns_panel* pnl = (dns_panel*)user_data;
	exalt_dbus_dns_add(exalt_conn,ewl_text_text_get(EWL_TEXT(pnl->entry_dns)));
	ewl_text_text_set(EWL_TEXT(pnl->entry_dns),"");
	EXALT_FREE(pnl->select_dns);
	dnspanel_load_dns_list(pnl);
}

void dnspanel_btn_dns_modify_clicked_cb(Ewl_Widget *w __UNUSED__,
        void *ev_data __UNUSED__,
        void *user_data)
{
	if(!user_data)
	{
		fprintf(stderr,"ethpanel_btn_dns_modify_clicked_cb(), data==null !\n");
		return ;
	}
	dns_panel* pnl = (dns_panel*)user_data;
	exalt_dbus_dns_replace(exalt_conn, pnl->select_dns,ewl_text_text_get(EWL_TEXT(pnl->entry_dns)));
	ewl_text_text_set(EWL_TEXT(pnl->entry_dns),"");
	EXALT_FREE(pnl->select_dns);
	dnspanel_load_dns_list(pnl);
}

void dnspanel_btn_dns_delete_clicked_cb(Ewl_Widget *w __UNUSED__,
        void *ev_data __UNUSED__,
        void *user_data)
{
	if(!user_data)
	{
		fprintf(stderr,"ethpanel_btn_dns_delete_clicked_cb(), data==null !\n");
		return ;
	}
	dns_panel* pnl = (dns_panel*)user_data;
	if(pnl->select_dns)
	{
		exalt_dbus_dns_delete(exalt_conn,pnl->select_dns);
		ewl_text_text_set(EWL_TEXT(pnl->entry_dns),"");
		dnspanel_load_dns_list(pnl);
		EXALT_FREE(pnl->select_dns);
	}
}


void dnspanel_dns_tree_changed_cb(Ewl_Widget *w ,
        void *ev_data __UNUSED__,
        void *user_data)
{
    Ecore_List *selected;
    Ewl_Selection *sel;
    char* val;
    dns_panel *pnl = (dns_panel*)user_data;

    selected = ewl_mvc_selected_list_get(EWL_MVC(w));
    ecore_list_first_goto(selected);

    sel = ecore_list_next(selected);
    if(sel)
    {
        //sel->type == EWL_SELECTION_TYPE_INDEX
        val = sel->model->fetch(sel->data, EWL_SELECTION_IDX(sel)->row, 0);
        ewl_text_text_set(EWL_TEXT(pnl->entry_dns),val);
        EXALT_FREE(pnl->select_dns);
        pnl->select_dns = strdup(val);
    }
    else
        ewl_text_text_set(EWL_TEXT(pnl->entry_dns),"");
    dnspanel_dns_entry_changed_cb(NULL,NULL,pnl);
}


void dnspanel_dns_entry_changed_cb(Ewl_Widget *w __UNUSED__,
        void *ev_data __UNUSED__,
        void *user_data)
{
        dns_panel* pnl;

	pnl = (dns_panel*) user_data;
        if(!exalt_is_address(ewl_text_text_get(EWL_TEXT(pnl->entry_dns))))
	{
	 	ewl_widget_disable(pnl->btn_dns_add);
		ewl_widget_disable(pnl->btn_dns_modify);
	}
	else
	{
	 	ewl_widget_enable(pnl->btn_dns_add);
		ewl_widget_enable(pnl->btn_dns_modify);
	}
}






unsigned int dnspanel_dns_tree_data_count (void *data)
{
    if(!data)
        return 0;

    Ecore_List* l = (Ecore_List*)data;
    return ecore_list_count(l);
}

void* dnspanel_dns_tree_data_fetch (
        void        *data,
        unsigned int row,
        unsigned int column __UNUSED__)
{
    if(!data)
        return 0;

    Ecore_List* l = (Ecore_List*)data;
    ecore_list_index_goto(l,row);
    char * elt =  ecore_list_current(l);
    return elt;
}

void * dnspanel_dns_header_data_fetch(void *data __UNUSED__, unsigned int column __UNUSED__)
{
    return "DNS list";
}
