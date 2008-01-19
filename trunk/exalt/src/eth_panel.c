
#include "eth_panel.h"

eth_panel* ethpanel_create(main_window* win)
{
    Etk_Widget *hbox,*table,*label;
    eth_panel* pnl;
    pnl=(eth_panel*)malloc((unsigned int)sizeof(eth_panel));

    pnl->interface = NULL;
    pnl->win = win;
    pnl->frame = etk_frame_new("hehe");
    pnl->pulsebar_timer = NULL;

    pnl->box_configuration = etk_vbox_new(ETK_FALSE,5);
    etk_container_add(ETK_CONTAINER(pnl->frame), pnl->box_configuration);


    //#########################################
    //## box_configuration: box_activate ##
    //#########################################
    pnl->box_activate = etk_hbox_new(ETK_FALSE,5);
    etk_box_append(ETK_BOX(pnl->box_configuration), pnl->box_activate, ETK_BOX_START, ETK_BOX_FILL, 0);
    pnl->btn_activate = etk_button_new_with_label("Activate");
    pnl->btn_disactivate = etk_button_new_with_label("Disactivate");
    etk_box_append(ETK_BOX(pnl->box_activate), pnl->btn_disactivate, ETK_BOX_START, ETK_BOX_FILL, 0);
    etk_box_append(ETK_BOX(pnl->box_activate), pnl->btn_activate, ETK_BOX_START, ETK_BOX_FILL, 0);
    etk_signal_connect_swapped("clicked", ETK_OBJECT(pnl->btn_activate),ETK_CALLBACK(ethpanel_btn_activate_clicked_cb), pnl);
    etk_signal_connect_swapped("clicked", ETK_OBJECT(pnl->btn_disactivate),ETK_CALLBACK(ethpanel_btn_disactivate_clicked_cb), pnl);




    //###################################################
    //## box_configuration: check list (static / dhcp) ##
    //###################################################

    hbox = etk_hbox_new(ETK_FALSE, 5);
    etk_box_append(ETK_BOX(pnl->box_configuration), hbox, ETK_BOX_START, ETK_BOX_FILL, 0);

    pnl->check_static = etk_radio_button_new_with_label(_("Static address"), NULL);
    etk_box_append(ETK_BOX(hbox), pnl->check_static, ETK_BOX_START, ETK_BOX_FILL, 0);
    pnl->check_dhcp = etk_radio_button_new_with_label_from_widget(_("DHCP (Automatic configuration)"),ETK_RADIO_BUTTON(pnl->check_static));
    etk_box_append(ETK_BOX(hbox), pnl->check_dhcp, ETK_BOX_START, ETK_BOX_FILL, 0);
    etk_signal_connect("toggled", ETK_OBJECT(pnl->check_static),ETK_CALLBACK(ethpanel_set_static_dhcp_clicked_cb), pnl);
    etk_signal_connect("toggled", ETK_OBJECT(pnl->check_dhcp),ETK_CALLBACK(ethpanel_set_static_dhcp_clicked_cb), pnl);


    //###################################
    //## box_configuration: Entry list ##
    //###################################
    pnl->entry_ip = etk_entry_new();
    etk_entry_clear_button_add(ETK_ENTRY(pnl->entry_ip));
    pnl->entry_mask = etk_entry_new();
    etk_entry_clear_button_add(ETK_ENTRY(pnl->entry_mask));
    pnl->entry_gateway = etk_entry_new();
    etk_entry_clear_button_add(ETK_ENTRY(pnl->entry_gateway));


    etk_signal_connect("text-changed",ETK_OBJECT(pnl->entry_ip),ETK_CALLBACK(ethpanel_textchanged_entry_cb),pnl);
    etk_signal_connect("text-changed",ETK_OBJECT(pnl->entry_mask),ETK_CALLBACK(ethpanel_textchanged_entry_cb),pnl);
    etk_signal_connect("text-changed",ETK_OBJECT(pnl->entry_gateway),ETK_CALLBACK(ethpanel_textchanged_entry_cb),pnl);




    table = etk_table_new(2, 4, ETK_FALSE);

    label = etk_label_new(_("Ip address: "));
    etk_table_attach(ETK_TABLE(table), label, 0, 0, 0, 0, 0, 0, ETK_TABLE_HFILL);
    etk_table_attach_default(ETK_TABLE(table), pnl->entry_ip, 1, 1, 0, 0);

    label = etk_label_new(_("Network mask: "));
    etk_table_attach(ETK_TABLE(table), label, 0, 0, 1, 1, 0, 0, ETK_TABLE_HFILL);
    etk_table_attach_default(ETK_TABLE(table), pnl->entry_mask, 1, 1, 1, 1);

    label = etk_label_new(_("Gateway: "));
    etk_table_attach(ETK_TABLE(table), label, 0, 0, 2, 2, 0, 0, ETK_TABLE_HFILL);
    etk_table_attach_default(ETK_TABLE(table), pnl->entry_gateway, 1, 1, 2, 2);


    etk_box_append(ETK_BOX(pnl->box_configuration), table, ETK_BOX_START, ETK_BOX_FILL, 0);

    //#####################################
    //## box_configuration: Apply button ##
    //#####################################

    hbox = etk_hbox_new(ETK_FALSE, 5);
    etk_box_append(ETK_BOX(pnl->box_configuration), hbox, ETK_BOX_START, ETK_BOX_FILL, 0);
    pnl->btn_apply = etk_button_new_from_stock (ETK_STOCK_DIALOG_APPLY);
    etk_box_append(ETK_BOX(hbox), pnl->btn_apply, ETK_BOX_START, ETK_BOX_FILL, 0);
    etk_signal_connect_swapped("clicked", ETK_OBJECT(pnl->btn_apply),ETK_CALLBACK(ethpanel_btn_apply_clicked_cb), pnl);



    //########################################
    //## box_configuration: hbox_pbar	##
    //########################################

    pnl->hbox_pbar = etk_hbox_new(ETK_FALSE,5);
    pnl->pbar = etk_progress_bar_new_with_text(APPLY_TEXT);
    etk_progress_bar_pulse_step_set(ETK_PROGRESS_BAR(pnl->pbar), APPLY_PULSE);
    etk_box_append(ETK_BOX(pnl->box_configuration), pnl->hbox_pbar, ETK_BOX_START, ETK_BOX_EXPAND_FILL, 0);
    etk_box_append(ETK_BOX(pnl->hbox_pbar), pnl->pbar, ETK_BOX_START, ETK_BOX_EXPAND_FILL, 0);
    return pnl;
}

