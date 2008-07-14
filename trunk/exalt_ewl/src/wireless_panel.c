
#include "wireless_panel.h"



wireless_panel* wirelesspanel_create(main_window* win)
{
    Ewl_Widget *hbox,*vbox,*scroll,*label, *grid;
    wireless_panel* pnl;
    Ewl_Model* model;
    Ewl_View *view;

    pnl=(wireless_panel*)malloc((unsigned int)sizeof(wireless_panel));
    pnl->win=win;
    pnl->interface = NULL;
    pnl->scan_timer=NULL;

    pnl->frame = ewl_border_new();
    ewl_border_label_set(EWL_BORDER(pnl->frame),"wireless_frame");
    ewl_widget_show(pnl->frame);


    hbox = ewl_hbox_new();
    ewl_container_child_append(EWL_CONTAINER(pnl->frame), hbox);
    ewl_widget_show(hbox);

    //##############################
    //## btn activate/deactivate ##
    //##############################
    vbox = ewl_vbox_new();
    ewl_container_child_append(EWL_CONTAINER(hbox), vbox);
    ewl_object_fill_policy_set(EWL_OBJECT(vbox),EWL_FLAG_FILL_VFILL);
    ewl_widget_show(vbox);

    pnl->box_activate = ewl_hbox_new();
    ewl_container_child_append(EWL_CONTAINER(vbox), pnl->box_activate);
    ewl_object_fill_policy_set(EWL_OBJECT(pnl->box_activate),EWL_FLAG_FILL_SHRINK);

    pnl->btn_activate = ewl_button_new();
    ewl_button_label_set(EWL_BUTTON(pnl->btn_activate),_("Activate"));
    ewl_object_fill_policy_set(EWL_OBJECT(pnl->btn_activate), EWL_FLAG_FILL_SHRINK);

    pnl->btn_deactivate = ewl_button_new();
    ewl_button_label_set(EWL_BUTTON(pnl->btn_deactivate),_("Deactivate"));
    ewl_object_fill_policy_set(EWL_OBJECT(pnl->btn_deactivate), EWL_FLAG_FILL_SHRINK);

    ewl_container_child_append(EWL_CONTAINER(pnl->box_activate), pnl->btn_deactivate);
    ewl_container_child_append(EWL_CONTAINER(pnl->box_activate), pnl->btn_activate);
    ewl_widget_show(pnl->btn_activate);
    ewl_widget_show(pnl->btn_deactivate);
    ewl_widget_show(pnl->box_activate);
    ewl_callback_append(pnl->btn_activate, EWL_CALLBACK_CLICKED,
            wirelesspanel_btn_activate_clicked_cb, pnl);
    ewl_callback_append(pnl->btn_deactivate, EWL_CALLBACK_CLICKED,
            wirelesspanel_btn_deactivate_clicked_cb, pnl);


    //###############
    //## scan list ##
    //###############
    scroll = ewl_scrollpane_new ();
    ewl_container_child_append(EWL_CONTAINER(vbox), scroll);
    ewl_object_fill_policy_set(EWL_OBJECT(scroll),EWL_FLAG_FILL_FILL);
    ewl_widget_show(scroll);

    pnl->scan_list = ecore_list_new();
    ecore_list_free_cb_set(pnl->scan_list, scan_elt_free);

    pnl->scan_tree = ewl_tree_new();
    ewl_container_child_append(EWL_CONTAINER(scroll), pnl->scan_tree);
    ewl_object_alignment_set(EWL_OBJECT(pnl->scan_tree), EWL_FLAG_ALIGN_LEFT);
    ewl_object_fill_policy_set(EWL_OBJECT(pnl->scan_tree), EWL_FLAG_FILL_FILL);
    ewl_tree_column_count_set (EWL_TREE (pnl->scan_tree), 1);
    ewl_widget_show(pnl->scan_tree);
    ewl_callback_append(pnl->scan_tree, EWL_CALLBACK_VALUE_CHANGED,
            wirelesspanel_scanlist_row_clicked_cb, pnl);
    /* model */
    model = ewl_model_new();
    ewl_model_data_fetch_set (model,EWL_MODEL_DATA_FETCH (wirelesspanel_scan_package_tree_data_fetch));
    ewl_model_data_count_set (model,EWL_MODEL_DATA_COUNT (wirelesspanel_scan_package_tree_data_count));
    ewl_model_data_header_fetch_set(model, wirelesspanel_scan_cb_header_data_fetch);

    /* view */
    view = ewl_label_view_get();
    ewl_view_widget_fetch_set(view, wirelesspanel_scan_cb_widget_fetch);

    /* MVC */
    ewl_mvc_selection_mode_set (EWL_MVC (pnl->scan_tree), EWL_SELECTION_MODE_SINGLE);
    ewl_mvc_model_set (EWL_MVC (pnl->scan_tree), model);
    ewl_mvc_view_set (EWL_MVC(pnl->scan_tree), view);
    ewl_mvc_data_set (EWL_MVC(pnl->scan_tree), pnl->scan_list);

    //###########################
    //## current configuration ##
    //###########################
    grid = ewl_grid_new();
    ewl_container_child_append(EWL_CONTAINER(vbox), grid);
    ewl_object_fill_policy_set(EWL_OBJECT(grid), EWL_FLAG_FILL_HFILL);
    ewl_grid_dimensions_set(EWL_GRID(grid), 2, 4);
    ewl_grid_column_relative_w_set(EWL_GRID(grid), 0, 0.20);
    ewl_widget_show(grid);

    pnl->lbl_essid = ewl_label_new();
    ewl_label_text_set(EWL_LABEL(pnl->lbl_essid),_("none"));
    pnl->lbl_ip = ewl_label_new();
    ewl_label_text_set(EWL_LABEL(pnl->lbl_ip),_("none"));
    pnl->lbl_gateway = ewl_label_new();
    ewl_label_text_set(EWL_LABEL(pnl->lbl_gateway),_("none"));
    pnl->lbl_mask = ewl_label_new();
    ewl_label_text_set(EWL_LABEL(pnl->lbl_mask),_("none"));

    ewl_widget_show(pnl->lbl_essid);
    ewl_widget_show(pnl->lbl_ip);
    ewl_widget_show(pnl->lbl_gateway);
    ewl_widget_show(pnl->lbl_mask);

    label = ewl_label_new();
    ewl_label_text_set(EWL_LABEL(label),_("Current essid: "));
    ewl_container_child_append(EWL_CONTAINER(grid), label);
    ewl_container_child_append(EWL_CONTAINER(grid), pnl->lbl_essid);
    ewl_widget_show(label);

    label = ewl_label_new();
    ewl_label_text_set(EWL_LABEL(label),_("Current IP: "));
    ewl_container_child_append(EWL_CONTAINER(grid), label);
    ewl_container_child_append(EWL_CONTAINER(grid), pnl->lbl_ip);
    ewl_widget_show(label);

    label = ewl_label_new();
    ewl_label_text_set(EWL_LABEL(label),_("Current mask: "));
    ewl_container_child_append(EWL_CONTAINER(grid), label);
    ewl_container_child_append(EWL_CONTAINER(grid), pnl->lbl_mask);
    ewl_widget_show(label);

    label = ewl_label_new();
    ewl_label_text_set(EWL_LABEL(label),_("Current gateway: "));
    ewl_container_child_append(EWL_CONTAINER(grid), label);
    ewl_container_child_append(EWL_CONTAINER(grid), pnl->lbl_gateway);
    ewl_widget_show(label);

    pnl->box_connection = wirelesspanel_pageconnection_create(pnl);
    ewl_container_child_append(EWL_CONTAINER(hbox), pnl->box_connection);
    ewl_widget_show(pnl->box_connection);

    exalt_dbus_scan_notify_set(exalt_conn,wirelesspanel_scan_networks_cb,pnl);

    return pnl;
}

