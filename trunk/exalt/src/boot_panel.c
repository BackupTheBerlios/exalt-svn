/*
 * =====================================================================================
 *
 *       Filename:  boot_panel.c
 *
 *    Description: the apnel about the boot process
 *
 *        Version:  1.0
 *        Created:  12/03/2007 08:22:06 PM CET
 *       Revision:  none
 *       Compiler:  gcc
 *
 *         Author:   (Watchwolf), Atton Jonathan <watchwolf@watchwolf.fr>
 *        Company:
 *
 * =====================================================================================
 */

#include "boot_panel.h"



boot_panel* bootpanel_create(main_window *win)
{
    boot_panel* pnl;
    Etk_Widget *scroll,*vbox, *hbox,*text,*frame;

    pnl = malloc(sizeof(boot_panel));
    pnl->win = win;

    vbox = etk_vbox_new(ETK_FALSE, 5);
    hbox = etk_hbox_new(ETK_FALSE, 5);

    pnl->frame = hbox;
    etk_box_append(ETK_BOX(hbox), vbox, ETK_BOX_START, ETK_BOX_EXPAND_FILL, 0);

    scroll = etk_scrolled_view_new ();
    etk_box_append(ETK_BOX(vbox), scroll, ETK_BOX_START, ETK_BOX_EXPAND_FILL, 0);
    etk_widget_show(scroll);

    pnl->eth_list = etk_tree_new();
    pnl->eth_col0 = etk_tree_col_new(ETK_TREE(pnl->eth_list), _("Interfaces"), 80, 0.0);
    etk_tree_col_model_add(pnl->eth_col0, etk_tree_model_checkbox_new());
    etk_tree_col_model_add(pnl->eth_col0, etk_tree_model_text_new());

    etk_signal_connect_by_code(ETK_TREE_COL_CELL_VALUE_CHANGED_SIGNAL,
            ETK_OBJECT(pnl->eth_col0),
            ETK_CALLBACK(bootpanel_ethlist_checkbox_change_cb), pnl);

    etk_tree_mode_set(ETK_TREE(pnl->eth_list), ETK_TREE_MODE_LIST);
    etk_tree_build(ETK_TREE(pnl->eth_list));
    etk_container_add(ETK_CONTAINER(scroll), pnl->eth_list);

    text = etk_text_view_new();
    etk_widget_size_request_set(text, -1, 150);
    etk_object_properties_set(ETK_OBJECT(text), "focusable", ETK_FALSE, NULL);
    etk_textblock_text_set(ETK_TEXT_VIEW(text)->textblock,
            _("<p align=\"center\"><style effect=glow color1=#fa14 color2=#fe87><b>Help!</b></style>"
                "\n<p>When your computer boot, exalt starts and configure yours networks interfaces in the background. If you use the DHCP, your interface will get a IP address after a while.</p>"
                "\n<p>You can have a problem if you use a daemon as nfs-client. If the daemon need a network connection when it starts and your interface has no IP address, the daemon will not work. To avoid this problem Exalt can configure your interface in the foreground. If you select the interface in the list, Exalt will wait until your interface gets a IP address.</p>"
                "\n<p>If the interface doesn't get a IP address before the timeout, your computer will continue the boot process.</p>"
             ),
            ETK_TRUE);
    etk_box_append(ETK_BOX(hbox), text, ETK_BOX_START, ETK_BOX_EXPAND_FILL, 0);


    frame=etk_frame_new(_("Timeout"));
    etk_box_append(ETK_BOX(vbox), frame, ETK_BOX_START, ETK_BOX_FILL, 0);

    hbox = etk_hbox_new(ETK_FALSE, 5);
    etk_container_add(ETK_CONTAINER(frame), hbox);

    pnl->slider = etk_hslider_new(1,60,30,1,5);
    etk_box_append(ETK_BOX(hbox), pnl->slider, ETK_BOX_START, ETK_BOX_EXPAND_FILL, 0);
    etk_slider_label_set(ETK_SLIDER(pnl->slider), "%.0f");
    etk_signal_connect_by_code(ETK_RANGE_VALUE_CHANGED_SIGNAL, ETK_OBJECT(pnl->slider), ETK_CALLBACK(bootpanel_slider_value_changed_cb), pnl);
    etk_slider_update_policy_set(ETK_SLIDER(pnl->slider), ETK_SLIDER_DELAYED);

    bootpanel_update_timeout(pnl);

    return pnl;
}

