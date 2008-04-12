
#include "main_window.h"

main_window* mainwindow_create()
{
    main_window* win;
    Ewl_Widget *hbox,*scroll,*vbox;
    Ewl_Model* model;
    Ewl_View *view;
    Ecore_List* interfaces;
    char* interface;

    win=(main_window*)malloc((unsigned int)sizeof(main_window));

    win->win = ewl_window_new();
    ewl_window_name_set(EWL_WINDOW(win->win), "Exalt_network_manager");
    ewl_window_title_set(EWL_WINDOW(win->win), "Exalt: network manager");
    ewl_window_class_set(EWL_WINDOW(win->win), "Exalt_network_manager");
    ewl_object_size_request(EWL_OBJECT(win->win), 650, 340);
    ewl_callback_append(win->win, EWL_CALLBACK_DELETE_WINDOW, mainWindow_close, win);

    hbox = ewl_hbox_new();
    ewl_container_child_append(EWL_CONTAINER(win->win), hbox);
    ewl_widget_show(hbox);

    vbox = ewl_vbox_new();
    ewl_container_child_append(EWL_CONTAINER(hbox), vbox);
    ewl_object_fill_policy_set(EWL_OBJECT(vbox),
                               EWL_FLAG_FILL_VFILL);
    ewl_object_alignment_set(EWL_OBJECT(vbox),
                             EWL_FLAG_ALIGN_LEFT);
    ewl_object_custom_w_set(EWL_OBJECT(vbox), 150);

    ewl_widget_show(vbox);


    scroll = ewl_scrollpane_new();
    ewl_container_child_append(EWL_CONTAINER(vbox), scroll);
    ewl_widget_show(scroll);

    win->eth_list = ecore_list_new();
    ecore_list_free_cb_set(win->eth_list, eth_elt_free);
    Eth_Elt *elt = malloc(sizeof(Eth_Elt));
    elt->name = _("General");
    elt->image = ewl_icon_theme_icon_path_get(EWL_ICON_PREFERENCES_SYSTEM, EWL_ICON_SIZE_LARGE) ;
    elt->status = NULL;
    ecore_list_append(win->eth_list,elt),

    win->eth_tree = ewl_tree_new();
    ewl_container_child_append(EWL_CONTAINER(scroll), win->eth_tree);
    ewl_object_alignment_set(EWL_OBJECT(win->eth_tree), EWL_FLAG_ALIGN_LEFT);
    ewl_object_fill_policy_set(EWL_OBJECT(win->eth_tree), EWL_FLAG_FILL_FILL);
    ewl_tree_column_count_set (EWL_TREE (win->eth_tree), 3);
    ewl_widget_show(win->eth_tree);
    ewl_callback_append(win->eth_tree, EWL_CALLBACK_VALUE_CHANGED,
            main_window_eth_tree_changed_cb, win);
    /* model */
    model = ewl_model_new();
    ewl_model_data_fetch_set (model,EWL_MODEL_DATA_FETCH (eth_ewl_package_tree_data_fetch));
    ewl_model_data_count_set (model,EWL_MODEL_DATA_COUNT (eth_ewl_package_tree_data_count));
    ewl_model_data_header_fetch_set(model, eth_ewl_cb_header_data_fetch);

    /* view */
    view = ewl_label_view_get();
    ewl_view_widget_fetch_set(view, eth_ewl_cb_widget_fetch);
    /* MVC */
    ewl_mvc_selection_mode_set (EWL_MVC (win->eth_tree), EWL_SELECTION_MODE_SINGLE);
    ewl_mvc_model_set (EWL_MVC (win->eth_tree), model);
    ewl_mvc_view_set (EWL_MVC(win->eth_tree), view);
    ewl_mvc_data_set (EWL_MVC(win->eth_tree), win->eth_list);


    win->advanced_mode = 0;
    win->btn_mode = ewl_button_new();
    ewl_button_label_set(EWL_BUTTON(win->btn_mode),_("Advanced"));
    ewl_container_child_append(EWL_CONTAINER(vbox), win->btn_mode);
    ewl_object_fill_policy_set(EWL_OBJECT(win->btn_mode), EWL_FLAG_FILL_HFILL);
    ewl_widget_show(win->btn_mode);
    ewl_callback_append(win->btn_mode, EWL_CALLBACK_CLICKED,
            main_window_btn_mode_clicked_cb, win);


    win->eth_panel = ethpanel_create(win);
    ewl_container_child_append(EWL_CONTAINER(hbox), win->eth_panel->frame);

/*

    win->general_panel = generalpanel_create(win);
    etk_box_append(ETK_BOX(hbox),win->general_panel->frame , ETK_BOX_START, ETK_BOX_EXPAND_FILL, 0);

    etk_widget_show_all(win->win);

    win->eth_panel = ethpanel_create(win);
    etk_box_append(ETK_BOX(hbox),win->eth_panel->frame , ETK_BOX_START, ETK_BOX_EXPAND_FILL, 0);

    win->wireless_panel = wirelesspanel_create(win);
    etk_box_append(ETK_BOX(hbox),win->wireless_panel->frame , ETK_BOX_START, ETK_BOX_EXPAND_FILL, 0);
*/
    //load the current interfaces
    exalt_dbus_notify_set(exalt_conn, mainwindow_notify_cb, win);

    interfaces = exalt_dbus_eth_get_list(exalt_conn);
    ecore_list_first_goto(interfaces);
    while ((interface = ecore_list_next(interfaces)))
    {
        mainwindow_add_interface(interface, win);
        //bootpanel_add_interface(interface,win->general_panel->boot);
    }
    ecore_list_destroy(interfaces);


    ewl_widget_show(win->win);
    return win;
}

