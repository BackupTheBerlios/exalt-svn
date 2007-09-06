
#include "wireless_panel.h"



wireless_panel* wirelesspanel_create(main_window* win)
{
	Etk_Widget *hbox,*vbox,*scroll,*label, *table;
	wireless_panel* pnl;
	pnl=(wireless_panel*)malloc((unsigned int)sizeof(wireless_panel));
	pnl->win=win;
	pnl->eth = NULL;
        pnl->pulsebar_timer = NULL;

	pnl->frame = etk_frame_new("wireless_frame");

	hbox = etk_hbox_new(ETK_FALSE, 5);
	etk_container_add(ETK_CONTAINER(pnl->frame), hbox);


	pnl->box_button_off = etk_vbox_new(ETK_FALSE, 5);
	pnl->box_button_on = etk_hbox_new(ETK_FALSE, 5);

	etk_box_append(ETK_BOX(hbox), pnl->box_button_on, ETK_BOX_START, ETK_BOX_EXPAND_FILL, 0);
	etk_box_append(ETK_BOX(hbox), pnl->box_button_off, ETK_BOX_START, ETK_BOX_EXPAND_FILL, 0);


	//box radio button off
	vbox = pnl->box_button_off;
	label = etk_label_new(_("The wireless card is not activated"));
	etk_box_append(ETK_BOX(vbox), label, ETK_BOX_START, ETK_BOX_FILL, 0);
	label = etk_label_new(_("Maybe you have disable the card with a key (bindings)"));
	etk_box_append(ETK_BOX(vbox), label, ETK_BOX_START, ETK_BOX_FILL, 0);

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

	exalt_eth_set_scan_cb(wirelesspanel_scan_networks_cb,pnl);

	return pnl;
}



void wirelesspanel_show(wireless_panel* pnl)
{
	etk_widget_show_all(pnl->frame);
	wirelesspanel_set_boxbutton(pnl);
	etk_widget_hide(pnl->hbox_pbar);
}

void wirelesspanel_set_boxbutton(wireless_panel* pnl)
{
	if(!pnl)
	{
	 	fprintf(stderr,"wirelesspanel_set_boxbutton(): pnl=null ! \n");
		return ;
	}

	if(!pnl->eth)
	{
	 	fprintf(stderr,"wirelesspanel_set_boxbutton(): pnl->eth==null ! \n");
		return ;
	}

	if(!exalt_eth_is_wireless(pnl->eth))
	{
	 	fprintf(stderr,"wirelesspanel_set_boxbutton(): pnl->eth->wireless==null ! \n");
		return ;
	}

        //if(exalt_wireless_radiobutton_ison(pnl->eth->wireless))
	//{
		etk_widget_hide_all(pnl->box_button_off);
		etk_widget_show_all(pnl->box_button_on);
 	 	etk_widget_hide(pnl->hbox_pbar);

	 	exalt_wireless_scan_stop(pnl->eth);
		exalt_wireless_scan_start(pnl->eth);
		etk_tree_clear (ETK_TREE(pnl->scan_list));
		if(!exalt_eth_is_up(pnl->eth))
			wirelesspanel_disabled_widget_activate(pnl);
		else
			wirelesspanel_disabled_widget_activate(pnl);

	/*}
	else
	{
	 	exalt_wireless_scan_stop(pnl->eth);
		//the radio button is off
		etk_widget_hide_all(pnl->box_button_on);
		etk_widget_show_all(pnl->box_button_off);
	}*/
}


void wirelesspanel_disabled_widget_activate(wireless_panel* pnl)
{
 	Etk_Combobox_Item* active_item;
	int* encryption;
        int apply = pnl->pulsebar_timer != NULL;
        int down = !exalt_eth_is_up(pnl->eth);

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
            if(encryption && *encryption==EXALT_WIRELESS_ENCRYPTION_NONE)
            {
                etk_widget_disabled_set(pnl->entry_conn_pwd, ETK_TRUE);
                etk_widget_disabled_set(pnl->cmbox_security, ETK_TRUE);
            }
            else if(encryption && (*encryption == EXALT_WIRELESS_ENCRYPTION_WPA_PSK_CCMP_ASCII
                    || *encryption == EXALT_WIRELESS_ENCRYPTION_WPA_PSK_TKIP_ASCII
                    || *encryption == EXALT_WIRELESS_ENCRYPTION_WPA2_PSK_CCMP_ASCII
                    || *encryption == EXALT_WIRELESS_ENCRYPTION_WPA2_PSK_TKIP_ASCII))
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
 	exalt_wireless_scan_stop(pnl->eth);
}

