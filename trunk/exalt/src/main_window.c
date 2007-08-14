
#include "main_window.h"

main_window* mainwindow_create()
{
    main_window* win;

    win=(main_window*)malloc((unsigned int)sizeof(main_window));

    win->win = etk_window_new();
    etk_window_title_set(ETK_WINDOW(win->win), _("Exalt: network manager"));
    etk_window_wmclass_set(ETK_WINDOW(win->win),"Exalt_network_manager","Exalt_network_manager");
    etk_signal_connect("delete-event", ETK_OBJECT(win->win), ETK_CALLBACK( mainWindow_close), win);


    //test if we are in root mode or not
    if(exalt_is_admin())
        mainwindow_create_admin(win);
    else
        mainwindow_create_noadmin(win);

    return win;
}

void mainwindow_create_admin(main_window* win)
{
    Etk_Widget *hbox,*scroll;

    etk_window_resize(ETK_WINDOW(win->win), 623,268);

    hbox = etk_hbox_new(ETK_FALSE, 5);
    etk_container_add(ETK_CONTAINER(win->win), hbox);

    scroll = etk_scrolled_view_new ();
    etk_box_append(ETK_BOX(hbox), scroll, ETK_BOX_START, ETK_BOX_FILL, 0);
    etk_widget_show(scroll);

    win->eth_list = etk_tree_new();
    etk_widget_size_request_set(win->eth_list, 120, 50);

    win->eth_col0 = etk_tree_col_new(ETK_TREE(win->eth_list), _("Interfaces"), 80, 0.0);
    etk_tree_col_model_add(win->eth_col0, etk_tree_model_image_new());
    etk_tree_col_model_add(win->eth_col0, etk_tree_model_text_new());

    etk_tree_rows_height_set(ETK_TREE(win->eth_list),40);
    etk_tree_mode_set(ETK_TREE(win->eth_list), ETK_TREE_MODE_LIST);
    etk_tree_build(ETK_TREE(win->eth_list));
    etk_container_add(ETK_CONTAINER(scroll), win->eth_list);

    etk_signal_connect("row-clicked", ETK_OBJECT(win->eth_list),ETK_CALLBACK(mainWindow_ethList_row_clicked_cb), win);


    //attach the add callback function to exalt
    exalt_eth_set_cb(mainWindow_eth_cb, win);

    //add the about panel in the list
    etk_tree_row_append(ETK_TREE(win->eth_list), NULL,
            win->eth_col0,PACKAGE_DATA_DIR ICONS_ABOUT,NULL,_("About") , NULL);

    //add the general panel in the list
    etk_tree_row_append(ETK_TREE(win->eth_list), NULL,
            win->eth_col0,PACKAGE_DATA_DIR ICONS_NETWORK_CONFIG,NULL,_("General") , NULL);

    win->about_panel = aboutpanel_create();
    etk_box_append(ETK_BOX(hbox),win->about_panel->frame , ETK_BOX_START, ETK_BOX_EXPAND_FILL, 0);

    etk_widget_show_all(win->win);

    win->general_panel = generalpanel_create();
    etk_box_append(ETK_BOX(hbox),win->general_panel->frame , ETK_BOX_START, ETK_BOX_EXPAND_FILL, 0);

    win->eth_panel = ethpanel_create(win);
    etk_box_append(ETK_BOX(hbox),win->eth_panel->frame , ETK_BOX_START, ETK_BOX_EXPAND_FILL, 0);

    win->wireless_panel = wirelesspanel_create(win);
    etk_box_append(ETK_BOX(hbox),win->wireless_panel->frame , ETK_BOX_START, ETK_BOX_EXPAND_FILL, 0);
}


void mainwindow_create_noadmin(main_window* win)
{
    Etk_Widget *hbox;
    win->info_panel = infopanel_create();
    hbox = etk_hbox_new(ETK_FALSE,5);
    etk_container_add(ETK_CONTAINER(win->win), hbox);
    etk_box_append(ETK_BOX(hbox),win->info_panel->frame , ETK_BOX_START, ETK_BOX_EXPAND_FILL, 0);
    etk_widget_show_all(win->win);
    exalt_eth_set_cb(infopanel_eth_cb, win);
}




Etk_Bool mainWindow_free(main_window** win)
{
    if(win && *win)
    {
        etk_object_destroy(ETK_OBJECT((*win)->win));
        win = NULL;
        return 1;
    }
    else
        return 0;
}

