
#include "eth_panel.h"

eth_panel* ethpanel_create(main_window* win)
{
    Ewl_Widget *hbox,*grid;
    Ewl_Widget *label;
    eth_panel* pnl;
    pnl=(eth_panel*)malloc((unsigned int)sizeof(eth_panel));

    pnl->interface = NULL;
    pnl->win = win;
    pnl->frame = ewl_border_new();
    pnl->apply = 0;
    ewl_border_label_set(EWL_BORDER(pnl->frame),"hehe");
    ewl_widget_show(pnl->frame);

    pnl->box_configuration = ewl_vbox_new();
    ewl_container_child_append(EWL_CONTAINER(pnl->frame), pnl->box_configuration);
    ewl_widget_show(pnl->box_configuration);
    ewl_object_fill_policy_set(EWL_OBJECT(pnl->box_configuration), EWL_FLAG_FILL_HFILL);
    ewl_object_alignment_set(EWL_OBJECT(pnl->box_configuration),EWL_FLAG_ALIGN_LEFT);


    //#########################################
    //## box_configuration: box_activate ##
    //#########################################
    pnl->box_activate = ewl_hbox_new();
    ewl_container_child_append(EWL_CONTAINER(pnl->box_configuration), pnl->box_activate);
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
            ethpanel_btn_activate_clicked_cb, pnl);
    ewl_callback_append(pnl->btn_deactivate, EWL_CALLBACK_CLICKED,
            ethpanel_btn_deactivate_clicked_cb, pnl);



    //###################################################
    //## box_configuration: check list (static / dhcp) ##
    //###################################################

    hbox = ewl_hbox_new();
    ewl_container_child_append(EWL_CONTAINER(pnl->box_configuration), hbox);

    pnl->check_static = ewl_radiobutton_new();
    ewl_button_label_set(EWL_BUTTON(pnl->check_static),_("Static address"));
    pnl->check_dhcp = ewl_radiobutton_new();
    ewl_button_label_set(EWL_BUTTON(pnl->check_dhcp),_("DHCP (Automatic configuration)"));
    ewl_radiobutton_chain_set(EWL_RADIOBUTTON(pnl->check_dhcp),
                              EWL_RADIOBUTTON(pnl->check_static));
    ewl_container_child_append(EWL_CONTAINER(hbox), pnl->check_static);
    ewl_container_child_append(EWL_CONTAINER(hbox), pnl->check_dhcp);
    ewl_widget_show(hbox);
    ewl_widget_show(pnl->check_dhcp);
    ewl_widget_show(pnl->check_static);
    ewl_callback_append(pnl->check_static, EWL_CALLBACK_CLICKED,
            ethpanel_set_static_dhcp_clicked_cb, pnl);
    ewl_callback_append(pnl->check_dhcp, EWL_CALLBACK_CLICKED,
            ethpanel_set_static_dhcp_clicked_cb, pnl);


    //###################################
    //## box_configuration: Entry list ##
    //###################################
    pnl->entry_ip = ewl_entry_new();
    ewl_object_fill_policy_set(EWL_OBJECT(pnl->entry_ip), EWL_FLAG_FILL_FILL);
    ewl_object_alignment_set(EWL_OBJECT(pnl->entry_ip),EWL_FLAG_ALIGN_LEFT);

    pnl->entry_mask = ewl_entry_new();
    pnl->entry_gateway = ewl_entry_new();
    pnl->entry_cmd = ewl_entry_new();
    ewl_callback_append(pnl->entry_ip, EWL_CALLBACK_KEY_UP,
            ethpanel_textchanged_entry_cb, pnl);
    ewl_callback_append(pnl->entry_mask, EWL_CALLBACK_KEY_UP,
            ethpanel_textchanged_entry_cb, pnl);
    ewl_callback_append(pnl->entry_gateway, EWL_CALLBACK_KEY_UP,
            ethpanel_textchanged_entry_cb, pnl);

    grid = ewl_grid_new();
    ewl_container_child_append(EWL_CONTAINER(pnl->box_configuration), grid);
    ewl_object_fill_policy_set(EWL_OBJECT(grid), EWL_FLAG_FILL_HFILL);
    ewl_grid_dimensions_set(EWL_GRID(grid), 2, 5);
    ewl_grid_column_relative_w_set(EWL_GRID(grid), 0, 0.20);

    label = ewl_label_new();
    ewl_label_text_set(EWL_LABEL(label),_("Ip address: "));
    ewl_container_child_append(EWL_CONTAINER(grid), label);
    ewl_container_child_append(EWL_CONTAINER(grid), pnl->entry_ip);
    ewl_widget_show(label);
    ewl_widget_show(pnl->entry_ip);

    label = ewl_label_new();
    ewl_label_text_set(EWL_LABEL(label),_("Network mask: "));
    ewl_container_child_append(EWL_CONTAINER(grid), label);
    ewl_container_child_append(EWL_CONTAINER(grid), pnl->entry_mask);
    ewl_widget_show(label);
    ewl_widget_show(pnl->entry_mask);

    label = ewl_label_new();
    ewl_label_text_set(EWL_LABEL(label),_("Gateway: "));
    ewl_container_child_append(EWL_CONTAINER(grid), label);
    ewl_container_child_append(EWL_CONTAINER(grid), pnl->entry_gateway);
    ewl_widget_show(label);
    ewl_widget_show(pnl->entry_gateway);

    pnl->lbl_cmd = ewl_label_new();
    ewl_label_text_set(EWL_LABEL(pnl->lbl_cmd),_("Command: "));
    ewl_container_child_append(EWL_CONTAINER(grid), pnl->lbl_cmd);
    ewl_container_child_append(EWL_CONTAINER(grid), pnl->entry_cmd);
    ewl_widget_show(pnl->lbl_cmd);
    ewl_widget_show(pnl->entry_cmd);

    ewl_widget_show(grid);

    //#####################################
    //## box_configuration: Apply button ##
    //#####################################

    hbox = ewl_hbox_new();
    ewl_container_child_append(EWL_CONTAINER(pnl->box_configuration), hbox);
    pnl->btn_apply = ewl_button_new();
    ewl_stock_type_set(EWL_STOCK(pnl->btn_apply), EWL_STOCK_APPLY);
    ewl_object_fill_policy_set(EWL_OBJECT(pnl->btn_apply), EWL_FLAG_FILL_SHRINK);

    ewl_container_child_append(EWL_CONTAINER(hbox), pnl->btn_apply);
    ewl_widget_show(pnl->btn_apply);
    ewl_widget_show(hbox);

    ewl_callback_append(pnl->btn_apply, EWL_CALLBACK_CLICKED,
            ethpanel_btn_apply_clicked_cb, pnl);

    //########################################
    //## box_configuration: hbox_pbar	##
    //########################################

    pnl->hbox_pbar = ewl_hbox_new();
    pnl->pbar = ewl_progressbar_new();
    ewl_container_child_append(EWL_CONTAINER(pnl->box_configuration), pnl->hbox_pbar);
    ewl_container_child_append(EWL_CONTAINER(pnl->hbox_pbar), pnl->pbar);

    ewl_progressbar_label_set(EWL_PROGRESSBAR(pnl->pbar), APPLY_TEXT);
    ewl_range_unknown_set(EWL_RANGE(pnl->pbar), TRUE);

    ewl_widget_show(pnl->pbar);
    ewl_widget_show(pnl->hbox_pbar);

    return pnl;
}


