
#include "wireless_panel.h"



wireless_panel* wirelesspanel_create(main_window* win)
{
    Etk_Widget *hbox,*vbox,*scroll,*label, *table;
    wireless_panel* pnl;
    pnl=(wireless_panel*)malloc((unsigned int)sizeof(wireless_panel));
    pnl->win=win;
    pnl->interface = NULL;
    pnl->pulsebar_timer = NULL;

    pnl->frame = etk_frame_new("wireless_frame");

    hbox = etk_hbox_new(ETK_FALSE, 5);
    etk_container_add(ETK_CONTAINER(pnl->frame), hbox);

    //##############################
    //## btn activate/deactivate ##
    //##############################
    vbox = etk_vbox_new(ETK_FALSE, 5);
    etk_box_append(ETK_BOX(hbox), vbox, ETK_BOX_START, ETK_BOX_EXPAND_FILL, 0);


    pnl->box_activate = etk_hbox_new(ETK_FALSE,5);
    etk_box_append(ETK_BOX(vbox), pnl->box_activate, ETK_BOX_START, ETK_BOX_FILL, 0);
    pnl->btn_activate = etk_button_new_with_label(_("Activate"));
    pnl->btn_disactivate = etk_button_new_with_label(_("Disactivate"));
    etk_box_append(ETK_BOX(pnl->box_activate), pnl->btn_disactivate, ETK_BOX_START, ETK_BOX_FILL, 0);
    etk_box_append(ETK_BOX(pnl->box_activate), pnl->btn_activate, ETK_BOX_START, ETK_BOX_FILL, 0);
    etk_signal_connect_swapped("clicked", ETK_OBJECT(pnl->btn_activate),ETK_CALLBACK(wirelesspanel_btn_activate_clicked_cb), pnl);
    etk_signal_connect_swapped("clicked", ETK_OBJECT(pnl->btn_disactivate),ETK_CALLBACK(wirelesspanel_btn_disactivate_clicked_cb), pnl);


    //###############
    //## scan list ##
    //###############
    scroll = etk_scrolled_view_new ();
    etk_box_append(ETK_BOX(vbox), scroll, ETK_BOX_START, ETK_BOX_EXPAND_FILL, 0);

    pnl->scan_list = etk_tree_new();
    etk_widget_size_request_set(pnl->scan_list, 200, 50);
    pnl->scan_encryption = etk_tree_col_new(ETK_TREE(pnl->scan_list), _("Encryption"), 0.0, 0.0);
    etk_tree_col_model_add(pnl->scan_encryption, etk_tree_model_image_new());
    pnl->scan_quality = etk_tree_col_new(ETK_TREE(pnl->scan_list), _("Quality"), 50.0, 0.0);
    etk_tree_col_model_add(pnl->scan_quality, etk_tree_model_image_new());
    pnl->scan_essid = etk_tree_col_new(ETK_TREE(pnl->scan_list), _("Essid"), 0.0, 0.0);
    etk_tree_col_model_add(pnl->scan_essid, etk_tree_model_text_new());

    etk_tree_mode_set(ETK_TREE(pnl->scan_list), ETK_TREE_MODE_LIST);
    etk_signal_connect("row-clicked", ETK_OBJECT(pnl->scan_list),ETK_CALLBACK(wirelesspanel_scanlist_row_clicked_cb), pnl);
    etk_tree_build(ETK_TREE(pnl->scan_list));
    etk_container_add(ETK_CONTAINER(scroll), pnl->scan_list);


    //###########################
    //## current configuration ##
    //###########################
    table = etk_table_new(2, 4,ETK_TABLE_HOMOGENEOUS);
    etk_box_append(ETK_BOX(vbox), table, ETK_BOX_START, ETK_BOX_NONE, 0);
    pnl->lbl_essid = etk_label_new(_("none"));
    pnl->lbl_ip = etk_label_new(_("none"));
    pnl->lbl_gateway = etk_label_new(_("none"));
    pnl->lbl_mask = etk_label_new(_("none"));

    label = etk_label_new(_("Current essid: "));
    etk_table_attach(ETK_TABLE(table), label, 0, 0, 0, 0, 0, 0, ETK_TABLE_HFILL);
    etk_table_attach_default(ETK_TABLE(table), pnl->lbl_essid, 1, 1, 0, 0);

    label = etk_label_new(_("Current IP: "));
    etk_table_attach(ETK_TABLE(table), label, 0, 0, 1, 1, 0, 0, ETK_TABLE_HFILL);
    etk_table_attach_default(ETK_TABLE(table), pnl->lbl_ip, 1, 1, 1, 1);

    label = etk_label_new(_("Current mask: "));
    etk_table_attach(ETK_TABLE(table), label, 0, 0, 2, 2, 0, 0, ETK_TABLE_HFILL);
    etk_table_attach_default(ETK_TABLE(table), pnl->lbl_mask, 1, 1, 2, 2);

    label = etk_label_new(_("Current gateway: "));
    etk_table_attach(ETK_TABLE(table), label, 0, 0, 3, 3, 0, 0, ETK_TABLE_HFILL);
    etk_table_attach_default(ETK_TABLE(table), pnl->lbl_gateway, 1, 1, 3, 3);


    pnl->box_connection = wirelesspanel_pageconnection_create(pnl);
    etk_box_append(ETK_BOX(hbox), pnl->box_connection, ETK_BOX_START, ETK_BOX_NONE , 0);

    exalt_dbus_scan_notify_set(exalt_conn,wirelesspanel_scan_networks_cb,pnl);

    return pnl;
}