unsigned int eth_ewl_package_tree_data_count (void *data)
{
    if(!data)
        return 0;

    Ecore_List* l = (Ecore_List*)data;
    return ecore_list_count(l);
}

void* eth_ewl_package_tree_data_fetch (
        void        *data,
        unsigned int row,
        unsigned int column)
{
    if(!data)
        return 0;

    Ecore_List* l = (Ecore_List*)data;
    ecore_list_index_goto(l,row);
    Eth_Elt * elt =  ecore_list_current(l);
    if(column==0)
        return elt->image;
    else if(column==1)
        return elt->name;
    else
        return elt->status;
}


void * eth_ewl_cb_header_data_fetch(void *data __UNUSED__, unsigned int column)
{
    if (column == 0)
        return "Type";
    else if(column == 1)
        return "Name";
    else
        return "Status";
}


Ewl_Widget * eth_ewl_cb_widget_fetch(void *data, unsigned int row __UNUSED__,
        unsigned int column)
{
    Ewl_Widget *w;
    if(column==1)
    {
        w = ewl_label_new();
        ewl_label_text_set(EWL_LABEL(w), data);
    }
    else
    {
        w = ewl_image_new();
        ewl_image_file_path_set(EWL_IMAGE(w), data);
    }
    return w;
}

Etk_Bool mainWindow_free(main_window** win)
{
    if(win && *win)
    {
        //ewl_object_destroy(ETK_OBJECT((*win)->win));
        /*ethpanel_free(&((*win)->eth_panel));
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

void mainWindow_close(Ewl_Widget *win __UNUSED__, void *ev __UNUSED__, void *data __UNUSED__)
{
    ewl_main_quit();
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
        //bootpanel_add_interface(interface,win->general_panel->boot);
    }
    else if(action == EXALT_ETH_CB_ACTION_REMOVE)
    {
        mainwindow_remove_interface(interface,win);
        //bootpanel_remove_interface(interface,win->general_panel->boot);
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
            /*etk_tree_row_fields_set(row,ETK_FALSE, win->eth_col0,
                    etk_theme_icon_path_get(),key_icon,
                    interface,
                    etk_theme_icon_path_get(),key_status, NULL);
                    */
        }

        //update the panel
        eth_panel* pnl = win->eth_panel;
        if( pnl->interface && interface && strcmp(pnl->interface, interface) == 0)
            ethpanel_set_eth(pnl,interface);
        //update the wireless panel
        /*wireless_panel* wpnl = win->wireless_panel;
        if( wpnl->interface && interface && strcmp(wpnl->interface, interface) == 0)
            wirelesspanel_set_eth(wpnl,interface);
            */
    }
    else if(action == EXALT_ETH_CB_ACTION_ADDRESS_NEW || action == EXALT_ETH_CB_ACTION_NETMASK_NEW || action == EXALT_ETH_CB_ACTION_GATEWAY_NEW)
    {
        //update the panel
        eth_panel* pnl = win->eth_panel;
        if( pnl->interface && interface && strcmp(pnl->interface, interface) == 0)
            ethpanel_set_eth(pnl,interface);
        //update the wireless panel
        /*wireless_panel* wpnl = win->wireless_panel;
        if( wpnl->interface && interface && strcmp(wpnl->interface, interface) == 0)
            wirelesspanel_set_eth(wpnl,interface);
            */
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
    const char* icon;
    const char* status;

    EXALT_ASSERT_RETURN_VOID(interface!=NULL);
    EXALT_ASSERT_RETURN_VOID(win!=NULL);

    if(exalt_dbus_eth_is_wireless(exalt_conn,interface))
    {
        icon = EWL_ICON_NETWORK_WIRELESS;
        if(exalt_dbus_eth_is_up(exalt_conn,interface))
            status = EWL_ICON_NETWORK_IDLE;
        else
            status = EWL_ICON_NETWORK_OFFLINE;
    }
    else
    {
        icon = EWL_ICON_NETWORK_WIRED;
        if(exalt_dbus_eth_is_up(exalt_conn,interface) && exalt_dbus_eth_is_link(exalt_conn,interface))
            status = EWL_ICON_NETWORK_IDLE;
        else if(exalt_dbus_eth_is_up(exalt_conn,interface) && !exalt_dbus_eth_is_link(exalt_conn,interface))
            status = EWL_ICON_NETWORK_ERROR;
        else
            status= EWL_ICON_NETWORK_OFFLINE;
    }

    Eth_Elt* elt = malloc(sizeof(Eth_Elt));
    elt->name = strdup(interface);
    icon =ewl_icon_theme_icon_path_get(icon, EWL_ICON_SIZE_LARGE) ;
    if(icon)
        elt->image = strdup(icon);
    else
        elt->image = NULL;
    status = ewl_icon_theme_icon_path_get(status, EWL_ICON_SIZE_SMALL);
    if(status)
        elt->status = strdup(status);
    else
        elt->status=NULL;
    ecore_list_append(win->eth_list, elt);

    ewl_mvc_dirty_set(EWL_MVC(win->eth_tree), TRUE);

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
    Eth_Elt* elt;
    Ecore_List *l = win->eth_list;
    ecore_list_first_goto(l);

    while( (elt = (Eth_Elt*)ecore_list_next(l)) )
    {
        if(elt->name && strcmp(elt->name, interface)==0)
        {
            int index = ecore_list_index(l);
            ecore_list_index_goto(l,index-1);
            ecore_list_remove_destroy(l);
            ewl_mvc_dirty_set(EWL_MVC(win->eth_tree), TRUE);
            return ;
        }
    }
}