void wirelesspanel_set_eth(wireless_panel* pnl, exalt_ethernet* eth)
{
	if(pnl && eth)
	{
		char name[100];
		sprintf(name,_("Wireless card: %s"),exalt_eth_get_name(eth));
		pnl->eth = eth;
		etk_frame_label_set(ETK_FRAME(pnl->frame),name);
		wirelesspanel_set_boxbutton(pnl);
	 	wirelesspanel_update_current_conf(pnl);
	}
}

void wirelesspanel_update_current_conf(wireless_panel* pnl)
{
	exalt_wireless* w;
	if(!pnl)
	{
	 	fprintf(stderr,"wirelesspanel_update_current_conf(): pnl==null! \n");
		return ;
	}
 	w = exalt_eth_get_wireless(pnl->eth);
	etk_label_set(ETK_LABEL(pnl->lbl_essid), exalt_wireless_get_essid(w));
	etk_label_set(ETK_LABEL(pnl->lbl_ip), exalt_eth_get_ip(pnl->eth));
	etk_label_set(ETK_LABEL(pnl->lbl_mask), exalt_eth_get_netmask(pnl->eth));
	etk_label_set(ETK_LABEL(pnl->lbl_gateway), exalt_eth_get_gateway(pnl->eth));
}

Etk_Widget* wirelesspanel_pageconnection_create(wireless_panel* pnl)
{
	Etk_Widget *table,*label;
	Etk_Combobox_Item* item;
	table = etk_table_new(2, 12,ETK_TABLE_NOT_HOMOGENEOUS);
 	int *en_none = malloc(sizeof(int));
	*en_none = EXALT_WIRELESS_ENCRYPTION_NONE;
	int *en_wep_ascii = malloc(sizeof(int));
	*en_wep_ascii = EXALT_WIRELESS_ENCRYPTION_WEP_ASCII;
	int *en_wep_hexa = malloc(sizeof(int));
	*en_wep_hexa = EXALT_WIRELESS_ENCRYPTION_WEP_HEXA;

        int *en_wpa_psk_ccmp = malloc(sizeof(int));
	*en_wpa_psk_ccmp = EXALT_WIRELESS_ENCRYPTION_WPA_PSK_CCMP_ASCII;
 	int *en_wpa_psk_tkip = malloc(sizeof(int));
	*en_wpa_psk_tkip = EXALT_WIRELESS_ENCRYPTION_WPA_PSK_TKIP_ASCII;

        int *en_wpa2_psk_ccmp = malloc(sizeof(int));
	*en_wpa2_psk_ccmp = EXALT_WIRELESS_ENCRYPTION_WPA2_PSK_CCMP_ASCII;
 	int *en_wpa2_psk_tkip = malloc(sizeof(int));
	*en_wpa2_psk_tkip = EXALT_WIRELESS_ENCRYPTION_WPA2_PSK_TKIP_ASCII;

        int *sec_open = malloc(sizeof(int));
        *sec_open = EXALT_WIRELESS_SECURITY_OPEN;
        int *sec_shared = malloc(sizeof(int));
        *sec_shared = EXALT_WIRELESS_SECURITY_SHARED;

        int *mode_adhoc = malloc(sizeof(int));
        *mode_adhoc = EXALT_WIRELESS_MODE_ADHOC;
	int *mode_managed = malloc(sizeof(int));
        *mode_managed = EXALT_WIRELESS_MODE_MANAGED;

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
	exalt_ethernet* eth;
	exalt_wireless_info * w;

	if (!(tree = ETK_TREE(object)) || !row || !event || !data)
		return;

	pnl = (wireless_panel*)data;

	etk_tree_row_fields_get(row, etk_tree_nth_col_get(tree, 2),  &row_name, NULL);
	eth = pnl->eth;
	w=exalt_wireless_get_networkinfo_by_essid(exalt_eth_get_wireless(eth),row_name);
	if(w)
	{
 	 	etk_entry_text_set(ETK_ENTRY(pnl->entry_conn_essid),exalt_wirelessinfo_get_essid(w));
	}
}