void wirelesspanel_show(wireless_panel* pnl)
{
    etk_tree_clear (ETK_TREE(pnl->scan_list));

    exalt_dbus_wireless_scan_stop(exalt_conn, pnl->interface);
    exalt_dbus_wireless_scan_start(exalt_conn,pnl->interface);

    etk_widget_show_all(pnl->frame);
    wirelesspanel_update_advanced_mode(pnl);
    etk_widget_hide(pnl->hbox_pbar);
}

void wirelesspanel_update_advanced_mode(wireless_panel *pnl)
{
    if(!pnl->win->advanced_mode)
    {
        etk_widget_hide(pnl->lbl_driver);
        etk_widget_hide(pnl->cmbox_driver);
    }
    else
    {
        etk_widget_show(pnl->lbl_driver);
        etk_widget_show(pnl->cmbox_driver);
    }
}

void wirelesspanel_disabled_widget_activate(wireless_panel* pnl)
{
    Etk_Combobox_Item* active_item;
    int* encryption;
    int apply = pnl->pulsebar_timer != NULL;
    int down = !exalt_dbus_eth_is_up(exalt_conn,pnl->interface);

    if(!pnl)
    {
        fprintf(stderr,"wireless_disabled_widget_activate(): pnl==null ! \n");
        return ;
    }

    if(apply)
    {
        etk_widget_disabled_set(pnl->btn_activate,ETK_TRUE);
        etk_widget_disabled_set(pnl->btn_disactivate,ETK_TRUE);
        etk_widget_disabled_set(pnl->scan_list,ETK_TRUE);
        etk_widget_disabled_set(pnl->entry_conn_essid,ETK_TRUE);
        etk_widget_disabled_set(pnl->cmbox_encryption,ETK_TRUE);
        etk_widget_disabled_set(pnl->btn_apply,ETK_TRUE);
        etk_widget_disabled_set(pnl->entry_conn_pwd,ETK_TRUE);
        etk_widget_disabled_set(pnl->cmbox_mode,ETK_TRUE);
        etk_widget_disabled_set(pnl->cmbox_security,ETK_TRUE);
        etk_widget_disabled_set(pnl->entry_conn_ip,ETK_TRUE);
        etk_widget_disabled_set(pnl->entry_conn_mask,ETK_TRUE);
        etk_widget_disabled_set(pnl->entry_conn_gateway,ETK_TRUE);
        etk_widget_disabled_set(pnl->check_static,ETK_TRUE);
        etk_widget_disabled_set(pnl->check_dhcp,ETK_TRUE);
        etk_widget_disabled_set(pnl->cmbox_driver,ETK_TRUE);

        etk_widget_show_all(pnl->hbox_pbar);

        return ;
    }

    if(down)
    {
        etk_widget_disabled_set(pnl->btn_activate,ETK_FALSE);
        etk_widget_disabled_set(pnl->btn_disactivate,ETK_TRUE);
        etk_widget_disabled_set(pnl->scan_list,ETK_TRUE);
        etk_widget_disabled_set(pnl->entry_conn_essid,ETK_TRUE);
        etk_widget_disabled_set(pnl->cmbox_encryption,ETK_TRUE);
        etk_widget_disabled_set(pnl->btn_apply,ETK_TRUE);
        etk_widget_disabled_set(pnl->entry_conn_pwd,ETK_TRUE);
        etk_widget_disabled_set(pnl->cmbox_mode,ETK_TRUE);
        etk_widget_disabled_set(pnl->cmbox_security,ETK_TRUE);
        etk_widget_disabled_set(pnl->entry_conn_ip,ETK_TRUE);
        etk_widget_disabled_set(pnl->entry_conn_mask,ETK_TRUE);
        etk_widget_disabled_set(pnl->entry_conn_gateway,ETK_TRUE);
        etk_widget_disabled_set(pnl->check_static,ETK_TRUE);
        etk_widget_disabled_set(pnl->check_dhcp,ETK_TRUE);
        etk_widget_disabled_set(pnl->cmbox_driver,ETK_TRUE);

        return ;
    }

    etk_widget_disabled_set(pnl->btn_activate,ETK_TRUE);
    etk_widget_disabled_set(pnl->btn_disactivate,ETK_FALSE);
    etk_widget_disabled_set(pnl->scan_list,ETK_FALSE);
    etk_widget_disabled_set(pnl->entry_conn_essid,ETK_FALSE);
    etk_widget_disabled_set(pnl->cmbox_encryption,ETK_FALSE);
    etk_widget_disabled_set(pnl->btn_apply,ETK_FALSE);
    etk_widget_disabled_set(pnl->entry_conn_pwd,ETK_FALSE);
    etk_widget_disabled_set(pnl->cmbox_mode,ETK_FALSE);
    etk_widget_disabled_set(pnl->cmbox_security,ETK_FALSE);
    etk_widget_disabled_set(pnl->entry_conn_ip,ETK_FALSE);
    etk_widget_disabled_set(pnl->entry_conn_mask,ETK_FALSE);
    etk_widget_disabled_set(pnl->entry_conn_gateway,ETK_FALSE);
    etk_widget_disabled_set(pnl->check_static,ETK_FALSE);
    etk_widget_disabled_set(pnl->check_dhcp,ETK_FALSE);
    etk_widget_disabled_set(pnl->cmbox_driver,ETK_FALSE);



    if(!etk_toggle_button_active_get(ETK_TOGGLE_BUTTON(pnl->check_static)))
    {
        //dhcp conf
        etk_widget_disabled_set(pnl->entry_conn_ip,ETK_TRUE);
        etk_widget_disabled_set(pnl->entry_conn_mask,ETK_TRUE);
        etk_widget_disabled_set(pnl->entry_conn_gateway,ETK_TRUE);
    }

    if (!(active_item = etk_combobox_active_item_get(ETK_COMBOBOX(pnl->cmbox_encryption))))
        return;

    encryption = etk_combobox_item_data_get(active_item);

    if(!apply)
    {
        if(encryption && *encryption==EXALT_ENCRYPTION_NONE)
        {
            etk_widget_disabled_set(pnl->entry_conn_pwd, ETK_TRUE);
            etk_widget_disabled_set(pnl->cmbox_security, ETK_TRUE);
        }
        else if(encryption && (*encryption == EXALT_ENCRYPTION_WPA_PSK_CCMP_ASCII
                    || *encryption == EXALT_ENCRYPTION_WPA_PSK_TKIP_ASCII
                    || *encryption == EXALT_ENCRYPTION_WPA2_PSK_CCMP_ASCII
                    || *encryption == EXALT_ENCRYPTION_WPA2_PSK_TKIP_ASCII))
        {
            etk_widget_disabled_set(pnl->cmbox_mode, ETK_TRUE);
            etk_widget_disabled_set(pnl->cmbox_security, ETK_TRUE);
        }
    }

    wirelesspanel_textchanged_entry_cb(NULL,pnl);

    if(!exalt_dhcp_is_support())
        etk_widget_disabled_set(pnl->check_dhcp, ETK_TRUE);
}