void ethpanel_free(eth_panel** pnl)
{
    if(pnl && *pnl)
    {
        EXALT_FREE((*pnl)->interface);
        EXALT_DELETE_TIMER((*pnl)->pulsebar_timer);
        EXALT_FREE(*pnl);
    }
}

void ethpanel_show(eth_panel* pnl)
{
    etk_widget_show_all(pnl->frame);
    etk_widget_hide(pnl->hbox_pbar);
}

void ethpanel_hide(eth_panel* pnl)
{
    etk_widget_hide_all(pnl->frame);
}

void ethpanel_set_eth(eth_panel* pnl, char* interface)
{
    char name[100];
    char *str;

    if(!pnl || !interface)
    {
        print_error( __FILE__, __func__,"pnl=%p and interface=%p",pnl,interface);
        return ;
    }
    sprintf(name,_("Network card: %s"),interface);

    EXALT_FREE(pnl->interface);
    pnl->interface = strdup(interface);

    etk_frame_label_set(ETK_FRAME(pnl->frame),name);
    str = exalt_dbus_eth_get_ip(exalt_conn,interface);
    etk_entry_text_set(ETK_ENTRY(pnl->entry_ip),str);
    EXALT_FREE(str);

    str = exalt_dbus_eth_get_netmask(exalt_conn, interface);
    etk_entry_text_set(ETK_ENTRY(pnl->entry_mask),str);
    EXALT_FREE(str);

    str = exalt_dbus_eth_get_gateway(exalt_conn, interface);
    etk_entry_text_set(ETK_ENTRY(pnl->entry_gateway),str);
    EXALT_FREE(str);

    if(exalt_dbus_eth_is_dhcp(exalt_conn, interface))
        etk_toggle_button_active_set(ETK_TOGGLE_BUTTON(pnl->check_dhcp),ETK_TRUE);
    else
        etk_toggle_button_active_set(ETK_TOGGLE_BUTTON(pnl->check_static),ETK_TRUE);

    ethpanel_disabled_set(pnl);
}

