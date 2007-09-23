
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


	pnl->box_button_on = etk_hbox_new(ETK_FALSE, 5);

	etk_box_append(ETK_BOX(hbox), pnl->box_button_on, ETK_BOX_START, ETK_BOX_EXPAND_FILL, 0);


	//box radio button on
	hbox = pnl->box_button_on;

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
	etk_widget_show(scroll);

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
	etk_widget_show_all(pnl->frame);
	etk_widget_hide(pnl->hbox_pbar);
}

void wirelesspanel_set_boxbutton(wireless_panel* pnl)
{
    if(!pnl)
    {
        fprintf(stderr,"wirelesspanel_set_boxbutton(): pnl=null ! \n");
        return ;
    }

    if(!pnl->interface)
    {
        fprintf(stderr,"wirelesspanel_set_boxbutton(): pnl->interface==null ! \n");
        return ;
    }

    if(!exalt_dbus_eth_is_wireless(exalt_conn, pnl->interface))
    {
        fprintf(stderr,"wirelesspanel_set_boxbutton(): pnl->eth->wireless==null ! \n");
        return ;
    }

    etk_widget_show_all(pnl->box_button_on);
    etk_widget_hide(pnl->hbox_pbar);

    etk_tree_clear (ETK_TREE(pnl->scan_list));
    exalt_dbus_wireless_scan_stop(exalt_conn, pnl->interface);
    exalt_dbus_wireless_scan_start(exalt_conn,pnl->interface);

    if(!exalt_dbus_eth_is_up(exalt_conn, pnl->interface))
        wirelesspanel_disabled_widget_activate(pnl);
    else
        wirelesspanel_disabled_widget_activate(pnl);
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
            if(encryption && *encryption==EXALT_DBUS_WIRELESS_ENCRYPTION_NONE)
            {
                etk_widget_disabled_set(pnl->entry_conn_pwd, ETK_TRUE);
                etk_widget_disabled_set(pnl->cmbox_security, ETK_TRUE);
            }
            else if(encryption && (*encryption == EXALT_DBUS_WIRELESS_ENCRYPTION_WPA_PSK_CCMP_ASCII
                    || *encryption == EXALT_DBUS_WIRELESS_ENCRYPTION_WPA_PSK_TKIP_ASCII
                    || *encryption == EXALT_DBUS_WIRELESS_ENCRYPTION_WPA2_PSK_CCMP_ASCII
                    || *encryption == EXALT_DBUS_WIRELESS_ENCRYPTION_WPA2_PSK_TKIP_ASCII))
            {
                etk_widget_disabled_set(pnl->cmbox_mode, ETK_TRUE);
                etk_widget_disabled_set(pnl->cmbox_security, ETK_TRUE);
            }
        }

        wirelesspanel_textchanged_entry_cb(NULL,pnl);
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
    EXALT_DBUS_FREE(pnl->interface);
    pnl->interface = strdup(interface);
    etk_frame_label_set(ETK_FRAME(pnl->frame),name);
    wirelesspanel_set_boxbutton(pnl);
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
}