void wirelesspanel_hide(wireless_panel* pnl)
{
    etk_widget_hide_all(pnl->frame);
    exalt_dbus_wireless_scan_stop(exalt_conn, pnl->interface);
}

void wirelesspanel_set_eth(wireless_panel* pnl, char* interface)
{
    if(!pnl || !interface)
    {
        print_error("ERROR", __FILE__, __LINE__,__func__,"pnl=%p and interface=%p",pnl,interface);
        return ;
    }
    char name[100];
    sprintf(name,_("Wireless card: %s"),interface);
    EXALT_FREE(pnl->interface);
    pnl->interface = strdup(interface);
    etk_frame_label_set(ETK_FRAME(pnl->frame),name);

    wirelesspanel_disabled_widget_activate(pnl);

    wirelesspanel_update_current_conf(pnl);
}

void wirelesspanel_update_current_conf(wireless_panel* pnl)
{
    if(!pnl)
    {
        fprintf(stderr,"wirelesspanel_update_current_conf(): pnl==null! \n");
        return ;
    }
    etk_label_set(ETK_LABEL(pnl->lbl_essid), exalt_dbus_wireless_get_essid(exalt_conn, pnl->interface));
    etk_label_set(ETK_LABEL(pnl->lbl_ip), exalt_dbus_eth_get_ip(exalt_conn, pnl->interface));
    etk_label_set(ETK_LABEL(pnl->lbl_mask), exalt_dbus_eth_get_netmask(exalt_conn,pnl->interface));
    etk_label_set(ETK_LABEL(pnl->lbl_gateway), exalt_dbus_eth_get_gateway(exalt_conn, pnl->interface));

    etk_entry_text_set(
            ETK_ENTRY(etk_combobox_entry_entry_get(ETK_COMBOBOX_ENTRY(pnl->cmbox_driver)))
            ,exalt_dbus_wireless_get_wpasupplicant_driver(exalt_conn,pnl->interface));
}