void ethpanel_update_advanced_mode(eth_panel *pnl)
{
    if(!pnl->win->advanced_mode)
    {
        ewl_widget_hide(pnl->entry_cmd);
        ewl_widget_hide(pnl->lbl_cmd);
    }
    else
    {
        ewl_widget_show(pnl->entry_cmd);
        ewl_widget_show(pnl->lbl_cmd);
    }
}



void ethpanel_free(eth_panel** pnl)
{
    if(pnl && *pnl)
    {
        EXALT_FREE((*pnl)->interface);
        EXALT_FREE(*pnl);
    }
}

void ethpanel_show(eth_panel* pnl)
{
    ewl_widget_show(pnl->frame);
    ewl_widget_hide(pnl->hbox_pbar);
    ethpanel_update_advanced_mode(pnl);
}

void ethpanel_hide(eth_panel* pnl)
{
    ewl_widget_hide(pnl->frame);
}

void ethpanel_set_eth(eth_panel* pnl, char* interface)
{
    char name[100];
    char *str;

    EXALT_ASSERT_RETURN_VOID(pnl!=NULL);
    EXALT_ASSERT_RETURN_VOID(interface!=NULL);
    sprintf(name,_("Network card: %s"),interface);

    EXALT_FREE(pnl->interface);
    pnl->interface = strdup(interface);

    ewl_border_label_set(EWL_BORDER(pnl->frame),name);
    str = exalt_dbus_eth_get_ip(exalt_conn,interface);
    ewl_text_text_set(EWL_TEXT(pnl->entry_ip),str);
    EXALT_FREE(str);

    str = exalt_dbus_eth_get_netmask(exalt_conn, interface);
    ewl_text_text_set(EWL_TEXT(pnl->entry_mask),str);
    EXALT_FREE(str);

    str = exalt_dbus_eth_get_gateway(exalt_conn, interface);
    ewl_text_text_set(EWL_TEXT(pnl->entry_gateway),str);
    EXALT_FREE(str);

    if(exalt_dbus_eth_is_dhcp(exalt_conn, interface))
        ewl_radiobutton_checked_set(EWL_RADIOBUTTON(pnl->check_dhcp), TRUE);
    else
        ewl_radiobutton_checked_set(EWL_RADIOBUTTON(pnl->check_static), TRUE);

    str = exalt_dbus_eth_get_cmd(exalt_conn, interface);

    ewl_text_text_set(EWL_TEXT(pnl->entry_cmd),str);
    EXALT_FREE(str);

    ethpanel_disabled_set(pnl);
}