void eth_elt_free(void* e)
{
    Eth_Elt* elt = (Eth_Elt*) e;
    if(!elt)
        return ;

    EXALT_FREE(elt->name);
    EXALT_FREE(elt->image);
    EXALT_FREE(elt->status);
    EXALT_FREE(elt);
}

Etk_Tree_Row * mainwindow_findrow(main_window* win, char* interface)
{
    Etk_Tree_Row* row;
    char* row_name;
    /*
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
*/
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

void main_window_eth_tree_changed_cb(Ewl_Widget *w, void *ev __UNUSED__, void *data)
{
    Ecore_List *selected;
    Ewl_Selection *sel;
    char* val;
    main_window *win = (main_window*)data;

    selected = ewl_mvc_selected_list_get(EWL_MVC(w));
    ecore_list_first_goto(selected);

    sel = ecore_list_next(selected);
    if(sel)
    {
        //sel->type == EWL_SELECTION_TYPE_INDEX
        val = sel->model->fetch(sel->data, EWL_SELECTION_IDX(sel)->row, 1);

        if(strcmp(val,_("General")) == 0 )
        {
            wirelesspanel_hide(win->wireless_panel);
            ethpanel_hide(win->eth_panel);
            generalpanel_show(win->general_panel);
        }
        else
        {
            if(!exalt_dbus_eth_is_wireless(exalt_conn, val))
            {
                ethpanel_set_eth(win->eth_panel,val);
                //wirelesspanel_hide(win->wireless_panel);
                //generalpanel_hide(win->general_panel);
                ethpanel_show(win->eth_panel);
            }
            else
            {
                //wirelesspanel_set_eth(win->wireless_panel,val);
                ethpanel_hide(win->eth_panel);
                //generalpanel_hide(win->general_panel);
                //wirelesspanel_show(win->wireless_panel);
            }
        }
    }
}

void main_window_btn_mode_clicked_cb(Ewl_Widget *w __UNUSED__,
        void *ev_data __UNUSED__,
        void *user_data)
{
    main_window* win = (main_window*) user_data;

    win->advanced_mode = !win->advanced_mode;
    if(win->advanced_mode)
        ewl_button_label_set(EWL_BUTTON(win->btn_mode),_("Basic"));
    else
        ewl_button_label_set(EWL_BUTTON(win->btn_mode),_("Advanced"));

    //generalpanel_update_advanced_mode(win->general_panel);
    ethpanel_update_advanced_mode(win->eth_panel);
    //wirelesspanel_update_advanced_mode(win->wireless_panel);

}