void wirelesspanel_free(wireless_panel** pnl)
{
    if(pnl && *pnl)
    {
        EXALT_FREE((*pnl)->interface);
        EXALT_FREE(*pnl);
    }
}

void wirelesspanel_show(wireless_panel* pnl)
{
    ecore_list_destroy(pnl->scan_list);
    pnl->scan_list = ecore_list_new();
    ecore_list_free_cb_set(pnl->scan_list,scan_elt_free );
    ewl_mvc_dirty_set(EWL_MVC(pnl->scan_tree), TRUE);

    exalt_dbus_wireless_scan_start(exalt_conn,pnl->interface);
    pnl->scan_timer = ecore_timer_add(5,wirelesspanel_scan_timer_cb,pnl);
    ewl_widget_show(pnl->frame);
    wirelesspanel_update_advanced_mode(pnl);
    ewl_widget_hide(pnl->hbox_pbar);
}

void wirelesspanel_update_advanced_mode(wireless_panel *pnl)
{
    if(!pnl->win->advanced_mode)
    {
        ewl_widget_hide(pnl->lbl_driver);
        ewl_widget_hide(pnl->cmbox_driver);
        ewl_widget_hide(pnl->entry_conn_cmd);
        ewl_widget_hide(pnl->lbl_conn_cmd);
    }
    else
    {
        ewl_widget_show(pnl->lbl_driver);
        ewl_widget_show(pnl->cmbox_driver);
        ewl_widget_show(pnl->entry_conn_cmd);
        ewl_widget_show(pnl->lbl_conn_cmd);
    }
}

void wirelesspanel_disabled_widget_activate(Ewl_Widget *w ,
        void *ev_data __UNUSED__,
        void *user_data)
{
    int* encryption=NULL;
    wireless_panel *pnl = (wireless_panel*)user_data;

    int down = !exalt_dbus_eth_is_up(exalt_conn,pnl->interface);
    int apply = pnl->apply;
    printf("disabled\n");
    if(!pnl)
    {
        fprintf(stderr,"wireless_disabled_widget_activate(): pnl==null ! \n");
        return ;
    }

    wirelesspanel_callback_del(pnl);

    if(apply)
    {
        ewl_widget_disable(pnl->btn_activate);
        ewl_widget_disable(pnl->btn_deactivate);
        ewl_widget_disable(pnl->scan_tree);
        ewl_widget_disable(pnl->entry_conn_essid);
        ewl_widget_disable(pnl->cmbox_encryption);
        ewl_widget_disable(pnl->btn_apply);
        ewl_widget_disable(pnl->entry_conn_pwd);
        ewl_widget_disable(pnl->cmbox_mode);
        ewl_widget_disable(pnl->cmbox_security);
        ewl_widget_disable(pnl->entry_conn_ip);
        ewl_widget_disable(pnl->entry_conn_mask);
        ewl_widget_disable(pnl->entry_conn_gateway);
        ewl_widget_disable(pnl->check_static);
        ewl_widget_disable(pnl->check_dhcp);
        ewl_widget_disable(pnl->cmbox_driver);
        ewl_widget_disable(pnl->entry_conn_cmd);
        ewl_widget_show(pnl->hbox_pbar);

    }
    else if(down)
    {
        ewl_widget_enable(pnl->btn_activate);
        ewl_widget_disable(pnl->btn_deactivate);
        ewl_widget_disable(pnl->scan_tree);
        ewl_widget_disable(pnl->entry_conn_essid);
        ewl_widget_disable(pnl->cmbox_encryption);
        ewl_widget_disable(pnl->btn_apply);
        ewl_widget_disable(pnl->entry_conn_pwd);
        ewl_widget_disable(pnl->cmbox_mode);
        ewl_widget_disable(pnl->cmbox_security);
        ewl_widget_disable(pnl->entry_conn_ip);
        ewl_widget_disable(pnl->entry_conn_mask);
        ewl_widget_disable(pnl->entry_conn_gateway);
        ewl_widget_disable(pnl->check_static);
        ewl_widget_disable(pnl->check_dhcp);
        ewl_widget_disable(pnl->cmbox_driver);
        ewl_widget_disable(pnl->entry_conn_cmd);
    }
    else
    {
        ewl_widget_disable(pnl->btn_activate);
        ewl_widget_enable(pnl->btn_deactivate);
        ewl_widget_enable(pnl->scan_tree);
        ewl_widget_enable(pnl->entry_conn_essid);
        ewl_widget_enable(pnl->cmbox_encryption);
        ewl_widget_enable(pnl->btn_apply);
        ewl_widget_enable(pnl->entry_conn_pwd);
        ewl_widget_enable(pnl->cmbox_mode);
        ewl_widget_enable(pnl->cmbox_security);
        ewl_widget_enable(pnl->entry_conn_ip);
        ewl_widget_enable(pnl->entry_conn_mask);
        ewl_widget_enable(pnl->entry_conn_gateway);
        ewl_widget_enable(pnl->check_static);
        ewl_widget_enable(pnl->check_dhcp);
        ewl_widget_enable(pnl->cmbox_driver);
        ewl_widget_enable(pnl->entry_conn_cmd);


        if(ewl_radiobutton_is_checked(EWL_RADIOBUTTON(pnl->check_dhcp)))
        {
            //dhcp conf
            ewl_widget_disable(pnl->entry_conn_ip);
            //ewl_widget_disable(pnl->entry_conn_mask);
            //ewl_widget_disable(pnl->entry_conn_gateway);
        }


/*
       Ewl_Selection_Idx *idx;
        idx = ewl_mvc_selected_get(EWL_MVC(pnl->cmbox_encryption));

        if (idx)
        {
            Ecore_List* l = (Ecore_List*) ewl_mvc_data_get(EWL_MVC(pnl->cmbox_encryption));
            ecore_list_index_goto(l,idx->row);
            Default_Elt* elt= ecore_list_next(l);
            encryption = elt->value;
        }

        if(!apply)
        {
            if(encryption && *encryption==EXALT_ENCRYPTION_NONE)
            {
             //   ewl_widget_disable(pnl->entry_conn_pwd);
             //   ewl_widget_disable(pnl->cmbox_security);
            }
            else if(encryption && (*encryption == EXALT_ENCRYPTION_WPA_PSK_CCMP_ASCII
                        || *encryption == EXALT_ENCRYPTION_WPA_PSK_TKIP_ASCII
                        || *encryption == EXALT_ENCRYPTION_WPA2_PSK_CCMP_ASCII
                        || *encryption == EXALT_ENCRYPTION_WPA2_PSK_TKIP_ASCII))
            {
             //   ewl_widget_disable(pnl->cmbox_mode);
             //   ewl_widget_disable(pnl->cmbox_security);
            }
        }
*/
    }
    if(!exalt_dhcp_is_support())
        ewl_widget_disable(pnl->check_dhcp);
    wirelesspanel_callback_append(pnl);
}