void ethpanel_set_static_dhcp_clicked_cb(Ewl_Widget *w __UNUSED__,
        void *ev_data __UNUSED__,
        void *data)
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
    ethpanel_textchanged_entry_cb(NULL,NULL,pnl);
}

void ethpanel_textchanged_entry_cb(Ewl_Widget *w __UNUSED__,
        void *ev_data __UNUSED__,
        void *data)
{
    eth_panel* pnl;
    const char* gateway,*ip,*mask;
    if(!data)
        return ;

    pnl=(eth_panel*) data;

    ip = ewl_text_text_get(EWL_TEXT(pnl->entry_ip));
    mask = ewl_text_text_get(EWL_TEXT(pnl->entry_mask));
    gateway = ewl_text_text_get(EWL_TEXT(pnl->entry_gateway));

    //verify if all entry contains a valid address
    if( ewl_radiobutton_is_checked(EWL_RADIOBUTTON(pnl->check_dhcp))
            || (exalt_is_address(ip)
                && exalt_is_address(mask)
                && (exalt_is_address(gateway) || !gateway) )
      )
        ewl_widget_enable(pnl->btn_apply);
    else
        ewl_widget_disable(pnl->btn_apply);
}

void ethpanel_btn_deactivate_clicked_cb(Ewl_Widget *w __UNUSED__,
        void *ev_data __UNUSED__,
        void *data)
{
    eth_panel* pnl;
    if(!data)
    {
        fprintf(stderr,"ethpanel_btn_deactivate_cb(): data== null!\n");
        return ;
    }

    pnl = (eth_panel*)data;
    exalt_dbus_eth_down(exalt_conn,pnl->interface);
}