void wirelesspanel_scan_networks_cb(exalt_wireless_info* wi, int action, void* data)
{
	wireless_panel* pnl=(wireless_panel*)data;
	char encryption[] = PACKAGE_DATA_DIR ICONS_ENCRYPTION;
	char img1[] = PACKAGE_DATA_DIR ICONS_QUALITY_LESS_25;
	char img2[] = PACKAGE_DATA_DIR ICONS_QUALITY_LESS_50;
	char img3[] = PACKAGE_DATA_DIR ICONS_QUALITY_LESS_75;
	char img4[] = PACKAGE_DATA_DIR ICONS_QUALITY_LESS_100;
	char *img[4];
	img[0] = img1;img[1] = img2;img[2] = img3;img[3] = img4;


	if(action==EXALT_WIRELESS_SCAN_CB_ACTION_NEW)
	{
	 	etk_tree_row_append(ETK_TREE(pnl->scan_list), NULL,
					pnl->scan_quality,
					img[(exalt_wirelessinfo_get_quality(wi))/25],NULL,
					pnl->scan_encryption,
					(exalt_wirelessinfo_get_encryption(wi)?encryption:NULL)
					,NULL,
 	 	 	 	 	pnl->scan_essid,
					exalt_wirelessinfo_get_essid(wi),NULL);
	    if(exalt_default_network && strcmp(exalt_wirelessinfo_get_essid(wi),exalt_default_network)==0)
            {
                etk_entry_text_set(ETK_ENTRY(pnl->entry_conn_essid),exalt_default_network);
                wirelesspanel_textchanged_entry_cb(NULL,pnl);
            }
        }
        else if(action ==EXALT_WIRELESS_SCAN_CB_ACTION_REMOVE)
	{
		Etk_Tree_Row* row = NULL;
		char* row_name;
		row = etk_tree_first_row_get(ETK_TREE(pnl->scan_list));
		while(row)
		{
			etk_tree_row_fields_get(row, pnl->scan_essid,  &row_name, NULL);
			if(strcmp(row_name,exalt_wirelessinfo_get_essid(wi))==0)
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
	exalt_eth_down(pnl->eth);
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
	exalt_eth_up(pnl->eth);
}


void wirelesspanel_set_static_dhcp_clicked_cb(Etk_Object *object, void *data)
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

void wirelesspanel_cmboxencryption_active_item_changed_cb(Etk_Object *object, void *data)
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
		exalt_eth_set_new_dhcp(pnl->eth,EXALT_STATIC);
		//static mode
		exalt_eth_set_new_ip(pnl->eth,etk_entry_text_get(ETK_ENTRY(pnl->entry_conn_ip)));
		exalt_eth_set_new_netmask(pnl->eth,etk_entry_text_get(ETK_ENTRY(pnl->entry_conn_mask)));
		exalt_eth_set_new_gateway(pnl->eth,etk_entry_text_get(ETK_ENTRY(pnl->entry_conn_gateway)));
	}
	else
		exalt_eth_set_new_dhcp(pnl->eth,EXALT_DHCP);


	exalt_wireless_set_new_essid(exalt_eth_get_wireless(pnl->eth),etk_entry_text_get(ETK_ENTRY(pnl->entry_conn_essid)));

	//get the passwd mode
	if ((active_item = etk_combobox_active_item_get(ETK_COMBOBOX(pnl->cmbox_encryption))))
        {
            encryption = etk_combobox_item_data_get(active_item);
            exalt_wireless_set_new_passwd_mode(exalt_eth_get_wireless(pnl->eth),*encryption);
	    if(*encryption>EXALT_WIRELESS_ENCRYPTION_NONE)
	 	exalt_wireless_set_new_passwd(exalt_eth_get_wireless(pnl->eth),etk_entry_text_get(ETK_ENTRY(pnl->entry_conn_pwd)));
        }

        //get the mode
	if ((active_item = etk_combobox_active_item_get(ETK_COMBOBOX(pnl->cmbox_mode))))
        {
            mode = etk_combobox_item_data_get(active_item);
	    exalt_wireless_set_new_mode(exalt_eth_get_wireless(pnl->eth),*mode);
        }
            //get the security mode
	if ((active_item = etk_combobox_active_item_get(ETK_COMBOBOX(pnl->cmbox_security))))
        {
            security = etk_combobox_item_data_get(active_item);
	    exalt_wireless_set_new_security_mode(exalt_eth_get_wireless(pnl->eth),*security);
        }

	etk_widget_show_all(pnl->hbox_pbar);
        exalt_eth_apply_conf(pnl->eth,wirelesspanel_apply_applied_cb, pnl);

        etk_widget_disabled_set(pnl->win->eth_list,ETK_TRUE);
	etk_widget_disabled_set(pnl->btn_activate,ETK_TRUE);
	etk_widget_disabled_set(pnl->btn_apply,ETK_TRUE);
	pnl->pulsebar_timer = ecore_timer_add(APPLY_PULSE_TIMER ,wirelesspanel_apply_pulsebar_timer,pnl);
        wirelesspanel_disabled_widget_activate(pnl);
}