void ethpanel_set_static_dhcp_clicked_cb(Etk_Object *object __UNUSED__, void *data)
{
    eth_panel* pnl;

    if(!data)
    {
        perror("ethpanel_set_staticaddress_clicked_cb(): data==null ! \n");
        return ;
    }

    pnl = (eth_panel*)data;
    ethpanel_disabled_set(pnl);
    //call the function to set the button apply
    ethpanel_textchanged_entry_cb(NULL,pnl);
}

void ethpanel_textchanged_entry_cb(Etk_Object *object __UNUSED__, void *data)
{
    eth_panel* pnl;
    const char* gateway;
    if(!data)
        return ;

    pnl=(eth_panel*) data;

    gateway = etk_entry_text_get(ETK_ENTRY(pnl->entry_gateway));
    //verify if all entry contains a valid address
    if( etk_toggle_button_active_get(ETK_TOGGLE_BUTTON(pnl->check_dhcp))
            || (exalt_is_address(etk_entry_text_get(ETK_ENTRY(pnl->entry_ip)))
                && exalt_is_address(etk_entry_text_get(ETK_ENTRY(pnl->entry_mask)))
                && (exalt_is_address(gateway) || (gateway && strlen(gateway)==0)) )
      )
        etk_widget_disabled_set(pnl->btn_apply,ETK_FALSE);
    else
        etk_widget_disabled_set(pnl->btn_apply, ETK_TRUE);
}

void ethpanel_btn_disactivate_clicked_cb(void *data)
{
    eth_panel* pnl;
    if(!data)
    {
        fprintf(stderr,"ethpanel_btn_disactivate_cb(): data== null!\n");
        return ;
    }

    pnl = (eth_panel*)data;
    exalt_dbus_eth_down(exalt_conn,pnl->interface);
}

void ethpanel_btn_activate_clicked_cb(void *data)
{
    eth_panel* pnl;
    if(!data)
    {
        fprintf(stderr,"ethpanel_btn_activate_cb(): data== null!\n");
        return ;
    }

    pnl = (eth_panel*)data;
    exalt_dbus_eth_up(exalt_conn,pnl->interface);
}

void ethpanel_btn_apply_clicked_cb(void *data)
{
    data=data;
    eth_panel* pnl;
    Exalt_Connection *c;
    if(!data)
    {
        perror("ethpanel_btn_apply_clicked_cb(): data==null ! \n");
        return ;
    }

    c = exalt_conn_new();
    if(!c)
    {
        perror("ethpanel_btn_apply_clicked_cb(): c==null ! \n");
        return ;
    }

    pnl = (eth_panel*)data;

    if(etk_toggle_button_active_get(ETK_TOGGLE_BUTTON(pnl->check_static)))
    {
        exalt_conn_set_mode(c,EXALT_STATIC);
        exalt_conn_set_ip(c, etk_entry_text_get(ETK_ENTRY(pnl->entry_ip)));
        exalt_conn_set_netmask(c,etk_entry_text_get(ETK_ENTRY(pnl->entry_mask)));
        exalt_conn_set_gateway(c,etk_entry_text_get(ETK_ENTRY(pnl->entry_gateway)));
    }
    //else nothing
    //because exalt_conn_new is a dhcp connection by default

    if(!exalt_dbus_eth_apply_conn(exalt_conn, pnl->interface,c))
        return ;
    etk_widget_show(pnl->hbox_pbar);
    etk_widget_disabled_set(pnl->win->eth_list,ETK_TRUE);
    etk_widget_disabled_set(pnl->btn_disactivate,ETK_TRUE);
    pnl->pulsebar_timer = ecore_timer_add(APPLY_PULSE_TIMER ,ethpanel_apply_pulsebar_timer,pnl);
    ethpanel_disabled_set(pnl);
    exalt_conn_free(c);
}

