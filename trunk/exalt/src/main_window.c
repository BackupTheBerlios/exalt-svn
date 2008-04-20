
#include "main_window.h"

main_window* mainwindow_create()
{
    main_window* win;
    Etk_Widget *hbox,*scroll,*vbox;
    Ecore_List* interfaces;
    char* interface;

    win=(main_window*)malloc((unsigned int)sizeof(main_window));

    win->win = etk_window_new();
    etk_window_title_set(ETK_WINDOW(win->win), _("Exalt: network manager"));
    etk_window_wmclass_set(ETK_WINDOW(win->win),"Exalt_network_manager","Exalt_network_manager");
    etk_signal_connect("delete-event", ETK_OBJECT(win->win), ETK_CALLBACK( mainWindow_close), win);

    etk_window_resize(ETK_WINDOW(win->win), 650,340);

    hbox = etk_hbox_new(ETK_FALSE, 5);
    etk_container_add(ETK_CONTAINER(win->win), hbox);

    vbox = etk_vbox_new(ETK_FALSE, 5);
    etk_box_append(ETK_BOX(hbox), vbox, ETK_BOX_START, ETK_BOX_FILL, 0);


    scroll = etk_scrolled_view_new ();
    etk_box_append(ETK_BOX(vbox), scroll, ETK_BOX_START, ETK_BOX_EXPAND_FILL, 0);
    etk_widget_show(scroll);

    win->eth_list = etk_tree_new();
    etk_widget_size_request_set(win->eth_list, 130, 50);

    win->eth_col0 = etk_tree_col_new(ETK_TREE(win->eth_list), _("Interfaces"), 80, 0.0);
    etk_tree_col_model_add(win->eth_col0, etk_tree_model_image_new());
    etk_tree_col_model_add(win->eth_col0, etk_tree_model_text_new());
    etk_tree_col_model_add(win->eth_col0, etk_tree_model_image_new());

    etk_tree_rows_height_set(ETK_TREE(win->eth_list),55);
    etk_tree_mode_set(ETK_TREE(win->eth_list), ETK_TREE_MODE_LIST);
    etk_tree_build(ETK_TREE(win->eth_list));
    etk_container_add(ETK_CONTAINER(scroll), win->eth_list);

    etk_signal_connect("row-clicked", ETK_OBJECT(win->eth_list),ETK_CALLBACK(mainWindow_ethList_row_clicked_cb), win);

    //bouton to switch in advanced/basic mode
    win->advanced_mode = 0;
    win->btn_mode = etk_button_new_with_label(_("Advanced"));
    etk_box_append(ETK_BOX(vbox), win->btn_mode, ETK_BOX_START, ETK_BOX_FILL, 0);
    etk_signal_connect("clicked", ETK_OBJECT(win->btn_mode),ETK_CALLBACK(mainwindow_btn_mode_clicked_cb), win);


    //add the general panel in the list
    etk_tree_row_append(ETK_TREE(win->eth_list), NULL,
            win->eth_col0,etk_theme_icon_path_get(),etk_stock_key_get(ETK_STOCK_PREFERENCES_SYSTEM, ETK_STOCK_BIG),
            _("General") , NULL, NULL, NULL);

    win->general_panel = generalpanel_create(win);
    etk_box_append(ETK_BOX(hbox),win->general_panel->frame , ETK_BOX_START, ETK_BOX_EXPAND_FILL, 0);

    etk_widget_show_all(win->win);

    win->eth_panel = ethpanel_create(win);
    etk_box_append(ETK_BOX(hbox),win->eth_panel->frame , ETK_BOX_START, ETK_BOX_EXPAND_FILL, 0);

    win->wireless_panel = wirelesspanel_create(win);
    etk_box_append(ETK_BOX(hbox),win->wireless_panel->frame , ETK_BOX_START, ETK_BOX_EXPAND_FILL, 0);

    //load the current interfaces
    exalt_dbus_notify_set(exalt_conn, mainwindow_notify_cb, win);

    interfaces = exalt_dbus_eth_get_list(exalt_conn);
    ecore_list_first_goto(interfaces);
    while ((interface = ecore_list_next(interfaces)))
    {
        mainwindow_add_interface(interface, win);
        bootpanel_add_interface(interface,win->general_panel->boot);
    }
    ecore_list_destroy(interfaces);
    return win;
}

Etk_Bool mainWindow_free(main_window** win)
{
    if(win && *win)
    {
        /*etk_object_destroy(ETK_OBJECT((*win)->win));
        ethpanel_free(&((*win)->eth_panel));
          wirelesspanel_free(&((*win)->wireless_panel));
          generalpanel_free(&((*win)->general_panel));
          */
        EXALT_FREE(*win);
        *win = NULL;
        return 1;
    }
    else
        return 0;
}

Etk_Bool mainWindow_close(Etk_Object *object __UNUSED__, void *data __UNUSED__)
{
    ecore_main_loop_quit();
    return ETK_TRUE;
}