void ethpanel_btn_activate_clicked_cb(Ewl_Widget *w __UNUSED__,
        void *ev_data __UNUSED__,
        void *data)
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

void ethpanel_btn_apply_clicked_cb(Ewl_Widget *w __UNUSED__,
        void *ev_data __UNUSED__,
        void *data)
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

    if(ewl_radiobutton_is_checked(EWL_RADIOBUTTON(pnl->check_static)))
    {
        exalt_conn_set_mode(c,EXALT_STATIC);
        exalt_conn_set_ip(c, ewl_text_text_get(EWL_TEXT(pnl->entry_ip)));
        exalt_conn_set_netmask(c,ewl_text_text_get(EWL_TEXT(pnl->entry_mask)));
        exalt_conn_set_gateway(c,ewl_text_text_get(EWL_TEXT(pnl->entry_gateway)));
    }
    //else nothing
    //because exalt_conn_new is a dhcp connection by default

    exalt_conn_set_cmd(c,ewl_text_text_get(EWL_TEXT(pnl->entry_cmd)));

    if(!exalt_dbus_eth_apply_conn(exalt_conn, pnl->interface,c))
        return ;
    ewl_widget_show(pnl->hbox_pbar);
    ewl_widget_disable(pnl->win->eth_tree);
    ewl_widget_disable(pnl->btn_deactivate);
    pnl->apply = 1;
    ethpanel_disabled_set(pnl);
    exalt_conn_free(c);
}

void ethpanel_disabled_set(eth_panel* pnl)
{
    int apply = pnl->apply;
    int down = !exalt_dbus_eth_is_up(exalt_conn,pnl->interface);

    ewl_widget_enable(pnl->btn_deactivate);
    ewl_widget_disable(pnl->btn_activate);
    ewl_widget_enable(pnl->check_static);
    ewl_widget_enable(pnl->check_dhcp);
    ewl_widget_enable(pnl->entry_ip);
    ewl_widget_enable(pnl->entry_mask);
    ewl_widget_enable(pnl->entry_gateway);
    ewl_widget_enable(pnl->btn_apply);
    ewl_widget_enable(pnl->entry_cmd);

    if(!apply && (ewl_radiobutton_is_checked(EWL_RADIOBUTTON(pnl->check_dhcp)) || down))
    {
        ewl_widget_disable(pnl->entry_ip);
        ewl_widget_disable(pnl->entry_mask);
        ewl_widget_disable(pnl->entry_gateway);
    }

    if(!apply && down)
    {
        ewl_widget_disable(pnl->btn_deactivate);
        ewl_widget_enable(pnl->btn_activate);
        ewl_widget_disable(pnl->check_static);
        ewl_widget_disable(pnl->check_dhcp);
        ewl_widget_disable(pnl->btn_apply);
        ewl_widget_disable(pnl->entry_cmd);
    }

    if(apply)
    {
        ewl_widget_disable(pnl->btn_deactivate);
        ewl_widget_disable(pnl->btn_activate);
        ewl_widget_disable(pnl->check_static);
        ewl_widget_disable(pnl->check_dhcp);
        ewl_widget_disable(pnl->entry_ip);
        ewl_widget_disable(pnl->entry_mask);
        ewl_widget_disable(pnl->entry_gateway);
        ewl_widget_disable(pnl->btn_apply);
        ewl_widget_disable(pnl->entry_cmd);
    }

    if(!exalt_dhcp_is_support())
        ewl_widget_disable(pnl->check_dhcp);
}

void ethpanel_conn_apply_done(eth_panel* pnl)
{
    ewl_widget_hide(pnl->hbox_pbar);
    ewl_widget_enable(pnl->win->eth_tree);
    ewl_widget_enable(pnl->btn_deactivate);
    pnl->apply = 0;
    ethpanel_disabled_set(pnl);
}