void wirelesspanel_hide(wireless_panel* pnl)
{
    EXALT_DELETE_TIMER(pnl->scan_timer);
    ewl_widget_hide(pnl->frame);
}

void wirelesspanel_set_eth(wireless_panel* pnl, char* interface)
{
    EXALT_ASSERT_RETURN_VOID(pnl!=NULL);
    EXALT_ASSERT_RETURN_VOID(interface!=NULL);
    pnl->apply=0;
    char name[100];
    sprintf(name,_("Wireless card: %s"),interface);
    EXALT_FREE(pnl->interface);
    pnl->interface = strdup(interface);
    ewl_border_label_set(EWL_BORDER(pnl->frame),name);

    wirelesspanel_textchanged_entry_cb(NULL,NULL,pnl);
    wirelesspanel_update_current_conf(pnl);
}

void wirelesspanel_update_current_conf(wireless_panel* pnl)
{
    char* str;
    Ecore_List* l;
    Default_Elt* elt;
    short find = 0;

    if(!pnl)
    {
        fprintf(stderr,"wirelesspanel_update_current_conf(): pnl==null! \n");
        return ;
    }
    str = exalt_dbus_wireless_get_essid(exalt_conn, pnl->interface);
    ewl_label_text_set(EWL_LABEL(pnl->lbl_essid), str);
    EXALT_FREE(str);

    str = exalt_dbus_eth_get_ip(exalt_conn, pnl->interface);
    ewl_label_text_set(EWL_LABEL(pnl->lbl_ip), str);
    EXALT_FREE(str);

    str = exalt_dbus_eth_get_netmask(exalt_conn,pnl->interface);
    ewl_label_text_set(EWL_LABEL(pnl->lbl_mask), str);
    EXALT_FREE(str);

    str = exalt_dbus_eth_get_gateway(exalt_conn, pnl->interface);
    ewl_label_text_set(EWL_LABEL(pnl->lbl_gateway), str);
    EXALT_FREE(str);

    str = exalt_dbus_wireless_get_wpasupplicant_driver(exalt_conn,pnl->interface);
    ewl_mvc_selected_clear(EWL_MVC(pnl->cmbox_driver));
    l = ewl_mvc_data_get(EWL_MVC(pnl->cmbox_driver));
    ecore_list_first_goto(l);
    find = 0;
    while( !find && (elt=ecore_list_next(l)) )
    {
        if(str && elt->label && strcmp(str, elt->label)==0)
            find =1;
    }
    ewl_mvc_selected_set(EWL_MVC(pnl->cmbox_driver),
            ewl_mvc_model_get(EWL_MVC(pnl->cmbox_driver)),
            l,
            ecore_list_index(l)-1,
            0);

    EXALT_FREE(str);
}