Etk_Widget* wirelesspanel_pageconnection_create(wireless_panel* pnl)
{
    Etk_Widget *table,*label;
    Etk_Combobox_Item* item;
    table = etk_table_new(2, 13,ETK_TABLE_NOT_HOMOGENEOUS);
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

    Exalt_Enum_Security_Mode *sec_open = malloc(sizeof(Exalt_Enum_Security_Mode));
    *sec_open = EXALT_SECURITY_OPEN;
    Exalt_Enum_Security_Mode *sec_shared = malloc(sizeof(Exalt_Enum_Security_Mode));
    *sec_shared = EXALT_SECURITY_SHARED;

    Exalt_Enum_Connection_Mode *mode_adhoc = malloc(sizeof(Exalt_Enum_Connection_Mode));
    *mode_adhoc = EXALT_CONNECTION_ADHOC;
    Exalt_Enum_Connection_Mode *mode_managed = malloc(sizeof(Exalt_Enum_Connection_Mode));
    *mode_managed = EXALT_CONNECTION_MANAGED;



    pnl->entry_conn_essid = etk_entry_new();
    etk_entry_clear_button_add(ETK_ENTRY(pnl->entry_conn_essid));
    etk_entry_text_set(ETK_ENTRY(pnl->entry_conn_essid),"");
    pnl->entry_conn_pwd = etk_entry_new();
    etk_entry_password_mode_set(ETK_ENTRY(pnl->entry_conn_pwd),ETK_TRUE);
    etk_entry_clear_button_add(ETK_ENTRY(pnl->entry_conn_pwd));
    pnl->entry_conn_ip = etk_entry_new();
    etk_entry_clear_button_add(ETK_ENTRY(pnl->entry_conn_ip));
    pnl->entry_conn_mask = etk_entry_new();
    etk_entry_clear_button_add(ETK_ENTRY(pnl->entry_conn_mask));
    pnl->entry_conn_gateway = etk_entry_new();
    etk_entry_clear_button_add(ETK_ENTRY(pnl->entry_conn_gateway));
    pnl->btn_apply = etk_button_new_from_stock(ETK_STOCK_DIALOG_APPLY);
    pnl->check_static = etk_radio_button_new_with_label(_("Static address"),NULL);
    pnl->check_dhcp = etk_radio_button_new_with_label_from_widget(_("DHCP (Automatic configuration)"),ETK_RADIO_BUTTON(pnl->check_static));
    pnl->cmbox_encryption = etk_combobox_new();
    pnl->cmbox_security = etk_combobox_new();
    pnl->cmbox_mode = etk_combobox_new();
    pnl->hbox_pbar = etk_hbox_new(ETK_FALSE,5);
    pnl->pbar = etk_progress_bar_new_with_text(APPLY_TEXT);
    etk_progress_bar_pulse_step_set(ETK_PROGRESS_BAR(pnl->pbar), APPLY_PULSE);
    etk_box_append(ETK_BOX(pnl->hbox_pbar), pnl->pbar, ETK_BOX_START, ETK_BOX_EXPAND_FILL, 0);

    etk_combobox_column_add(ETK_COMBOBOX(pnl->cmbox_encryption), ETK_COMBOBOX_LABEL, 50, ETK_TRUE, 0.5);
    etk_combobox_build(ETK_COMBOBOX(pnl->cmbox_encryption));
    item = etk_combobox_item_append(ETK_COMBOBOX(pnl->cmbox_encryption), WIRELESS_ENCRYPTION_TEXT_NONE);
    etk_combobox_item_data_set_full(item, en_none, free);
    item = etk_combobox_item_append(ETK_COMBOBOX(pnl->cmbox_encryption), WIRELESS_ENCRYPTION_TEXT_WEP_ASCII);
    etk_combobox_item_data_set_full(item, en_wep_ascii, free);
    item = etk_combobox_item_append(ETK_COMBOBOX(pnl->cmbox_encryption), WIRELESS_ENCRYPTION_TEXT_WEP_HEXA);
    etk_combobox_item_data_set_full(item, en_wep_hexa, free);

    if(exalt_wpasupplicant_is_support())
    {
        item = etk_combobox_item_append(ETK_COMBOBOX(pnl->cmbox_encryption), WIRELESS_ENCRYPTION_TEXT_WPA_PSK_CCMP_ASCII);
        etk_combobox_item_data_set_full(item, en_wpa_psk_ccmp, free);
        item = etk_combobox_item_append(ETK_COMBOBOX(pnl->cmbox_encryption), WIRELESS_ENCRYPTION_TEXT_WPA_PSK_TKIP_ASCII);
        etk_combobox_item_data_set_full(item, en_wpa_psk_tkip, free);
        item = etk_combobox_item_append(ETK_COMBOBOX(pnl->cmbox_encryption), WIRELESS_ENCRYPTION_TEXT_WPA2_PSK_CCMP_ASCII);
        etk_combobox_item_data_set_full(item, en_wpa2_psk_ccmp, free);
        item = etk_combobox_item_append(ETK_COMBOBOX(pnl->cmbox_encryption), WIRELESS_ENCRYPTION_TEXT_WPA2_PSK_TKIP_ASCII);
        etk_combobox_item_data_set_full(item, en_wpa2_psk_tkip, free);
    }

    etk_combobox_column_add(ETK_COMBOBOX(pnl->cmbox_security), ETK_COMBOBOX_LABEL, 50, ETK_TRUE, 0.5);
    etk_combobox_build(ETK_COMBOBOX(pnl->cmbox_security));
    item = etk_combobox_item_append(ETK_COMBOBOX(pnl->cmbox_security), WIRELESS_SECURITY_TEXT_OPEN);
    etk_combobox_item_data_set_full(item, sec_open, free);
    item = etk_combobox_item_append(ETK_COMBOBOX(pnl->cmbox_security), WIRELESS_SECURITY_TEXT_SHARED);
    etk_combobox_item_data_set_full(item, sec_shared, free);

    etk_combobox_column_add(ETK_COMBOBOX(pnl->cmbox_mode), ETK_COMBOBOX_LABEL, 50, ETK_TRUE, 0.5);
    etk_combobox_build(ETK_COMBOBOX(pnl->cmbox_mode));
    item = etk_combobox_item_append(ETK_COMBOBOX(pnl->cmbox_mode), WIRELESS_MODE_TEXT_ADHOC);
    etk_combobox_item_data_set_full(item, mode_adhoc, free);
    item = etk_combobox_item_append(ETK_COMBOBOX(pnl->cmbox_mode), WIRELESS_MODE_TEXT_MANAGED);
    etk_combobox_item_data_set_full(item, mode_managed, free);
    etk_signal_connect_swapped("clicked", ETK_OBJECT(pnl->btn_apply),ETK_CALLBACK(wirelesspanel_btn_apply_clicked_cb), pnl);


    pnl->cmbox_driver = etk_combobox_entry_new_default();
    etk_combobox_entry_item_append(ETK_COMBOBOX_ENTRY(pnl->cmbox_driver),"wext");
    etk_combobox_entry_item_append(ETK_COMBOBOX_ENTRY(pnl->cmbox_driver),"hostap");
    etk_combobox_entry_item_append(ETK_COMBOBOX_ENTRY(pnl->cmbox_driver),"prism54");
    etk_combobox_entry_item_append(ETK_COMBOBOX_ENTRY(pnl->cmbox_driver),"atmel");
    etk_combobox_entry_item_append(ETK_COMBOBOX_ENTRY(pnl->cmbox_driver),"ndiswrapper");
    etk_combobox_entry_item_append(ETK_COMBOBOX_ENTRY(pnl->cmbox_driver),"ipw");
    etk_combobox_entry_item_append(ETK_COMBOBOX_ENTRY(pnl->cmbox_driver),"wired");
    etk_signal_connect("active-item-changed", ETK_OBJECT(pnl->cmbox_driver), ETK_CALLBACK(wirelesspanel_cmboxdriver_active_item_changed_cb), pnl);


    label = etk_label_new(_("Essid: "));
    etk_table_attach(ETK_TABLE(table), label, 0, 0, 0, 0, 0, 0, ETK_TABLE_HFILL);
    etk_table_attach_default(ETK_TABLE(table), pnl->entry_conn_essid, 1, 1, 0, 0);

    label = etk_label_new(_("Encryption mode: "));
    etk_table_attach(ETK_TABLE(table), label, 0, 0, 1, 1, 0, 0, ETK_TABLE_HFILL);
    etk_table_attach_default(ETK_TABLE(table), pnl->cmbox_encryption, 1, 1, 1, 1);

    label = etk_label_new(_("Password: "));
    etk_table_attach(ETK_TABLE(table), label, 0, 0, 2, 2, 0, 0, ETK_TABLE_HFILL);
    etk_table_attach_default(ETK_TABLE(table), pnl->entry_conn_pwd, 1, 1, 2, 2);

    label = etk_label_new(_("Connection mode: "));
    etk_table_attach(ETK_TABLE(table), label, 0, 0, 3, 3, 0, 0, ETK_TABLE_HFILL);
    etk_table_attach_default(ETK_TABLE(table), pnl->cmbox_mode, 1, 1, 3, 3);

    label = etk_label_new(_("Security mode: "));
    etk_table_attach(ETK_TABLE(table), label, 0, 0, 4, 4, 0, 0, ETK_TABLE_HFILL);
    etk_table_attach_default(ETK_TABLE(table), pnl->cmbox_security, 1, 1, 4, 4);


    etk_table_attach(ETK_TABLE(table), pnl->check_static,0, 0, 5, 5, 0, 0, ETK_TABLE_HFILL);
    etk_table_attach_default(ETK_TABLE(table), pnl->check_dhcp, 1, 1, 5, 5);

    label = etk_label_new(_("Ip address: "));
    etk_table_attach(ETK_TABLE(table), label, 0, 0, 6, 6, 0, 0, ETK_TABLE_HFILL);
    etk_table_attach_default(ETK_TABLE(table), pnl->entry_conn_ip, 1, 1, 6, 6);

    label = etk_label_new(_("Network mask: "));
    etk_table_attach(ETK_TABLE(table), label, 0, 0, 7, 7, 0, 0, ETK_TABLE_HFILL);
    etk_table_attach_default(ETK_TABLE(table), pnl->entry_conn_mask, 1, 1, 7, 7);

    label = etk_label_new(_("Gateway: "));
    etk_table_attach(ETK_TABLE(table), label, 0, 0, 8, 8, 0, 0, ETK_TABLE_HFILL);
    etk_table_attach_default(ETK_TABLE(table), pnl->entry_conn_gateway, 1, 1, 8, 8);


    pnl->lbl_driver = etk_label_new(_("Wpa_supplicant driver: "));
    etk_table_attach(ETK_TABLE(table), pnl->lbl_driver, 0, 0, 9, 9, 0, 0, ETK_TABLE_HFILL);
    etk_table_attach_default(ETK_TABLE(table), pnl->cmbox_driver, 1, 1, 9, 9);


    etk_table_attach(ETK_TABLE(table), pnl->btn_apply, 0, 0, 10, 10, 0, 0, ETK_TABLE_HFILL);
    etk_table_attach_default(ETK_TABLE(table), pnl->hbox_pbar, 1, 1, 10, 10);

    etk_signal_connect("text-changed",ETK_OBJECT(pnl->entry_conn_essid),ETK_CALLBACK(wirelesspanel_textchanged_entry_cb),pnl);
    etk_signal_connect("text-changed",ETK_OBJECT(pnl->entry_conn_pwd),ETK_CALLBACK(wirelesspanel_textchanged_entry_cb),pnl);
    etk_signal_connect("text-changed",ETK_OBJECT(pnl->entry_conn_ip),ETK_CALLBACK(wirelesspanel_textchanged_entry_cb),pnl);
    etk_signal_connect("text-changed",ETK_OBJECT(pnl->entry_conn_mask),ETK_CALLBACK(wirelesspanel_textchanged_entry_cb),pnl);
    etk_signal_connect("text-changed",ETK_OBJECT(pnl->entry_conn_gateway),ETK_CALLBACK(wirelesspanel_textchanged_entry_cb),pnl);
    etk_signal_connect("toggled", ETK_OBJECT(pnl->check_static),ETK_CALLBACK(wirelesspanel_set_static_dhcp_clicked_cb), pnl);
    etk_signal_connect("toggled", ETK_OBJECT(pnl->check_dhcp),ETK_CALLBACK(wirelesspanel_set_static_dhcp_clicked_cb), pnl);
    etk_signal_connect("active-item-changed", ETK_OBJECT(pnl->cmbox_encryption), ETK_CALLBACK(wirelesspanel_cmboxencryption_active_item_changed_cb), pnl);


    return table;
}