Etk_Widget* wirelesspanel_pageconnection_create(wireless_panel* pnl)
{
	Etk_Widget *table,*label;
	Etk_Combobox_Item* item;
	table = etk_table_new(2, 12,ETK_TABLE_NOT_HOMOGENEOUS);
 	int *en_none = malloc(sizeof(int));
	*en_none = EXALT_DBUS_WIRELESS_ENCRYPTION_NONE;
	int *en_wep_ascii = malloc(sizeof(int));
	*en_wep_ascii = EXALT_DBUS_WIRELESS_ENCRYPTION_WEP_ASCII;
	int *en_wep_hexa = malloc(sizeof(int));
	*en_wep_hexa = EXALT_DBUS_WIRELESS_ENCRYPTION_WEP_HEXA;

        int *en_wpa_psk_ccmp = malloc(sizeof(int));
	*en_wpa_psk_ccmp = EXALT_DBUS_WIRELESS_ENCRYPTION_WPA_PSK_CCMP_ASCII;
 	int *en_wpa_psk_tkip = malloc(sizeof(int));
	*en_wpa_psk_tkip = EXALT_DBUS_WIRELESS_ENCRYPTION_WPA_PSK_TKIP_ASCII;

        int *en_wpa2_psk_ccmp = malloc(sizeof(int));
	*en_wpa2_psk_ccmp = EXALT_DBUS_WIRELESS_ENCRYPTION_WPA2_PSK_CCMP_ASCII;
 	int *en_wpa2_psk_tkip = malloc(sizeof(int));
	*en_wpa2_psk_tkip = EXALT_DBUS_WIRELESS_ENCRYPTION_WPA2_PSK_TKIP_ASCII;

        int *sec_open = malloc(sizeof(int));
        *sec_open = EXALT_DBUS_WIRELESS_SECURITY_OPEN;
        int *sec_shared = malloc(sizeof(int));
        *sec_shared = EXALT_DBUS_WIRELESS_SECURITY_SHARED;

        int *mode_adhoc = malloc(sizeof(int));
        *mode_adhoc = EXALT_DBUS_WIRELESS_MODE_ADHOC;
	int *mode_managed = malloc(sizeof(int));
        *mode_managed = EXALT_DBUS_WIRELESS_MODE_MANAGED;

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



	etk_signal_connect("text-changed",ETK_OBJECT(pnl->entry_conn_essid),ETK_CALLBACK(wirelesspanel_textchanged_entry_cb),pnl);
	etk_signal_connect("text-changed",ETK_OBJECT(pnl->entry_conn_pwd),ETK_CALLBACK(wirelesspanel_textchanged_entry_cb),pnl);
	etk_signal_connect("text-changed",ETK_OBJECT(pnl->entry_conn_ip),ETK_CALLBACK(wirelesspanel_textchanged_entry_cb),pnl);
	etk_signal_connect("text-changed",ETK_OBJECT(pnl->entry_conn_mask),ETK_CALLBACK(wirelesspanel_textchanged_entry_cb),pnl);
	etk_signal_connect("text-changed",ETK_OBJECT(pnl->entry_conn_gateway),ETK_CALLBACK(wirelesspanel_textchanged_entry_cb),pnl);
	etk_signal_connect("toggled", ETK_OBJECT(pnl->check_static),ETK_CALLBACK(wirelesspanel_set_static_dhcp_clicked_cb), pnl);
	etk_signal_connect("toggled", ETK_OBJECT(pnl->check_dhcp),ETK_CALLBACK(wirelesspanel_set_static_dhcp_clicked_cb), pnl);
	etk_signal_connect("active-item-changed", ETK_OBJECT(pnl->cmbox_encryption), ETK_CALLBACK(wirelesspanel_cmboxencryption_active_item_changed_cb), pnl);


	etk_combobox_column_add(ETK_COMBOBOX(pnl->cmbox_encryption), ETK_COMBOBOX_LABEL, 50, ETK_TRUE, 0.5);
	etk_combobox_build(ETK_COMBOBOX(pnl->cmbox_encryption));
	item = etk_combobox_item_append(ETK_COMBOBOX(pnl->cmbox_encryption), WIRELESS_ENCRYPTION_TEXT_NONE);
	etk_combobox_item_data_set_full(item, en_none, free);
	item = etk_combobox_item_append(ETK_COMBOBOX(pnl->cmbox_encryption), WIRELESS_ENCRYPTION_TEXT_WEP_ASCII);
	etk_combobox_item_data_set_full(item, en_wep_ascii, free);
	item = etk_combobox_item_append(ETK_COMBOBOX(pnl->cmbox_encryption), WIRELESS_ENCRYPTION_TEXT_WEP_HEXA);
	etk_combobox_item_data_set_full(item, en_wep_hexa, free);
	item = etk_combobox_item_append(ETK_COMBOBOX(pnl->cmbox_encryption), WIRELESS_ENCRYPTION_TEXT_WPA_PSK_CCMP_ASCII);
 	etk_combobox_item_data_set_full(item, en_wpa_psk_ccmp, free);
	item = etk_combobox_item_append(ETK_COMBOBOX(pnl->cmbox_encryption), WIRELESS_ENCRYPTION_TEXT_WPA_PSK_TKIP_ASCII);
	etk_combobox_item_data_set_full(item, en_wpa_psk_tkip, free);
        item = etk_combobox_item_append(ETK_COMBOBOX(pnl->cmbox_encryption), WIRELESS_ENCRYPTION_TEXT_WPA2_PSK_CCMP_ASCII);
 	etk_combobox_item_data_set_full(item, en_wpa2_psk_ccmp, free);
	item = etk_combobox_item_append(ETK_COMBOBOX(pnl->cmbox_encryption), WIRELESS_ENCRYPTION_TEXT_WPA2_PSK_TKIP_ASCII);
	etk_combobox_item_data_set_full(item, en_wpa2_psk_tkip, free);


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


	etk_table_attach(ETK_TABLE(table), pnl->btn_apply, 0, 0, 9, 9, 0, 0, ETK_TABLE_HFILL);
	etk_table_attach_default(ETK_TABLE(table), pnl->hbox_pbar, 1, 1, 9, 9);



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


void wirelesspanel_btn_apply_clicked_cb(void *data)
{
	wireless_panel* pnl;
 	int* encryption;
        int *mode;
        int *security;
 	Etk_Combobox_Item *active_item;

	if(!data)
	{
		perror("wirelesspanel_btn_apply_clicked_cb(): data==null ! \n");
		return ;
	}

	pnl = (wireless_panel*)data;

	if(etk_toggle_button_active_get(ETK_TOGGLE_BUTTON(pnl->check_static)))
	{
		exalt_dbus_eth_set_new_dhcp(exalt_conn, pnl->interface,EXALT_DBUS_STATIC);
		//static mode
		exalt_dbus_eth_set_new_ip(exalt_conn,pnl->interface,etk_entry_text_get(ETK_ENTRY(pnl->entry_conn_ip)));
		exalt_dbus_eth_set_new_netmask(exalt_conn,pnl->interface,etk_entry_text_get(ETK_ENTRY(pnl->entry_conn_mask)));
		exalt_dbus_eth_set_new_gateway(exalt_conn,pnl->interface,etk_entry_text_get(ETK_ENTRY(pnl->entry_conn_gateway)));
	}
	else
		exalt_dbus_eth_set_new_dhcp(exalt_conn,pnl->interface,EXALT_DBUS_DHCP);


	exalt_dbus_wireless_set_new_essid(exalt_conn, pnl->interface ,etk_entry_text_get(ETK_ENTRY(pnl->entry_conn_essid)));

	//get the passwd mode
	if ((active_item = etk_combobox_active_item_get(ETK_COMBOBOX(pnl->cmbox_encryption))))
        {
            encryption = etk_combobox_item_data_get(active_item);
            exalt_dbus_wireless_set_new_passwd_mode(exalt_conn, pnl->interface,*encryption);
	    if(*encryption>EXALT_DBUS_WIRELESS_ENCRYPTION_NONE)
	 	exalt_dbus_wireless_set_new_passwd(exalt_conn, pnl->interface,etk_entry_text_get(ETK_ENTRY(pnl->entry_conn_pwd)));
        }

        //get the mode
	if ((active_item = etk_combobox_active_item_get(ETK_COMBOBOX(pnl->cmbox_mode))))
        {
            mode = etk_combobox_item_data_get(active_item);
	    exalt_dbus_wireless_set_new_mode(exalt_conn, pnl->interface ,*mode);
        }
            //get the security mode
	if ((active_item = etk_combobox_active_item_get(ETK_COMBOBOX(pnl->cmbox_security))))
        {
            security = etk_combobox_item_data_get(active_item);
	    exalt_dbus_wireless_set_new_security_mode(exalt_conn, pnl->interface,*security);
        }

        etk_widget_show_all(pnl->hbox_pbar);
        exalt_dbus_eth_apply_conf(exalt_conn, pnl->interface,wirelesspanel_apply_applied_cb, pnl);

        etk_widget_disabled_set(pnl->win->eth_list,ETK_TRUE);
	etk_widget_disabled_set(pnl->btn_activate,ETK_TRUE);
	etk_widget_disabled_set(pnl->btn_apply,ETK_TRUE);
	pnl->pulsebar_timer = ecore_timer_add(APPLY_PULSE_TIMER ,wirelesspanel_apply_pulsebar_timer,pnl);
        wirelesspanel_disabled_widget_activate(pnl);
}


void wirelesspanel_textchanged_entry_cb(Etk_Object *object __UNUSED__, void *data __UNUSED__)
{
	wireless_panel* pnl;
	Etk_Combobox_Item* active_item;
        int * passwd_mode;
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
            int *passwd_mode = malloc(sizeof(int));
            int *security = malloc(sizeof(int));
            int dhcp;
            int *mode = malloc(sizeof(int));
            Etk_Combobox_Item* item;

            //try to find if the essid exist to load the default configuration
            interface = pnl->interface;
            essid = etk_entry_text_get(ETK_ENTRY(pnl->entry_conn_essid));

            *passwd_mode = exalt_dbus_wirelessinfo_get_default_passwd_mode(exalt_conn, interface, essid);
            *mode = exalt_dbus_wirelessinfo_get_default_mode(exalt_conn, interface, essid);
            *security = exalt_dbus_wirelessinfo_get_default_security_mode(exalt_conn, interface, essid);
            dhcp = exalt_dbus_wirelessinfo_is_default_dhcp(exalt_conn, interface, essid);

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
                etk_entry_text_set(ETK_ENTRY(pnl->entry_conn_ip),exalt_dbus_wirelessinfo_get_default_ip(exalt_conn, interface, essid));
                etk_entry_text_set(ETK_ENTRY(pnl->entry_conn_gateway),exalt_dbus_wirelessinfo_get_default_gateway(exalt_conn, interface, essid));
                etk_entry_text_set(ETK_ENTRY(pnl->entry_conn_mask),exalt_dbus_wirelessinfo_get_default_netmask(exalt_conn, interface, essid));
            }


            item = exalt_etk_combobox_data_item_get(ETK_COMBOBOX(pnl->cmbox_encryption),passwd_mode);
            etk_combobox_active_item_set(ETK_COMBOBOX(pnl->cmbox_encryption),item);

            item = exalt_etk_combobox_data_item_get(ETK_COMBOBOX(pnl->cmbox_security),security);
            etk_combobox_active_item_set(ETK_COMBOBOX(pnl->cmbox_security),item);

            item = exalt_etk_combobox_data_item_get(ETK_COMBOBOX(pnl->cmbox_mode),mode);
            etk_combobox_active_item_set(ETK_COMBOBOX(pnl->cmbox_mode),item);

            if(*passwd_mode > EXALT_DBUS_WIRELESS_ENCRYPTION_NONE)
                etk_entry_text_set(ETK_ENTRY(pnl->entry_conn_pwd),exalt_dbus_wirelessinfo_get_default_passwd(exalt_conn, interface, essid));
            else
                etk_entry_clear(ETK_ENTRY(pnl->entry_conn_pwd));
        }

        if (!(active_item = etk_combobox_active_item_get(ETK_COMBOBOX(pnl->cmbox_encryption))))
            return;

        //get the password mode
        passwd_mode = etk_combobox_item_data_get(active_item);
        if(!passwd_mode) { passwd_mode=malloc(sizeof(int)); *passwd_mode=EXALT_DBUS_WIRELESS_ENCRYPTION_NONE; }

        //verify if all entry contains a valid address && essid & passwd
        gateway = etk_entry_text_get(ETK_ENTRY(pnl->entry_conn_gateway));

        if( exalt_dbus_is_essid(exalt_conn, essid)
                && exalt_dbus_is_passwd(exalt_conn, etk_entry_text_get(ETK_ENTRY(pnl->entry_conn_pwd)),*passwd_mode)

                &&(
                    etk_toggle_button_active_get(ETK_TOGGLE_BUTTON(pnl->check_dhcp))
                    || (exalt_dbus_is_address(exalt_conn, etk_entry_text_get(ETK_ENTRY(pnl->entry_conn_ip)))
                        && exalt_dbus_is_address(exalt_conn, etk_entry_text_get(ETK_ENTRY(pnl->entry_conn_mask)))
                        && (exalt_dbus_is_address(exalt_conn, gateway) || (gateway && strlen(gateway)==0 ))
                       )
                  )
          )
            etk_widget_disabled_set(pnl->btn_apply,ETK_FALSE);
        else
            etk_widget_disabled_set(pnl->btn_apply, ETK_TRUE);
}


void wirelesspanel_apply_applied_cb(char* interface __UNUSED__,void* data)
{
    wireless_panel* pnl;

    pnl = (wireless_panel*) data;

    DELETE_TIMER(pnl->pulsebar_timer)

        wirelesspanel_disabled_widget_activate(pnl);
    etk_widget_disabled_set(pnl->win->eth_list,ETK_FALSE);
    etk_widget_hide_all(pnl->pbar);
}


int wirelesspanel_apply_pulsebar_timer(void* data)
{
    wireless_panel* pnl = (wireless_panel*) data;
    etk_progress_bar_pulse(ETK_PROGRESS_BAR(pnl->pbar));
    return 1;
}


Etk_Combobox_Item * exalt_etk_combobox_data_item_get (Etk_Combobox *combobox, void *data)
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