Ewl_Widget* wirelesspanel_pageconnection_create(wireless_panel* pnl)
{
    Ewl_Widget *grid,*label;
    Ewl_Model *model;
    Ewl_View *view;

    grid = ewl_grid_new();
    ewl_object_fill_policy_set(EWL_OBJECT(grid), EWL_FLAG_FILL_HFILL);
    ewl_grid_dimensions_set(EWL_GRID(grid), 2, 14);
    ewl_grid_column_relative_w_set(EWL_GRID(grid), 0, 0.35);
    ewl_widget_show(grid);

    Ecore_List *l_enc =ecore_list_new();
    Ecore_List *l_sec =ecore_list_new();
    Ecore_List *l_conn =ecore_list_new();

    Exalt_Enum_Encryption_Mode *en_none = malloc(sizeof(int));
    *en_none = EXALT_ENCRYPTION_NONE;
    Exalt_Enum_Encryption_Mode *en_wep_ascii = malloc(sizeof(Exalt_Enum_Encryption_Mode));
    *en_wep_ascii = EXALT_ENCRYPTION_WEP_ASCII;
    Exalt_Enum_Encryption_Mode *en_wep_hexa = malloc(sizeof(Exalt_Enum_Encryption_Mode));
    *en_wep_hexa = EXALT_ENCRYPTION_WEP_HEXA;

    Exalt_Enum_Encryption_Mode *en_wpa_psk_ccmp = malloc(sizeof(Exalt_Enum_Encryption_Mode));
    *en_wpa_psk_ccmp = EXALT_ENCRYPTION_WPA_PSK_CCMP_ASCII;
    Exalt_Enum_Encryption_Mode *en_wpa_psk_tkip = malloc(sizeof(Exalt_Enum_Encryption_Mode));
    *en_wpa_psk_tkip = EXALT_ENCRYPTION_WPA_PSK_TKIP_ASCII;

    Exalt_Enum_Encryption_Mode *en_wpa2_psk_ccmp = malloc(sizeof(Exalt_Enum_Encryption_Mode));
    *en_wpa2_psk_ccmp = EXALT_ENCRYPTION_WPA2_PSK_CCMP_ASCII;
    Exalt_Enum_Encryption_Mode *en_wpa2_psk_tkip = malloc(sizeof(Exalt_Enum_Encryption_Mode));
    *en_wpa2_psk_tkip = EXALT_ENCRYPTION_WPA2_PSK_TKIP_ASCII;

    Default_Elt* elt = malloc(sizeof(Default_Elt*));
    elt->label = strdup(WIRELESS_ENCRYPTION_TEXT_NONE);
    elt->value = (int*)en_none;
    ecore_list_append(l_enc, elt);
    elt = malloc(sizeof(Default_Elt*));
    elt->label = strdup(WIRELESS_ENCRYPTION_TEXT_WEP_ASCII);
    elt->value = (int*)en_wep_ascii;
    ecore_list_append(l_enc, elt);
    elt = malloc(sizeof(Default_Elt*));
    elt->label = strdup(WIRELESS_ENCRYPTION_TEXT_WEP_HEXA);
    elt->value = (int*)en_wep_hexa;
    ecore_list_append(l_enc, elt);
    elt = malloc(sizeof(Default_Elt*));
    elt->label = strdup(WIRELESS_ENCRYPTION_TEXT_WPA_PSK_CCMP_ASCII);
    elt->value = (int*)en_wpa_psk_ccmp;
    ecore_list_append(l_enc, elt);
    elt = malloc(sizeof(Default_Elt*));
    elt->label = strdup(WIRELESS_ENCRYPTION_TEXT_WPA_PSK_TKIP_ASCII);
    elt->value = (int*)en_wpa_psk_tkip;
    ecore_list_append(l_enc, elt);
    elt = malloc(sizeof(Default_Elt*));
    elt->label = strdup(WIRELESS_ENCRYPTION_TEXT_WPA2_PSK_CCMP_ASCII);
    elt->value = (int*)en_wpa2_psk_ccmp;
    ecore_list_append(l_enc, elt);
    elt = malloc(sizeof(Default_Elt*));
    elt->label = strdup(WIRELESS_ENCRYPTION_TEXT_WPA2_PSK_TKIP_ASCII);
    elt->value = (int*)en_wpa2_psk_tkip;
    ecore_list_append(l_enc, elt);

    Exalt_Enum_Security_Mode *sec_open = malloc(sizeof(Exalt_Enum_Security_Mode));
    *sec_open = EXALT_SECURITY_OPEN;
    Exalt_Enum_Security_Mode *sec_shared = malloc(sizeof(Exalt_Enum_Security_Mode));
    *sec_shared = EXALT_SECURITY_SHARED;

    elt = malloc(sizeof(Default_Elt*));
    elt->label = strdup(WIRELESS_SECURITY_TEXT_OPEN);
    elt->value = (int*)sec_open;
    ecore_list_append(l_sec, elt);
    elt = malloc(sizeof(Default_Elt*));
    elt->label = strdup(WIRELESS_SECURITY_TEXT_SHARED);
    elt->value = (int*)sec_shared;
    ecore_list_append(l_sec, elt);


    Exalt_Enum_Connection_Mode *mode_adhoc = malloc(sizeof(Exalt_Enum_Connection_Mode));
    *mode_adhoc = EXALT_CONNECTION_ADHOC;
    Exalt_Enum_Connection_Mode *mode_managed = malloc(sizeof(Exalt_Enum_Connection_Mode));
    *mode_managed = EXALT_CONNECTION_MANAGED;

    elt = malloc(sizeof(Default_Elt*));
    elt->label = strdup(WIRELESS_MODE_TEXT_ADHOC);
    elt->value = (int*)mode_adhoc;
    ecore_list_append(l_conn, elt);
    elt = malloc(sizeof(Default_Elt*));
    elt->label = strdup(WIRELESS_MODE_TEXT_MANAGED);
    elt->value = (int*)mode_managed;
    ecore_list_append(l_conn, elt);


    pnl->entry_conn_essid = ewl_entry_new();
    ewl_text_text_set(EWL_TEXT(pnl->entry_conn_essid),"");
    ewl_widget_show(pnl->entry_conn_essid);
    pnl->entry_conn_pwd = ewl_entry_new();
    //etk_entry_password_mode_set(ETK_ENTRY(pnl->entry_conn_pwd),ETK_TRUE);
    ewl_widget_show(pnl->entry_conn_pwd);
    pnl->entry_conn_ip = ewl_entry_new();
    ewl_widget_show(pnl->entry_conn_ip);
    pnl->entry_conn_mask = ewl_entry_new();
    ewl_widget_show(pnl->entry_conn_mask);
    pnl->entry_conn_gateway = ewl_entry_new();
    ewl_widget_show(pnl->entry_conn_gateway);
    pnl->entry_conn_cmd = ewl_entry_new();
    ewl_widget_show(pnl->entry_conn_cmd);

    pnl->btn_apply = ewl_button_new();
    ewl_stock_type_set(EWL_STOCK(pnl->btn_apply), EWL_STOCK_APPLY);
    ewl_object_fill_policy_set(EWL_OBJECT(pnl->btn_apply), EWL_FLAG_FILL_SHRINK);
    ewl_widget_show(pnl->btn_apply);

    pnl->check_static = ewl_radiobutton_new();
    ewl_button_label_set(EWL_BUTTON(pnl->check_static),_("Static address"));
    ewl_widget_show(pnl->check_static);
    pnl->check_dhcp = ewl_radiobutton_new();
    ewl_button_label_set(EWL_BUTTON(pnl->check_dhcp),_("DHCP (Automatic configuration)"));
    ewl_radiobutton_chain_set(EWL_RADIOBUTTON(pnl->check_dhcp),
            EWL_RADIOBUTTON(pnl->check_static));
    ewl_widget_show(pnl->check_dhcp);

    pnl->cmbox_encryption = ewl_combo_new();
    /* model */
    model = ewl_model_new();
    ewl_model_data_fetch_set (model,EWL_MODEL_DATA_FETCH (wirelesspanel_default_tree_data_fetch));
    ewl_model_data_count_set (model,EWL_MODEL_DATA_COUNT (wirelesspanel_default_tree_data_count));
    /* view */
    view = ewl_label_view_get();
    /* MVC */
    ewl_mvc_selection_mode_set (EWL_MVC (pnl->cmbox_encryption), EWL_SELECTION_MODE_SINGLE);
    ewl_mvc_model_set (EWL_MVC (pnl->cmbox_encryption), model);
    ewl_mvc_view_set (EWL_MVC(pnl->cmbox_encryption), view);
    ewl_mvc_data_set (EWL_MVC(pnl->cmbox_encryption), l_enc);
    ewl_widget_show(pnl->cmbox_encryption);


    pnl->cmbox_security = ewl_combo_new();
    /* model */
    model = ewl_model_new();
    ewl_model_data_fetch_set (model,EWL_MODEL_DATA_FETCH (wirelesspanel_default_tree_data_fetch));
    ewl_model_data_count_set (model,EWL_MODEL_DATA_COUNT (wirelesspanel_default_tree_data_count));
    /* view */
    view = ewl_label_view_get();
    /* MVC */
    ewl_mvc_selection_mode_set (EWL_MVC (pnl->cmbox_security), EWL_SELECTION_MODE_SINGLE);
    ewl_mvc_model_set (EWL_MVC (pnl->cmbox_security), model);
    ewl_mvc_view_set (EWL_MVC(pnl->cmbox_security), view);
    ewl_mvc_data_set (EWL_MVC(pnl->cmbox_security), l_sec);
    ewl_widget_show(pnl->cmbox_security);


    pnl->cmbox_mode = ewl_combo_new();
    /* model */
    model = ewl_model_new();
    ewl_model_data_fetch_set (model,EWL_MODEL_DATA_FETCH (wirelesspanel_default_tree_data_fetch));
    ewl_model_data_count_set (model,EWL_MODEL_DATA_COUNT (wirelesspanel_default_tree_data_count));
    /* view */
    view = ewl_label_view_get();
    /* MVC */
    ewl_mvc_selection_mode_set (EWL_MVC (pnl->cmbox_mode), EWL_SELECTION_MODE_SINGLE);
    ewl_mvc_model_set (EWL_MVC (pnl->cmbox_mode), model);
    ewl_mvc_view_set (EWL_MVC(pnl->cmbox_mode), view);
    ewl_mvc_data_set (EWL_MVC(pnl->cmbox_mode), l_conn);
    ewl_widget_show(pnl->cmbox_mode);

    pnl->hbox_pbar = ewl_hbox_new();
    pnl->pbar = ewl_progressbar_new();
    ewl_container_child_append(EWL_CONTAINER(pnl->hbox_pbar), pnl->pbar);
    ewl_progressbar_label_set(EWL_PROGRESSBAR(pnl->pbar), APPLY_TEXT);
    ewl_range_unknown_set(EWL_RANGE(pnl->pbar), TRUE);
    ewl_widget_show(pnl->hbox_pbar);
    ewl_widget_show(pnl->pbar);

    pnl->cmbox_driver = ewl_combo_new();
    ewl_widget_name_set(pnl->cmbox_driver,"cmbox_driver");
    ewl_widget_show(pnl->cmbox_driver);
    Ecore_List *l_driver = ecore_list_new();
    elt = malloc(sizeof(Default_Elt*));
    elt->label = strdup("wext");
    elt->value = NULL;
    ecore_list_append(l_driver, elt);
    elt = malloc(sizeof(Default_Elt*));
    elt->label = strdup("hostap");
    elt->value = NULL;
    ecore_list_append(l_driver, elt);
    elt = malloc(sizeof(Default_Elt*));
    elt->label = strdup("prism54");
    elt->value = NULL;
    ecore_list_append(l_driver, elt);
    elt = malloc(sizeof(Default_Elt*));
    elt->label = strdup("atmel");
    elt->value = NULL;
    ecore_list_append(l_driver, elt);
    elt = malloc(sizeof(Default_Elt*));
    elt->label = strdup("ndiswrapper");
    elt->value = NULL;
    ecore_list_append(l_driver, elt);
    elt = malloc(sizeof(Default_Elt*));
    elt->label = strdup("ipw");
    elt->value = NULL;
    ecore_list_append(l_driver, elt);
    elt = malloc(sizeof(Default_Elt*));
    elt->label = strdup("wired");
    elt->value = NULL;
    ecore_list_append(l_driver, elt);

    /* model */
    model = ewl_model_new();
    ewl_model_data_fetch_set (model,EWL_MODEL_DATA_FETCH (wirelesspanel_default_tree_data_fetch));
    ewl_model_data_count_set (model,EWL_MODEL_DATA_COUNT (wirelesspanel_default_tree_data_count));
    /* view */
    view = ewl_label_view_get();
    ewl_view_header_fetch_set(view,wirelesspanel_default_editable_header_fetch);
    /* MVC */
    ewl_mvc_selection_mode_set (EWL_MVC (pnl->cmbox_driver), EWL_SELECTION_MODE_SINGLE);
    ewl_mvc_model_set (EWL_MVC (pnl->cmbox_driver), model);
    ewl_mvc_view_set (EWL_MVC(pnl->cmbox_driver), view);
    ewl_mvc_data_set (EWL_MVC(pnl->cmbox_driver), l_driver);
    ewl_combo_editable_set(EWL_COMBO(pnl->cmbox_driver), TRUE);

    label = ewl_label_new();
    ewl_label_text_set(EWL_LABEL(label),_("Essid: "));
    ewl_container_child_append(EWL_CONTAINER(grid), label);
    ewl_container_child_append(EWL_CONTAINER(grid), pnl->entry_conn_essid);
    ewl_widget_show(label);

    label = ewl_label_new();
    ewl_label_text_set(EWL_LABEL(label),_("Encryption mode: "));
    ewl_container_child_append(EWL_CONTAINER(grid), label);
    ewl_container_child_append(EWL_CONTAINER(grid), pnl->cmbox_encryption);
    ewl_widget_show(label);

    label = ewl_label_new();
    ewl_label_text_set(EWL_LABEL(label),_("Password: "));
    ewl_container_child_append(EWL_CONTAINER(grid), label);
    ewl_container_child_append(EWL_CONTAINER(grid), pnl->entry_conn_pwd);
    ewl_widget_show(label);

    label = ewl_label_new();
    ewl_label_text_set(EWL_LABEL(label),_("Connection mode: "));
    ewl_container_child_append(EWL_CONTAINER(grid), label);
    ewl_container_child_append(EWL_CONTAINER(grid), pnl->cmbox_mode);
    ewl_widget_show(label);

    label = ewl_label_new();
    ewl_label_text_set(EWL_LABEL(label),_("Security mode: "));
    ewl_container_child_append(EWL_CONTAINER(grid), label);
    ewl_container_child_append(EWL_CONTAINER(grid), pnl->cmbox_security);
    ewl_widget_show(label);

    ewl_container_child_append(EWL_CONTAINER(grid), pnl->check_static);
    ewl_container_child_append(EWL_CONTAINER(grid), pnl->check_dhcp);
    ewl_widget_show(label);

    label = ewl_label_new();
    ewl_label_text_set(EWL_LABEL(label),_("Ip address: "));
    ewl_container_child_append(EWL_CONTAINER(grid), label);
    ewl_container_child_append(EWL_CONTAINER(grid), pnl->entry_conn_ip);
    ewl_widget_show(label);

    label = ewl_label_new();
    ewl_label_text_set(EWL_LABEL(label),_("Network mask: "));
    ewl_container_child_append(EWL_CONTAINER(grid), label);
    ewl_container_child_append(EWL_CONTAINER(grid), pnl->entry_conn_mask);
    ewl_widget_show(label);

    label = ewl_label_new();
    ewl_label_text_set(EWL_LABEL(label),_("Gateway: "));
    ewl_container_child_append(EWL_CONTAINER(grid), label);
    ewl_container_child_append(EWL_CONTAINER(grid), pnl->entry_conn_gateway);
    ewl_widget_show(label);

    pnl->lbl_conn_cmd = ewl_label_new();
    ewl_label_text_set(EWL_LABEL(pnl->lbl_conn_cmd),_("Command: "));
    ewl_container_child_append(EWL_CONTAINER(grid), pnl->lbl_conn_cmd);
    ewl_container_child_append(EWL_CONTAINER(grid), pnl->entry_conn_cmd);
    ewl_widget_show(pnl->lbl_conn_cmd);

    pnl->lbl_driver = ewl_label_new();
    ewl_label_text_set(EWL_LABEL(pnl->lbl_driver),_("Wpa_supplicant driver: "));
    ewl_container_child_append(EWL_CONTAINER(grid), pnl->lbl_driver);
    ewl_container_child_append(EWL_CONTAINER(grid), pnl->cmbox_driver);
    ewl_widget_show(pnl->lbl_driver);


    ewl_container_child_append(EWL_CONTAINER(grid), pnl->btn_apply);
    ewl_container_child_append(EWL_CONTAINER(grid), pnl->hbox_pbar);

    wirelesspanel_callback_append(pnl);
    return grid;
}