void wirelesspanel_scanlist_row_clicked_cb(Etk_Object *object, Etk_Tree_Row *row, Etk_Event_Mouse_Up *event, void *data)
{
    Etk_Tree *tree;
    char *row_name;
    wireless_panel* pnl;
    if (!(tree = ETK_TREE(object)) || !row || !event || !data)
        return;

    pnl = (wireless_panel*)data;

    etk_tree_row_fields_get(row, etk_tree_nth_col_get(tree, 2),  &row_name, NULL);
    etk_entry_text_set(ETK_ENTRY(pnl->entry_conn_essid), row_name);
}

void wirelesspanel_scan_networks_cb(char* interface, Ecore_List* new_networks, Ecore_List* old_networks, void* data)
{
    wireless_panel* pnl;
    char encryption[] = PACKAGE_DATA_DIR ICONS_ENCRYPTION;
    char img1[] = PACKAGE_DATA_DIR ICONS_QUALITY_LESS_25;
    char img2[] = PACKAGE_DATA_DIR ICONS_QUALITY_LESS_50;
    char img3[] = PACKAGE_DATA_DIR ICONS_QUALITY_LESS_75;
    char img4[] = PACKAGE_DATA_DIR ICONS_QUALITY_LESS_100;
    char *img[4];
    char * essid;
    img[0] = img1;img[1] = img2;img[2] = img3;img[3] = img4;

    if( !interface || !new_networks || !interface || !new_networks)
    {
        print_error("ERROR", __FILE__, __LINE__,__func__,"interface = %p, old_networks = %p, new_networks = %p, data = %p",interface, new_networks, old_networks, data);
        return ;
    }

    pnl=(wireless_panel*)data;

    ecore_list_first_goto(new_networks);
    while( (essid = ecore_list_next(new_networks)))
    {
        etk_tree_row_append(ETK_TREE(pnl->scan_list), NULL,
                pnl->scan_quality,
                img[(exalt_dbus_wirelessinfo_get_quality(exalt_conn, interface, essid))/25],NULL,
                pnl->scan_encryption,
                (exalt_dbus_wirelessinfo_get_encryption(exalt_conn, interface, essid)?encryption:NULL)
                ,NULL,
                pnl->scan_essid,
                essid,NULL);
        if(essid && exalt_default_network && strcmp(essid,exalt_default_network)==0)
        {
            etk_entry_text_set(ETK_ENTRY(pnl->entry_conn_essid),exalt_default_network);
            wirelesspanel_textchanged_entry_cb(NULL,pnl);
        }
    }

    ecore_list_first_goto(old_networks);
    while( (essid = ecore_list_next(old_networks)))
    {
        Etk_Tree_Row* row = NULL;
        char* row_name;
        row = etk_tree_first_row_get(ETK_TREE(pnl->scan_list));

        while(row)
        {
            etk_tree_row_fields_get(row, pnl->scan_essid,  &row_name, NULL);
            if(strcmp(row_name,essid)==0)
            {
                etk_tree_row_delete(row);
                row = NULL;
            }
            else
                row = etk_tree_row_next_get(row);
        }
    }
}