void mainwindow_notify_cb(char* interface, Exalt_Enum_Action action, void* user_data)
{
    int icon,status;
    const char* key_icon;
    const char *key_status;
    main_window* win =  (main_window*) user_data;

    if(action == EXALT_ETH_CB_ACTION_NEW || action == EXALT_ETH_CB_ACTION_ADD)
    {
        mainwindow_add_interface(interface,win);
        bootpanel_add_interface(interface,win->general_panel->boot);
    }
    else if(action == EXALT_ETH_CB_ACTION_REMOVE)
    {
        mainwindow_remove_interface(interface,win);
        bootpanel_remove_interface(interface,win->general_panel->boot);
    }
    else if(action == EXALT_ETH_CB_ACTION_UP || action == EXALT_ETH_CB_ACTION_DOWN || action == EXALT_ETH_CB_ACTION_LINK || action == EXALT_ETH_CB_ACTION_UNLINK )
    {
        Etk_Tree_Row *row;
        row = mainwindow_findrow(win, interface);
        if(row)
        {
            if(exalt_dbus_eth_is_wireless(exalt_conn,interface))
            {
                icon = ETK_STOCK_NETWORK_WIRELESS;
                if(exalt_dbus_eth_is_up(exalt_conn,interface))
                    status = ETK_STOCK_DIALOG_APPLY;
                else
                    status = ETK_STOCK_DIALOG_NO;
            }
            else
            {
                icon = ETK_STOCK_NETWORK_WIRED;
                if(exalt_dbus_eth_is_up(exalt_conn,interface) && exalt_dbus_eth_is_link(exalt_conn,interface))
                    status = ETK_STOCK_DIALOG_APPLY;
                else if(exalt_dbus_eth_is_up(exalt_conn,interface) && !exalt_dbus_eth_is_link(exalt_conn,interface))
                    status = ETK_STOCK_DIALOG_WARNING;
                else
                    status= ETK_STOCK_DIALOG_NO;
            }

            key_icon = etk_stock_key_get(icon, ETK_STOCK_BIG);
            key_status = etk_stock_key_get(status, ETK_STOCK_SMALL);
            etk_tree_row_fields_set(row,ETK_FALSE, win->eth_col0,
                    etk_theme_icon_path_get(),key_icon,
                    interface,
                    etk_theme_icon_path_get(),key_status, NULL);
        }

        //update the panel
        eth_panel* pnl = win->eth_panel;
        if( pnl->interface && interface && strcmp(pnl->interface, interface) == 0)
            ethpanel_set_eth(pnl,interface);
        //update the wireless panel
        wireless_panel* wpnl = win->wireless_panel;
        if( wpnl->interface && interface && strcmp(wpnl->interface, interface) == 0)
            wirelesspanel_set_eth(wpnl,interface);
    }
    else if(action == EXALT_ETH_CB_ACTION_ADDRESS_NEW || action == EXALT_ETH_CB_ACTION_NETMASK_NEW || action == EXALT_ETH_CB_ACTION_GATEWAY_NEW)
    {
        //update the panel
        eth_panel* pnl = win->eth_panel;
        if( pnl->interface && interface && strcmp(pnl->interface, interface) == 0)
            ethpanel_set_eth(pnl,interface);
        //update the wireless panel
        wireless_panel* wpnl = win->wireless_panel;
        if( wpnl->interface && interface && strcmp(wpnl->interface, interface) == 0)
            wirelesspanel_set_eth(wpnl,interface);
    }
    else if(action == EXALT_WIRELESS_CB_ACTION_ESSIDCHANGE)
    {
        //update the wireless panel
        wireless_panel* wpnl = win->wireless_panel;
        if(wpnl->interface && interface && strcmp(wpnl->interface, interface) == 0)
            wirelesspanel_set_eth(wpnl,interface);
    }
    else if(action == EXALT_ETH_CB_ACTION_CONN_APPLY_DONE)
    {
        if(exalt_dbus_eth_is_wireless(exalt_conn, interface))
            wirelesspanel_conn_apply_done(win->wireless_panel);
        else
            ethpanel_conn_apply_done(win->eth_panel);
    }
    else if(action == EXALTD_ETH_CB_WAITINGBOOT_CHANGE)
        bootpanel_update_interface(interface, win->general_panel->boot);
    else if(action == EXALTD_ETH_CB_WAITINGBOOT_TIMEOUT_CHANGE)
        bootpanel_update_timeout(win->general_panel->boot);
}