void wirelesspanel_scanlist_row_clicked_cb(Ewl_Widget *w __UNUSED__,
        void *ev_data __UNUSED__,
        void *user_data)
{
    Etk_Tree *tree;
    char *row_name;
    wireless_panel* pnl;
    /*if (!(tree = ETK_TREE(object)) || !row || !event || !data)
      return;

      pnl = (wireless_panel*)data;

      etk_tree_row_fields_get(row, etk_tree_nth_col_get(tree, 2),  &row_name, NULL);
      etk_entry_text_set(ETK_ENTRY(pnl->entry_conn_essid), row_name);
      */
}

void wirelesspanel_scan_networks_cb(char* interface, Ecore_List* networks, void* data)
{
    wireless_panel* pnl;
    Ecore_List *l;
    char* essid;
    Scan_Elt *elt;


    EXALT_ASSERT_RETURN_VOID(interface!=NULL);
    EXALT_ASSERT_RETURN_VOID(networks!=NULL);

    pnl=(wireless_panel*)data;
    l = pnl->scan_list;

    //delete the old networks
    //they are in the tree but not in the networks list.
    ecore_list_first_goto(l);
    while( (elt=ecore_list_next(l)))
    {
        ecore_list_first_goto(networks);
        while( (essid=ecore_list_next(networks)))
        {
            if(essid && elt->essid && strcmp(essid, elt->essid)==0)
                break;
        }
        if(!essid)
        {
            int index = ecore_list_index(l);
            ecore_list_index_goto(l,index-1);
            ecore_list_remove_destroy(l);
        }
    }

    ecore_list_first_goto(networks);
    while( (essid = ecore_list_next(networks)))
    {
        int quality = exalt_dbus_wirelessnetwork_get_quality(exalt_conn, interface, essid)/25;
        if(quality<0)
            quality = 0;
        if(quality>3)
            quality=3;

        ecore_list_first_goto(l);
        short find = 0;
        while(!find && (elt=ecore_list_next(l)))
        {
            if(essid && elt->essid && strcmp(essid, elt->essid)==0)
                find = 1;
        }
        if(find)
        {
            *(elt->quality) = quality;
            *(elt->encryption) = exalt_dbus_wirelessnetwork_get_encryption(exalt_conn, interface, essid);
        }
        else
        {
            //we add the network
            elt = malloc(sizeof(Scan_Elt));
            elt->essid  = strdup(essid);
            elt->quality = malloc(sizeof(int));
            elt->encryption = malloc(sizeof(int));
            *(elt->quality) = quality;
            *(elt->encryption)= exalt_dbus_wirelessnetwork_get_encryption(exalt_conn, interface, essid);
            ecore_list_append(l,elt);
            //select the network if it is in the argument (exalt -w essid_name)
            if(essid && exalt_default_network && strcmp(essid,exalt_default_network)==0)
            {
                ewl_text_text_set(EWL_TEXT(pnl->entry_conn_essid),exalt_default_network);
                wirelesspanel_textchanged_entry_cb(NULL,NULL,pnl);
            }
        }
    }
    ewl_mvc_dirty_set(EWL_MVC(pnl->scan_tree), TRUE);
}