void wirelesspanel_btn_disactivate_clicked_cb(void *data)
{
    wireless_panel* pnl;
    if(!data)
    {
        fprintf(stderr,"ethpanel_btn_disactivate_cb(): data== null!\n");
        return ;
    }

    pnl = (wireless_panel*)data;
    exalt_dbus_eth_down(exalt_conn,pnl->interface);
}

void wirelesspanel_btn_activate_clicked_cb(void *data)
{
    wireless_panel* pnl;
    if(!data)
    {
        fprintf(stderr,"ethpanel_btn_activate_cb(): data== null!\n");
        return ;
    }

    pnl = (wireless_panel*)data;
    exalt_dbus_eth_up(exalt_conn,pnl->interface);
}


void wirelesspanel_set_static_dhcp_clicked_cb(Etk_Object *object __UNUSED__, void *data)
{
    wireless_panel* pnl;

    if(!data)
    {
        perror("wirelesspanel_set_staticaddress_clicked_cb(): data==null ! \n");
        return ;
    }

    pnl = (wireless_panel*)data;
    wirelesspanel_disabled_widget_activate(pnl);
}

void wirelesspanel_cmboxencryption_active_item_changed_cb(Etk_Object *object __UNUSED__, void *data)
{
    wireless_panel* pnl;
    if(!data)
    {
        fprintf(stderr,"wirelesspanel_cmboxencryption_active_item_changed_cb(): data==null ! \n");
        return ;
    }

    pnl = (wireless_panel*)data;
    wirelesspanel_disabled_widget_activate(pnl);
}