Etk_Bool mainWindow_close(Etk_Object *object, void *data)
{
    exalt_eth_ethernets_free();
    main_window* win = (main_window*)data;
    etk_object_destroy(ETK_OBJECT(win->win));
    exit(1);
}

void mainWindow_eth_cb(exalt_ethernet* eth, int action, void* user_data)
{
    char wireless_img[] = PACKAGE_DATA_DIR ICONS_WIRELESS_ACTIVATE;
    char wireless_img_not_activate[] = PACKAGE_DATA_DIR ICONS_WIRELESS_NOT_ACTIVATE;
    char eth_img[] = PACKAGE_DATA_DIR ICONS_ETHERNET_ACTIVATE;
    char eth_img_not_activate[] = PACKAGE_DATA_DIR ICONS_ETHERNET_NOT_ACTIVATE;

    main_window* win =  (main_window*) user_data;

    if(action == EXALT_ETH_CB_ACTION_NEW || action == EXALT_ETH_CB_ACTION_ADD)
    {
        char* img;
        if(exalt_eth_is_wireless(eth))
        {
            if(exalt_eth_is_up(eth))
                img = wireless_img;
            else
                img = wireless_img_not_activate;
        }
        else if(exalt_eth_is_up(eth))
            img = eth_img;
        else
            img = eth_img_not_activate;
        etk_tree_row_append(ETK_TREE(win->eth_list), NULL,
                win->eth_col0,img,NULL,exalt_eth_get_name(eth) ,
                NULL);

        if(exalt_default_interface && strcmp(exalt_eth_get_name(eth),exalt_default_interface)==0)
        {
            if(!exalt_eth_is_wireless(eth))
            {
                ethpanel_set_eth(win->eth_panel,eth);
                wirelesspanel_hide(win->wireless_panel);
                generalpanel_hide(win->general_panel);
                aboutpanel_hide(win->about_panel);
                ethpanel_show(win->eth_panel);
            }
            else
            {
                wirelesspanel_set_eth(win->wireless_panel,eth);
                ethpanel_hide(win->eth_panel);
                generalpanel_hide(win->general_panel);
                wirelesspanel_show(win->wireless_panel);
                aboutpanel_hide(win->about_panel);
            }
        }
    }
    else if(action == EXALT_ETH_CB_ACTION_REMOVE)
    {
        Etk_Tree_Row* row;
        row = mainWindow_findrow(win, eth);
        if(row)
            etk_tree_row_delete(row);
    }
    else if(action == EXALT_ETH_CB_ACTION_UP || action == EXALT_ETH_CB_ACTION_DOWN || action == EXALT_ETH_CB_ACTION_LINK || action == EXALT_ETH_CB_ACTION_UNLINK )
    {
        Etk_Tree_Row *row;
        row = mainWindow_findrow(win, eth);
        if(row)
        {
            if(exalt_eth_is_wireless(eth))
            {
                if(exalt_eth_is_up(eth))
                    etk_tree_row_fields_set(row, 0, win->eth_col0,wireless_img,NULL,exalt_eth_get_name(eth), NULL);
                else
                    etk_tree_row_fields_set(row, 0, win->eth_col0,wireless_img_not_activate,NULL,exalt_eth_get_name(eth), NULL);
            }
            else
                if(exalt_eth_is_up(eth) && exalt_eth_is_link(eth))
                    etk_tree_row_fields_set(row, 0, win->eth_col0,eth_img,NULL,exalt_eth_get_name(eth), NULL);
                else
                    etk_tree_row_fields_set(row, 0, win->eth_col0,eth_img_not_activate,NULL,exalt_eth_get_name(eth), NULL);
        }

        //update the panel
        eth_panel* pnl = win->eth_panel;
        if(pnl->eth == eth)
            ethpanel_set_eth(pnl,eth);
        //update the wireless panel
        wireless_panel* wpnl = win->wireless_panel;
        if(wpnl->eth == eth)
            wirelesspanel_set_eth(wpnl,eth);
    }
    else if(action == EXALT_ETH_CB_ACTION_ADDRESS_NEW || action == EXALT_ETH_CB_ACTION_NETMASK_NEW || action == EXALT_ETH_CB_ACTION_GATEWAY_NEW)
    {
        //update the panel
        eth_panel* pnl = win->eth_panel;
        if(pnl->eth == eth)
            ethpanel_set_eth(pnl,eth);
        //update the wireless panel
        wireless_panel* wpnl = win->wireless_panel;
        if(wpnl->eth == eth)
            wirelesspanel_set_eth(wpnl,eth);
    }
    else if(action == EXALT_WIRELESS_CB_ACTION_ESSIDCHANGE)
    {
        //update the wireless panel
        wireless_panel* wpnl = win->wireless_panel;
        if(wpnl->eth == eth)
            wirelesspanel_set_eth(wpnl,eth);
    }
}