int wirelesspanel_scan_timer_cb(void *data)
{
    wireless_panel *pnl = data;
    exalt_dbus_wireless_scan_start(exalt_conn,pnl->interface);
    return 1;
}

void wirelesspanel_btn_deactivate_clicked_cb(Ewl_Widget *w __UNUSED__,
        void *ev_data __UNUSED__,
        void *user_data)
{
    wireless_panel* pnl;
    if(!user_data)
    {
        fprintf(stderr,"ethpanel_btn_deactivate_cb(): data== null!\n");
        return ;
    }

    pnl = (wireless_panel*)user_data;
    exalt_dbus_eth_down(exalt_conn,pnl->interface);
}

void wirelesspanel_btn_activate_clicked_cb(Ewl_Widget *w __UNUSED__,
        void *ev_data __UNUSED__,
        void *user_data)
{
    wireless_panel* pnl;
    if(!user_data)
    {
        fprintf(stderr,"ethpanel_btn_activate_cb(): data== null!\n");
        return ;
    }

    pnl = (wireless_panel*)user_data;
    exalt_dbus_eth_up(exalt_conn,pnl->interface);
}


void wirelesspanel_set_static_dhcp_clicked_cb(Ewl_Widget *w __UNUSED__,
        void *ev_data __UNUSED__,
        void *user_data)
{
    wireless_panel* pnl;

    if(!user_data)
    {
        perror("wirelesspanel_set_staticaddress_clicked_cb(): data==null ! \n");
        return ;
    }

    pnl = (wireless_panel*)user_data;
    wirelesspanel_disabled_widget_activate(NULL,NULL,pnl);
}

void wirelesspanel_cmboxencryption_active_item_changed_cb(Ewl_Widget *w __UNUSED__,
        void *ev_data __UNUSED__,
        void *user_data)
{
    wireless_panel* pnl;
    if(!user_data)
    {
        fprintf(stderr,"wirelesspanel_cmboxencryption_active_item_changed_cb(): data==null ! \n");
        return ;
    }

    pnl = (wireless_panel*)user_data;
    wirelesspanel_disabled_widget_activate(NULL,NULL,pnl);
}

void wirelesspanel_btn_apply_clicked_cb(Ewl_Widget *w __UNUSED__,
        void *ev_data __UNUSED__,
        void *user_data)
{
    wireless_panel* pnl;
    int* encryption;
    int *mode;
    int *security;
    const char* driver;
    const char* cmd;
    Etk_Combobox_Item *active_item;
    Exalt_Connection *c;

    if(!user_data)
    {
        perror("wirelesspanel_btn_apply_clicked_cb(): data==null ! \n");
        return ;
    }

    c = exalt_conn_new();
    if(!c)
    {
        perror("wirelesspanel_btn_apply_clicked_cb(): c==null ! \n");
        return ;
    }
    exalt_conn_set_wireless(c,1);

    pnl = (wireless_panel*)user_data;
    /*
       if(etk_toggle_button_active_get(ETK_TOGGLE_BUTTON(pnl->check_static)))
       {
       exalt_conn_set_mode(c,EXALT_STATIC);
       exalt_conn_set_ip(c,etk_entry_text_get(ETK_ENTRY(pnl->entry_conn_ip)));
       exalt_conn_set_netmask(c,etk_entry_text_get(ETK_ENTRY(pnl->entry_conn_mask)));
       exalt_conn_set_gateway(c,etk_entry_text_get(ETK_ENTRY(pnl->entry_conn_gateway)));
       }
    //else nothing
    // the default connection is dhcp

    exalt_conn_set_essid(c ,etk_entry_text_get(ETK_ENTRY(pnl->entry_conn_essid)));

    //get the passwd mode
    if ((active_item = etk_combobox_active_item_get(ETK_COMBOBOX(pnl->cmbox_encryption))))
    {
    encryption = etk_combobox_item_data_get(active_item);
    exalt_conn_set_encryption_mode(c,*encryption);
    if(*encryption!=EXALT_ENCRYPTION_NONE)
    exalt_conn_set_key(c,etk_entry_text_get(ETK_ENTRY(pnl->entry_conn_pwd)));
    }

    //get the mode
    if ((active_item = etk_combobox_active_item_get(ETK_COMBOBOX(pnl->cmbox_mode))))
    {
    mode = etk_combobox_item_data_get(active_item);
    exalt_conn_set_connection_mode(c ,*mode);
    }
    //get the security mode
    if ((active_item = etk_combobox_active_item_get(ETK_COMBOBOX(pnl->cmbox_security))))
    {
    security = etk_combobox_item_data_get(active_item);
    exalt_conn_set_security_mode(c,*security);
    }

    //apply the driver
    driver = etk_entry_text_get(ETK_ENTRY(etk_combobox_entry_entry_get(ETK_COMBOBOX_ENTRY(pnl->cmbox_driver))));
    if(driver)
    exalt_dbus_wireless_set_wpasupplicant_driver(exalt_conn, pnl->interface, driver);

    cmd = etk_entry_text_get(ETK_ENTRY(pnl->entry_conn_cmd));
    exalt_conn_set_cmd(c, cmd);

    if(!exalt_dbus_eth_apply_conn(exalt_conn, pnl->interface,c))
    return ;

    ewl_widget_show(pnl->hbox_pbar);
    ewl_widget_disable(pnl->win->eth_list);
    ewl_widget_disable(pnl->win->btn_mode);
    ewl_widget_disable(pnl->btn_activate);
    ewl_widget_disable(pnl->btn_apply);
    pnl->apply=1;
    wirelesspanel_disabled_widget_activate(NULL,NULL,pnl);

    exalt_conn_free(c);
    */
}