void wirelesspanel_textchanged_entry_cb(Etk_Object *object, void *data)
{
	wireless_panel* pnl;
	Etk_Combobox_Item* active_item;
	int * passwd_mode;
	char* gateway;
	exalt_wireless* w;
	exalt_wireless_info* wi;
	const char* essid;

	if(!data)
	 	return ;

	pnl=(wireless_panel*) data;

	if (!(active_item = etk_combobox_active_item_get(ETK_COMBOBOX(pnl->cmbox_encryption))))
		return;


	essid = etk_entry_text_get(ETK_ENTRY(pnl->entry_conn_essid));

	if(object && ETK_IS_ENTRY(object) && object == pnl->entry_conn_essid)
	{
		//try to find if the essid exist to load the default configuration
		w = exalt_eth_get_wireless(pnl->eth);
		if(!w)
		{
			fprintf(stderr,"wirelesspanel_textchanged_entry_cb(): the data is not a wireless interface ! \n");
			return ;
		}
		essid = etk_entry_text_get(ETK_ENTRY(pnl->entry_conn_essid));
		wi =  exalt_wireless_get_networkinfo_by_essid(w,essid);
                if(wi)
		{
			//load the default configuration
			int *passwd_mode = malloc(sizeof(int));
                        *passwd_mode = exalt_wirelessinfo_get_default_passwd_mode(wi);
 	 	 	int dhcp = exalt_wirelessinfo_is_default_dhcp(wi);
			int *security = malloc(sizeof(int));
                        *security = exalt_wirelessinfo_get_default_security_mode(wi);
                        int *mode = malloc(sizeof(int));
                        *mode = exalt_wirelessinfo_get_default_mode(wi);
                        Etk_Combobox_Item* item;
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
 	 	 	 	etk_entry_text_set(ETK_ENTRY(pnl->entry_conn_ip),exalt_wirelessinfo_get_default_ip(wi));
				etk_entry_text_set(ETK_ENTRY(pnl->entry_conn_gateway),exalt_wirelessinfo_get_default_gateway(wi));
				etk_entry_text_set(ETK_ENTRY(pnl->entry_conn_mask),exalt_wirelessinfo_get_default_netmask(wi));
			}


			item = exalt_etk_combobox_data_item_get(ETK_COMBOBOX(pnl->cmbox_encryption),passwd_mode);
			etk_combobox_active_item_set(ETK_COMBOBOX(pnl->cmbox_encryption),item);

                        item = exalt_etk_combobox_data_item_get(ETK_COMBOBOX(pnl->cmbox_security),security);
			etk_combobox_active_item_set(ETK_COMBOBOX(pnl->cmbox_security),item);

                	item = exalt_etk_combobox_data_item_get(ETK_COMBOBOX(pnl->cmbox_mode),mode);
			etk_combobox_active_item_set(ETK_COMBOBOX(pnl->cmbox_mode),item);

			if(*passwd_mode > EXALT_WIRELESS_ENCRYPTION_NONE)
				etk_entry_text_set(ETK_ENTRY(pnl->entry_conn_pwd),exalt_wirelessinfo_get_default_passwd(wi));
			else
				etk_entry_clear(ETK_ENTRY(pnl->entry_conn_pwd));

			return ;
		}
	}

 	//get the password mode
	passwd_mode = etk_combobox_item_data_get(active_item);
 	if(!passwd_mode) { passwd_mode=malloc(sizeof(int)); *passwd_mode=EXALT_WIRELESS_ENCRYPTION_NONE; }

	//verify if all entry contains a valid address && essid & passwd
	gateway = etk_entry_text_get(ETK_ENTRY(pnl->entry_conn_gateway));

	if( exalt_is_essid(essid)
	 	&& exalt_is_passwd(etk_entry_text_get(ETK_ENTRY(pnl->entry_conn_pwd)),*passwd_mode)
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


int wirelesspanel_apply_applied_cb(exalt_ethernet* eth,void* data)
{
	wireless_panel* pnl;

        pnl = (wireless_panel*) data;

        DELETE_TIMER(pnl->pulsebar_timer)

        //wirelesspanel_update_current_conf(pnl);

        wirelesspanel_disabled_widget_activate(pnl);
        etk_widget_disabled_set(pnl->win->eth_list,ETK_FALSE);
        etk_widget_hide_all(pnl->pbar);


        return 1;
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