void bootpanel_free(boot_panel** pnl)
{
    if(pnl)
        EXALT_FREE(*pnl);
}


void bootpanel_show(boot_panel* pnl)
{
    etk_widget_show_all(pnl->frame);
}

void bootpanel_hide(boot_panel* pnl)
{
    etk_widget_hide_all(pnl->frame);
}


void bootpanel_add_interface(char* interface, boot_panel* pnl)
{
    int checked;
    EXALT_ASSERT_RETURN_VOID(interface!=NULL);
    EXALT_ASSERT_RETURN_VOID(pnl!=NULL);

    checked = exalt_dbus_bootprocess_iface_is(exalt_conn, interface);
    etk_tree_row_append(ETK_TREE(pnl->eth_list), NULL,
            pnl->eth_col0,checked,interface ,
            NULL);
}

void bootpanel_update_interface(char* interface, boot_panel* pnl)
{
    int checked;
    EXALT_ASSERT_RETURN_VOID(interface!=NULL);
    EXALT_ASSERT_RETURN_VOID(pnl!=NULL);

    checked = exalt_dbus_bootprocess_iface_is(exalt_conn,interface);

    Etk_Tree_Row* row;
    row = bootpanel_findrow(interface,pnl);
    if(row)
        etk_tree_row_fields_set(row, ETK_FALSE, pnl->eth_col0, checked,interface,NULL);
}

void bootpanel_update_timeout(boot_panel* pnl)
{
    int timeout;
    EXALT_ASSERT_RETURN_VOID(pnl!=NULL);

    timeout = exalt_dbus_bootprocess_timeout_get(exalt_conn);
    etk_range_value_set(ETK_RANGE(pnl->slider),timeout);
}

void bootpanel_remove_interface(char* interface, boot_panel* pnl)
{
    EXALT_ASSERT_RETURN_VOID(pnl!=NULL);
    EXALT_ASSERT_RETURN_VOID(interface!=NULL);

    Etk_Tree_Row* row;
    row = bootpanel_findrow(interface,pnl);
    if(row)
        etk_tree_row_delete(row);
}

Etk_Tree_Row * bootpanel_findrow(char* interface, boot_panel* pnl)
{
    Etk_Tree_Row* row;
    char* row_name;

    EXALT_ASSERT_RETURN(interface!=NULL);
    EXALT_ASSERT_RETURN(pnl!=NULL);

    row = etk_tree_first_row_get(ETK_TREE(pnl->eth_list));
    while(row)
    {
        etk_tree_row_fields_get(row, pnl->eth_col0, NULL, &row_name, NULL);
        if(strcmp(row_name,interface)==0)
            return row;

        row = etk_tree_row_next_get(row);
    }

    return NULL;
}

Etk_Bool bootpanel_ethlist_checkbox_change_cb(Etk_Object *object, Etk_Tree_Row *row, void *data)
{
    Etk_Tree_Col* col;
    char* interface;
    Etk_Bool checked;
    boot_panel *pnl;

    pnl = (boot_panel*) data;
    col = ETK_TREE_COL(object);
    EXALT_ASSERT_RETURN(col!=NULL);
    EXALT_ASSERT_RETURN(row!=NULL);
    EXALT_ASSERT_RETURN(pnl!=NULL);

    etk_tree_row_fields_get(row, col, NULL, &interface, NULL);
    etk_tree_row_fields_get(row, col, &checked, NULL, NULL);

    //we change the state of the checkbox because it will be update when the daemon will update the configuration (see main_window.c::mainwindow_notify_cb())
    bootpanel_update_interface(interface, pnl);

    if(checked)
        exalt_dbus_bootprocess_iface_add(exalt_conn,interface);
    else
        exalt_dbus_bootprocess_iface_remove(exalt_conn,interface);

    return ETK_TRUE;
}

Etk_Bool bootpanel_slider_value_changed_cb(Etk_Object *object __UNUSED__, double value, void* data __UNUSED__)
{
    exalt_dbus_bootprocess_timeout_set(exalt_conn,(int)value);
    return 1;
}