void wirelesspanel_textchanged_entry_cb(Ewl_Widget *w ,
        void *ev_data __UNUSED__,
        void *user_data)
{
    wireless_panel* pnl;
    const char* gateway;
    const char* ip;
    const char* mask;
    const char* essid;
    const char* key;
    char* interface;
    Ecore_List* l;
    short find;
    Default_Elt* elt;

    if(!user_data)
        return ;

    printf("textchanged\n");
    pnl=(wireless_panel*) user_data;

    wirelesspanel_callback_del(pnl);

    essid = ewl_text_text_get(EWL_TEXT(pnl->entry_conn_essid));
    interface = pnl->interface;

    //load the default configuration
    Exalt_Connection *c;
    Exalt_Enum_Security_Mode security_mode;
    Exalt_Enum_Connection_Mode connection_mode;
    Exalt_Enum_Encryption_Mode encryption_mode;
    short dhcp;

    //try to find if the essid exist to load the default configuration
    if(w==pnl->entry_conn_essid)
    {
        c = exalt_dbus_wirelessnetwork_get_default_conn(exalt_conn, interface, essid);
        encryption_mode = exalt_conn_get_encryption_mode(c);
        connection_mode = exalt_conn_get_connection_mode(c);
        security_mode = exalt_conn_get_security_mode(c);
        dhcp = exalt_conn_is_dhcp(c);

        if(dhcp)
        {
            ewl_checkbutton_checked_set(EWL_CHECKBUTTON(pnl->check_static),FALSE);
            ewl_checkbutton_checked_set(EWL_CHECKBUTTON(pnl->check_dhcp),TRUE);
            ewl_text_clear(EWL_TEXT(pnl->entry_conn_ip));
            ewl_text_clear(EWL_TEXT(pnl->entry_conn_gateway));
            ewl_text_clear(EWL_TEXT(pnl->entry_conn_mask));
        }
        else
        {
            ewl_checkbutton_checked_set(EWL_CHECKBUTTON(pnl->check_static),TRUE);
            ewl_checkbutton_checked_set(EWL_CHECKBUTTON(pnl->check_dhcp),FALSE);

            ewl_text_text_set(EWL_TEXT(pnl->entry_conn_ip),exalt_conn_get_ip(c));
            ewl_text_text_set(EWL_TEXT(pnl->entry_conn_gateway),exalt_conn_get_gateway(c));
            ewl_text_text_set(EWL_TEXT(pnl->entry_conn_mask),exalt_conn_get_netmask(c));
        }

        if(encryption_mode != EXALT_ENCRYPTION_NONE)
            ewl_text_text_set(EWL_TEXT(pnl->entry_conn_pwd),exalt_conn_get_key(c));
        else
            ewl_text_clear(EWL_TEXT(pnl->entry_conn_pwd));

        ewl_text_text_set(EWL_TEXT(pnl->entry_conn_cmd),exalt_conn_get_cmd(c));

        exalt_conn_free(c);

        l = ewl_mvc_data_get(EWL_MVC(pnl->cmbox_encryption));
        ecore_list_first_goto(l);
        find = 0;
        while( !find && (elt=ecore_list_next(l)) )
        {
            if(encryption_mode == (unsigned int)*(elt->value))
                find =1;
        }
        ewl_mvc_selected_set(EWL_MVC(pnl->cmbox_encryption),
                ewl_mvc_model_get(EWL_MVC(pnl->cmbox_encryption)),
                l,
                ecore_list_index(l)-1,
                0);

    }

/*
    Ewl_Selection *sel;
    char* val;
    l = ewl_mvc_selected_list_get(EWL_MVC(pnl->cmbox_encryption));
    ecore_list_first_goto(l);
    sel = ecore_list_next(l);
    if(sel)
    {
        val = sel->model->fetch(sel->data, EWL_SELECTION_IDX(sel)->row, 0);
        l = ewl_mvc_data_get(EWL_MVC(pnl->cmbox_encryption));
        ecore_list_first_goto(l);
        find = 0;
        while( !find && (elt=ecore_list_next(l)) )
        {
            if(val && elt->label && strcmp(val,elt->label))
                find =1;
        }
        encryption_mode = (unsigned int)*(elt->value);
    }
    else
        encryption_mode=EXALT_ENCRYPTION_NONE;


    //verify if all entry contains a valid address && essid & password
    gateway = ewl_text_text_get(EWL_TEXT(pnl->entry_conn_gateway));
    ip = ewl_text_text_get(EWL_TEXT(pnl->entry_conn_ip));
    mask = ewl_text_text_get(EWL_TEXT(pnl->entry_conn_mask));
    key = ewl_text_text_get(EWL_TEXT(pnl->entry_conn_pwd));

    if( exalt_is_essid(essid)
            && exalt_is_key(key,encryption_mode)
            &&(
                ewl_radiobutton_is_checked(EWL_RADIOBUTTON(pnl->check_dhcp))
                || (exalt_is_address(ip)
                    && exalt_is_address(mask)
                    && (exalt_is_address(gateway) || (gateway && strlen(gateway)==0 ))
                   )
              )
      )
        ewl_widget_enable(pnl->btn_apply);
    else
        ewl_widget_disable(pnl->btn_apply);

    wirelesspanel_callback_append(pnl);
    */
    wirelesspanel_disabled_widget_activate(NULL,NULL,pnl);
}


void wirelesspanel_conn_apply_done(wireless_panel* pnl)
{
    pnl->apply=0;
    ewl_widget_enable(pnl->win->eth_tree);
    ewl_widget_enable(pnl->win->btn_mode);
    ewl_widget_hide(pnl->pbar);
    wirelesspanel_disabled_widget_activate(NULL,NULL,pnl);
}


Etk_Combobox_Item * exalt_etk_combobox_data_item_get(Etk_Combobox *combobox, void *data)
{
    Etk_Combobox_Item *item;
    void * data2;
    item = etk_combobox_first_item_get(combobox);
    while(item)
    {
        data2 = etk_combobox_item_data_get(item);
        if(strcmp(data2,data)==0)
            return item;
        else
            item = etk_combobox_item_next_get(item);
    }
    return NULL;
}