void wirelesspanel_cmboxdriver_active_item_changed_cb(Etk_Object *object __UNUSED__, void *data)
{
    wireless_panel* pnl;
    const char* str;
    Etk_Combobox_Entry_Item *item;

    if(!data)
    {
        fprintf(stderr,"wirelesspanel_cmboxdriver_active_item_changed_cb(): data==null ! \n");
        return ;
    }

    pnl = (wireless_panel*)data;

    item = etk_combobox_entry_active_item_get(ETK_COMBOBOX_ENTRY(pnl->cmbox_driver));
    if(!item)
        return ;

    str = etk_combobox_entry_item_field_get(item, 0);
    if(!str)
        return ;
    etk_entry_text_set(ETK_ENTRY(etk_combobox_entry_entry_get(ETK_COMBOBOX_ENTRY(pnl->cmbox_driver))),str);
}

void wirelesspanel_btn_apply_clicked_cb(void *data)
{
    data=data;
    wireless_panel* pnl;
    int* encryption;
    int *mode;
    int *security;
    const char* driver;
    Etk_Combobox_Item *active_item;
    Exalt_Connection *c;

    if(!data)
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

    pnl = (wireless_panel*)data;

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

    if(!exalt_dbus_eth_apply_conn(exalt_conn, pnl->interface,c))
        return ;

    etk_widget_show_all(pnl->hbox_pbar);
    etk_widget_disabled_set(pnl->win->eth_list,ETK_TRUE);
    etk_widget_disabled_set(pnl->win->btn_mode,ETK_TRUE);
    etk_widget_disabled_set(pnl->btn_activate,ETK_TRUE);
    etk_widget_disabled_set(pnl->btn_apply,ETK_TRUE);
    pnl->pulsebar_timer = ecore_timer_add(APPLY_PULSE_TIMER ,wirelesspanel_apply_pulsebar_timer,pnl);
    wirelesspanel_disabled_widget_activate(pnl);
}