void mainwindow_add_interface(char* interface, main_window* win)
{
    int icon;
    int status;
    const char* key_icon, *key_status;

    EXALT_ASSERT_RETURN_VOID(interface!=NULL);
    EXALT_ASSERT_RETURN_VOID(win!=NULL);

        if(exalt_dbus_eth_is_wireless(exalt_conn,interface))
        {
            icon = ETK_STOCK_NETWORK_WIRELESS;
            if(exalt_dbus_eth_is_up(exalt_conn,interface))
                status = ETK_STOCK_DIALOG_APPLY;
            else
                status = ETK_STOCK_DIALOG_NO;
        }
        else
        {
            icon = ETK_STOCK_NETWORK_WIRED;
            if(exalt_dbus_eth_is_up(exalt_conn,interface) && exalt_dbus_eth_is_link(exalt_conn,interface))
                status = ETK_STOCK_DIALOG_APPLY;
            else if(exalt_dbus_eth_is_up(exalt_conn,interface) && !exalt_dbus_eth_is_link(exalt_conn,interface))
                status = ETK_STOCK_DIALOG_WARNING;
            else
                status= ETK_STOCK_DIALOG_NO;
        }
    key_icon = etk_stock_key_get(icon, ETK_STOCK_BIG);
    key_status = etk_stock_key_get(status, ETK_STOCK_SMALL);
    etk_tree_row_append(ETK_TREE(win->eth_list), NULL,win->eth_col0,
            etk_theme_icon_path_get(),key_icon,
            interface,
            etk_theme_icon_path_get(),key_status,NULL);

    if(exalt_default_interface && strcmp(interface,exalt_default_interface)==0)
    {
        if(!exalt_dbus_eth_is_wireless(exalt_conn,interface))
        {
            ethpanel_set_eth(win->eth_panel,interface);
            wirelesspanel_hide(win->wireless_panel);
            generalpanel_hide(win->general_panel);
            ethpanel_show(win->eth_panel);
        }
        else
        {
            wirelesspanel_set_eth(win->wireless_panel,interface);
            ethpanel_hide(win->eth_panel);
            generalpanel_hide(win->general_panel);
            wirelesspanel_show(win->wireless_panel);
        }
    }
}

void mainwindow_remove_interface(char* interface, main_window* win)
{
    EXALT_ASSERT_RETURN_VOID(interface!=NULL);
    EXALT_ASSERT_RETURN_VOID(win!=NULL);


        Etk_Tree_Row* row;
    row = mainwindow_findrow(win, interface);
    if(row)
        etk_tree_row_delete(row);
}

Etk_Tree_Row * mainwindow_findrow(main_window* win, char* interface)
{
    Etk_Tree_Row* row;
    char* row_name;

    row = etk_tree_first_row_get(ETK_TREE(win->eth_list));
    //the first row is "General", we can jump it
    row = etk_tree_row_next_get(row);
    while(row)
    {
        etk_tree_row_fields_get(row, win->eth_col0, NULL, NULL, &row_name, NULL, NULL, NULL);
        if(strcmp(row_name,interface)==0)
            return row;

        row = etk_tree_row_next_get(row);
    }

    return NULL;
}

void mainWindow_ethList_row_clicked_cb(Etk_Object *object, Etk_Tree_Row *row, Etk_Event_Mouse_Up *event, void *data)
{
    Etk_Tree *tree;
    char *row_name;
    main_window* win;

    if (!(tree = ETK_TREE(object)) || !row || !event || !data)
        return;

    win = (main_window*)data;

    etk_tree_row_fields_get(row, etk_tree_nth_col_get(tree, 0), NULL, NULL, &row_name, NULL, NULL, NULL);

    if(strcmp(row_name,_("General")) == 0 )
    {
        wirelesspanel_hide(win->wireless_panel);
        ethpanel_hide(win->eth_panel);
        generalpanel_show(win->general_panel);
    }
    else if(strcmp(row_name,_("Boot process")) == 0 )
    {
        wirelesspanel_hide(win->wireless_panel);
        ethpanel_hide(win->eth_panel);
        generalpanel_hide(win->general_panel);
    }
    else
    {
        if(!exalt_dbus_eth_is_wireless(exalt_conn, row_name))
        {
            ethpanel_set_eth(win->eth_panel,row_name);
            wirelesspanel_hide(win->wireless_panel);
            generalpanel_hide(win->general_panel);
            ethpanel_show(win->eth_panel);
        }
        else
        {
            wirelesspanel_set_eth(win->wireless_panel,row_name);
            ethpanel_hide(win->eth_panel);
            generalpanel_hide(win->general_panel);
            wirelesspanel_show(win->wireless_panel);
        }
    }
}

void mainwindow_btn_mode_clicked_cb(Etk_Object *object , void *data)
{
    main_window* win = (main_window*) data;
    Etk_Widget *btn = ETK_WIDGET(object);
    win->advanced_mode = !win->advanced_mode;

    if(win->advanced_mode)
        etk_button_label_set(ETK_BUTTON(btn),_("Basic"));
    else
        etk_button_label_set(ETK_BUTTON(btn),_("Advanced"));

    generalpanel_update_advanced_mode(win->general_panel);
    ethpanel_update_advanced_mode(win->eth_panel);
    wirelesspanel_update_advanced_mode(win->wireless_panel);
}