void wirelesspanel_callback_append(wireless_panel *pnl)
{
    wirelesspanel_callback_del(pnl);
    ewl_callback_append(pnl->entry_conn_essid, EWL_CALLBACK_VALUE_CHANGED,
            wirelesspanel_textchanged_entry_cb , pnl);
    ewl_callback_append(pnl->entry_conn_pwd, EWL_CALLBACK_VALUE_CHANGED,
            wirelesspanel_disabled_widget_activate, pnl);
    ewl_callback_append(pnl->entry_conn_ip, EWL_CALLBACK_VALUE_CHANGED,
            wirelesspanel_disabled_widget_activate, pnl);
    ewl_callback_append(pnl->entry_conn_mask, EWL_CALLBACK_VALUE_CHANGED,
             wirelesspanel_disabled_widget_activate, pnl);
    ewl_callback_append(pnl->entry_conn_gateway, EWL_CALLBACK_VALUE_CHANGED,
             wirelesspanel_disabled_widget_activate, pnl);
    ewl_callback_append(pnl->check_static, EWL_CALLBACK_CLICKED,
            wirelesspanel_textchanged_entry_cb , pnl);
    ewl_callback_append(pnl->check_dhcp, EWL_CALLBACK_CLICKED,
             wirelesspanel_disabled_widget_activate , pnl);
    ewl_callback_append(pnl->cmbox_encryption, EWL_CALLBACK_VALUE_CHANGED,
              wirelesspanel_disabled_widget_activate, pnl);
}



void wirelesspanel_callback_del(wireless_panel *pnl)
{
    ewl_callback_del(pnl->entry_conn_essid, EWL_CALLBACK_VALUE_CHANGED,
            wirelesspanel_textchanged_entry_cb);
    ewl_callback_del(pnl->entry_conn_pwd, EWL_CALLBACK_VALUE_CHANGED,
            wirelesspanel_disabled_widget_activate);
    ewl_callback_del(pnl->entry_conn_ip, EWL_CALLBACK_VALUE_CHANGED,
            wirelesspanel_disabled_widget_activate);
    ewl_callback_del(pnl->entry_conn_mask, EWL_CALLBACK_VALUE_CHANGED,
             wirelesspanel_disabled_widget_activate);
    ewl_callback_del(pnl->entry_conn_gateway, EWL_CALLBACK_VALUE_CHANGED,
             wirelesspanel_disabled_widget_activate);
    ewl_callback_del(pnl->check_static, EWL_CALLBACK_CLICKED,
            wirelesspanel_textchanged_entry_cb);
    ewl_callback_del(pnl->check_dhcp, EWL_CALLBACK_CLICKED,
             wirelesspanel_disabled_widget_activate);
    ewl_callback_del(pnl->cmbox_encryption, EWL_CALLBACK_VALUE_CHANGED,
              wirelesspanel_disabled_widget_activate);
}





unsigned int wirelesspanel_scan_package_tree_data_count (void *data)
{
    if(!data)
        return 0;

    Ecore_List* l = (Ecore_List*)data;
    return ecore_list_count(l);
}

void* wirelesspanel_scan_package_tree_data_fetch (
        void        *data,
        unsigned int row,
        unsigned int column)
{
    if(!data)
        return 0;

    Ecore_List* l = (Ecore_List*)data;
    ecore_list_index_goto(l,row);
    Scan_Elt * elt =  ecore_list_current(l);
    return elt;
}


void * wirelesspanel_scan_cb_header_data_fetch(void *data __UNUSED__, unsigned int column)
{
    return _("Essid");
}


Ewl_Widget * wirelesspanel_scan_cb_widget_fetch(void *data, unsigned int row __UNUSED__,
        unsigned int column __UNUSED__)
{
    Ewl_Widget *w_hbox, *w_quality, *w_encryption, *w_essid;
    const char* encryption = ewl_icon_theme_icon_path_get(EWL_ICON_DIALOG_PASSWORD, EWL_ICON_SIZE_SMALL);
    char img1[] = PACKAGE_DATA_DIR ICONS_QUALITY_LESS_25;
    char img2[] = PACKAGE_DATA_DIR ICONS_QUALITY_LESS_50;
    char img3[] = PACKAGE_DATA_DIR ICONS_QUALITY_LESS_75;
    char img4[] = PACKAGE_DATA_DIR ICONS_QUALITY_LESS_100;
    char *img[4];
    img[0] = img1;img[1] = img2;img[2] = img3;img[3] = img4;

    Scan_Elt* elt = (Scan_Elt*) data;

    w_hbox = ewl_hbox_new();

    if(*(elt->encryption))
    {
        w_encryption = ewl_image_new();
        ewl_image_file_path_set(EWL_IMAGE(w_encryption), encryption);
    }
    else
        w_encryption = NULL;

    w_essid = ewl_label_new();
    ewl_label_text_set(EWL_LABEL(w_essid), elt->essid);

    int quality = (*(elt->quality));

    w_quality = ewl_image_new();
    ewl_image_file_path_set(EWL_IMAGE(w_quality), img[quality]);

    ewl_container_child_append(EWL_CONTAINER(w_hbox), w_encryption);
    ewl_container_child_append(EWL_CONTAINER(w_hbox), w_quality);
    ewl_container_child_append(EWL_CONTAINER(w_hbox), w_essid);

    ewl_widget_show(w_hbox);
    ewl_widget_show(w_encryption);
    ewl_widget_show(w_quality);
    ewl_widget_show(w_essid);

    return w_hbox;
}





void scan_elt_free(void *data)
{
    Scan_Elt *elt = (Scan_Elt*)data;
    EXALT_FREE(elt->encryption);
    EXALT_FREE(elt->essid);
    EXALT_FREE(elt->quality);
    EXALT_FREE(elt);
}












void default_elt_free(void* data)
{
    Default_Elt* elt = (Default_Elt*) data;
    EXALT_FREE(elt->label);
    EXALT_FREE(elt->value);
}

unsigned int wirelesspanel_default_tree_data_count (void *data)
{
    if(!data)
        return 0;

    Ecore_List* l = (Ecore_List*)data;
    return ecore_list_count(l);
}

void* wirelesspanel_default_tree_data_fetch (
        void        *data,
        unsigned int row,
        unsigned int column)
{
    if(!data)
        return 0;

    Ecore_List* l = (Ecore_List*)data;
    ecore_list_index_goto(l,row);
    Default_Elt * elt =  ecore_list_current(l);
    return elt->label;
}



Ewl_Widget *wirelesspanel_default_editable_header_fetch(void *data, unsigned int col __UNUSED__)
{
    Ecore_List *d;
    Ewl_Widget *w,*o;
    Ewl_Selection_Idx *idx;
    char *val;

    d = data;
    w = ewl_widget_name_find("cmbox_driver");

    idx = ewl_mvc_selected_get(EWL_MVC(w));

    val = "wext";
    o = ewl_entry_new();

    if (idx)
    {
        ecore_list_index_goto(d,idx->row);
        Default_Elt* elt= ecore_list_next(d);
        ewl_text_text_set(EWL_TEXT(o),elt->label);
    }
    else
        ewl_text_text_set(EWL_TEXT(o), val);

    ewl_widget_show(o);

    return o;
}