void wirelesspanel_textchanged_entry_cb(Etk_Object *object __UNUSED__, void *data __UNUSED__)
{
    wireless_panel* pnl;
    Etk_Combobox_Item* active_item;
    Exalt_Enum_Encryption_Mode * encryption_mode;
    const char* gateway;
    const char* essid;
    char* interface;

    if(!data)
        return ;

    pnl=(wireless_panel*) data;

    essid = etk_entry_text_get(ETK_ENTRY(pnl->entry_conn_essid));
    if(object && ETK_IS_ENTRY(object) && object == ETK_OBJECT(pnl->entry_conn_essid))
    {
        //load the default configuration
        Exalt_Connection *c;
        Etk_Combobox_Item* item;
        Exalt_Enum_Security_Mode security_mode;
        Exalt_Enum_Connection_Mode connection_mode;
        Exalt_Enum_Encryption_Mode encryption_mode;
        short dhcp;

        //try to find if the essid exist to load the default configuration
        interface = pnl->interface;
        essid = etk_entry_text_get(ETK_ENTRY(pnl->entry_conn_essid));

        c = exalt_dbus_wirelessinfo_get_default_conn(exalt_conn, interface, essid);
        encryption_mode = exalt_conn_get_encryption_mode(c);
        connection_mode = exalt_conn_get_connection_mode(c);
        security_mode = exalt_conn_get_security_mode(c);
        dhcp = exalt_conn_is_dhcp(c);

        if(dhcp)
        {
            etk_toggle_button_active_set(ETK_TOGGLE_BUTTON(pnl->check_static),ETK_FALSE);
            etk_toggle_button_active_set(ETK_TOGGLE_BUTTON(pnl->check_dhcp),ETK_TRUE);
            etk_entry_clear(ETK_ENTRY(pnl->entry_conn_ip));
            etk_entry_clear(ETK_ENTRY(pnl->entry_conn_gateway));
            etk_entry_clear(ETK_ENTRY(pnl->entry_conn_mask));
        }
        else
        {
            etk_toggle_button_active_set(ETK_TOGGLE_BUTTON(pnl->check_static),ETK_TRUE);
            etk_toggle_button_active_set(ETK_TOGGLE_BUTTON(pnl->check_dhcp),ETK_FALSE);
            etk_entry_text_set(ETK_ENTRY(pnl->entry_conn_ip),exalt_conn_get_ip(c));
            etk_entry_text_set(ETK_ENTRY(pnl->entry_conn_gateway),exalt_conn_get_gateway(c));
            etk_entry_text_set(ETK_ENTRY(pnl->entry_conn_mask),exalt_conn_get_netmask(c));
        }

        item = exalt_etk_combobox_data_item_get(ETK_COMBOBOX(pnl->cmbox_encryption),&encryption_mode);
        etk_combobox_active_item_set(ETK_COMBOBOX(pnl->cmbox_encryption),item);

        item = exalt_etk_combobox_data_item_get(ETK_COMBOBOX(pnl->cmbox_security),&security_mode);
        etk_combobox_active_item_set(ETK_COMBOBOX(pnl->cmbox_security),item);

        item = exalt_etk_combobox_data_item_get(ETK_COMBOBOX(pnl->cmbox_mode),&connection_mode);
        etk_combobox_active_item_set(ETK_COMBOBOX(pnl->cmbox_mode),item);

        if(encryption_mode != EXALT_ENCRYPTION_NONE)
            etk_entry_text_set(ETK_ENTRY(pnl->entry_conn_pwd),exalt_conn_get_key(c));
        else
            etk_entry_clear(ETK_ENTRY(pnl->entry_conn_pwd));
    }

    if (!(active_item = etk_combobox_active_item_get(ETK_COMBOBOX(pnl->cmbox_encryption))))
        return;

    //get the password mode
    encryption_mode = etk_combobox_item_data_get(active_item);
    if(!encryption_mode) { encryption_mode=malloc(sizeof(int)); *encryption_mode=EXALT_ENCRYPTION_NONE; }

    //verify if all entry contains a valid address && essid & passwd
    gateway = etk_entry_text_get(ETK_ENTRY(pnl->entry_conn_gateway));

    if( exalt_is_essid(essid)
            && exalt_is_key(etk_entry_text_get(ETK_ENTRY(pnl->entry_conn_pwd)),*encryption_mode)
            &&(
                etk_toggle_button_active_get(ETK_TOGGLE_BUTTON(pnl->check_dhcp))
                || (exalt_is_address(etk_entry_text_get(ETK_ENTRY(pnl->entry_conn_ip)))
                    && exalt_is_address(etk_entry_text_get(ETK_ENTRY(pnl->entry_conn_mask)))
                    && (exalt_is_address(gateway) || (gateway && strlen(gateway)==0 ))
                   )
              )
      )
        etk_widget_disabled_set(pnl->btn_apply,ETK_FALSE);
    else
        etk_widget_disabled_set(pnl->btn_apply, ETK_TRUE);
}


void wirelesspanel_conn_apply_done(wireless_panel* pnl)
{
    DELETE_TIMER(pnl->pulsebar_timer)
        wirelesspanel_disabled_widget_activate(pnl);
    etk_widget_disabled_set(pnl->win->eth_list,ETK_FALSE);
    etk_widget_disabled_set(pnl->win->btn_mode,ETK_FALSE);
    etk_widget_hide_all(pnl->pbar);
}


int wirelesspanel_apply_pulsebar_timer(void* data)
{
    wireless_panel* pnl = (wireless_panel*) data;
    etk_progress_bar_pulse(ETK_PROGRESS_BAR(pnl->pbar));
    return 1;
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