Etk_Tree_Row * mainWindow_findrow(main_window* win, exalt_ethernet* eth)
{
    Etk_Tree_Row* row;
    char* row_name;

    row = etk_tree_first_row_get(ETK_TREE(win->eth_list));
    //the first row is "General", we can jump it
    row = etk_tree_row_next_get(row);
    while(row)
    {
        etk_tree_row_fields_get(row, win->eth_col0, NULL, NULL, &row_name, NULL);
        if(strcmp(row_name,exalt_eth_get_name(eth))==0)
            return row;

        row = etk_tree_row_next_get(row);
    }

    return NULL;
}

int mainWindow_eth_state_timer(void* data)
{
    main_window* win = (main_window*)data;
    Etk_Tree_Row *row;
    char* row_name;
    exalt_ethernet* eth;

    row = etk_tree_first_row_get(ETK_TREE(win->eth_list));
    //the first row is "General", we can jump it
    row = etk_tree_row_next_get(row);
    while(row!=NULL)
    {
        etk_tree_row_fields_get(row, win->eth_col0, NULL, NULL, &row_name, NULL);
        eth=exalt_eth_get_ethernet_byname(row_name);
        if(eth)
        {
            if(exalt_eth_is_wireless(eth))
            {
                short radio;
                radio = exalt_wireless_radiobutton_ison(exalt_eth_get_wireless(eth));
                if(radio && exalt_eth_is_up(eth))
                    etk_tree_row_fields_set(row, 0, win->eth_col0,PACKAGE_DATA_DIR ICONS_WIRELESS_ACTIVATE,NULL,row_name, NULL);
                else
                    etk_tree_row_fields_set(row, 0, win->eth_col0,PACKAGE_DATA_DIR ICONS_WIRELESS_NOT_ACTIVATE,NULL,row_name, NULL);
            }
            else
                if(exalt_eth_is_up(eth))
                    etk_tree_row_fields_set(row, 0, win->eth_col0,PACKAGE_DATA_DIR ICONS_ETHERNET_ACTIVATE,NULL,row_name, NULL);
                else
                    etk_tree_row_fields_set(row, 0, win->eth_col0,PACKAGE_DATA_DIR ICONS_ETHERNET_NOT_ACTIVATE,NULL,row_name, NULL);
        }
        row = etk_tree_row_next_get(row);
    }
    return 1;
}


void mainWindow_ethList_row_clicked_cb(Etk_Object *object, Etk_Tree_Row *row, Etk_Event_Mouse_Up *event, void *data)
{
    Etk_Tree *tree;
    char *row_name;
    main_window* win;
    exalt_ethernet* eth;

    if (!(tree = ETK_TREE(object)) || !row || !event || !data)
        return;

    win = (main_window*)data;

    etk_tree_row_fields_get(row, etk_tree_nth_col_get(tree, 0), NULL, NULL, &row_name, NULL);

    if(strcmp(row_name,_("About")) == 0 )
    {
        wirelesspanel_hide(win->wireless_panel);
        ethpanel_hide(win->eth_panel);
        generalpanel_hide(win->general_panel);
        aboutpanel_show(win->about_panel);
    }
    else if(strcmp(row_name,_("General")) == 0 )
    {
        wirelesspanel_hide(win->wireless_panel);
        ethpanel_hide(win->eth_panel);
        generalpanel_show(win->general_panel);
        aboutpanel_hide(win->about_panel);
    }
    else
    {
        eth = exalt_eth_get_ethernet_byname(row_name);
        if(!exalt_eth_is_wireless(eth))
        {
            ethpanel_set_eth(win->eth_panel,eth);
            wirelesspanel_hide(win->wireless_panel);
            generalpanel_hide(win->general_panel);
            aboutpanel_hide(win->about_panel);
            ethpanel_show(win->eth_panel);
        }
        else
        {
            wirelesspanel_set_eth(win->wireless_panel,eth);
            ethpanel_hide(win->eth_panel);
            generalpanel_hide(win->general_panel);
            wirelesspanel_show(win->wireless_panel);
            aboutpanel_hide(win->about_panel);
        }
    }
}