void ethpanel_disabled_set(eth_panel* pnl)
{
    int apply = pnl->pulsebar_timer !=NULL;
    int down = !exalt_dbus_eth_is_up(exalt_conn,pnl->interface);

    etk_widget_disabled_set(pnl->btn_disactivate,ETK_FALSE);
    etk_widget_disabled_set(pnl->btn_activate,ETK_TRUE);
    etk_widget_disabled_set(pnl->check_static,ETK_FALSE);
    etk_widget_disabled_set(pnl->check_dhcp,ETK_FALSE);
    etk_widget_disabled_set(pnl->entry_ip,ETK_FALSE);
    etk_widget_disabled_set(pnl->entry_mask,ETK_FALSE);
    etk_widget_disabled_set(pnl->entry_gateway,ETK_FALSE);
    etk_widget_disabled_set(pnl->btn_apply,ETK_FALSE);

    if(!apply && (etk_toggle_button_active_get(ETK_TOGGLE_BUTTON(pnl->check_dhcp)) || down))
    {
        etk_widget_disabled_set(pnl->entry_ip,ETK_TRUE);
        etk_widget_disabled_set(pnl->entry_mask,ETK_TRUE);
        etk_widget_disabled_set(pnl->entry_gateway,ETK_TRUE);
    }

    if(!apply && down)
    {
        etk_widget_disabled_set(pnl->btn_disactivate,ETK_TRUE);
        etk_widget_disabled_set(pnl->btn_activate,ETK_FALSE);
        etk_widget_disabled_set(pnl->check_static,ETK_TRUE);
        etk_widget_disabled_set(pnl->check_dhcp,ETK_TRUE);
        etk_widget_disabled_set(pnl->btn_apply,ETK_TRUE);
    }

    if(apply)
    {
        etk_widget_disabled_set(pnl->btn_disactivate,ETK_TRUE);
        etk_widget_disabled_set(pnl->btn_activate,ETK_TRUE);
        etk_widget_disabled_set(pnl->check_static,ETK_TRUE);
        etk_widget_disabled_set(pnl->check_dhcp,ETK_TRUE);
        etk_widget_disabled_set(pnl->entry_ip,ETK_TRUE);
        etk_widget_disabled_set(pnl->entry_mask,ETK_TRUE);
        etk_widget_disabled_set(pnl->entry_gateway,ETK_TRUE);
        etk_widget_disabled_set(pnl->btn_apply,ETK_TRUE);
    }


    if(!exalt_dhcp_is_support())
        etk_widget_disabled_set(pnl->check_dhcp, ETK_TRUE);
}

void ethpanel_conn_apply_done(eth_panel* pnl)
{
    etk_widget_hide(pnl->hbox_pbar);
    DELETE_TIMER(pnl->pulsebar_timer);
    etk_widget_disabled_set(pnl->win->eth_list,ETK_FALSE);
    etk_widget_disabled_set(pnl->btn_disactivate,ETK_FALSE);

    ethpanel_disabled_set(pnl);
}

int ethpanel_apply_pulsebar_timer(void* data)
{
    eth_panel* pnl = (eth_panel*) data;
    etk_progress_bar_pulse(ETK_PROGRESS_BAR(pnl->pbar));
    return 1;
}

