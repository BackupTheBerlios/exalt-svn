
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
    ecore_list_free_cb_set(win->eth_list, free);

    win->eth_tree = ewl_tree_new();
    ewl_container_child_append(EWL_CONTAINER(scroll), win->eth_tree);
    ewl_object_alignment_set(EWL_OBJECT(win->eth_tree), EWL_FLAG_ALIGN_LEFT);
    ewl_object_fill_policy_set(EWL_OBJECT(win->eth_tree), EWL_FLAG_FILL_FILL);
    ewl_tree_column_count_set (EWL_TREE (win->eth_tree), 1);
    ewl_widget_show(win->eth_tree);
    ewl_callback_append(win->eth_tree, EWL_CALLBACK_VALUE_CHANGED,
            main_window_eth_tree_changed_cb, win);
    /* model */
    model = ewl_model_new();
    ewl_model_data_fetch_set (model,EWL_MODEL_DATA_FETCH (mainwindow_eth_tree_data_fetch));
    ewl_model_data_count_set (model,EWL_MODEL_DATA_COUNT (mainwindow_eth_tree_data_count));
    ewl_model_data_header_fetch_set(model, mainwindow_eth_tree_header_data_fetch);

    /* view */
    view = ewl_label_view_get();
    ewl_view_widget_fetch_set(view, mainwindow_eth_tree_widget_fetch);
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
    ewl_widget_hide(win->eth_panel->frame);

    win->wireless_panel = wirelesspanel_create(win);
    ewl_container_child_append(EWL_CONTAINER(hbox), win->wireless_panel->frame);
    ewl_widget_hide(win->wireless_panel->frame);

    win->general_panel = generalpanel_create(win);
    ewl_container_child_append(EWL_CONTAINER(hbox), win->general_panel->frame);

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

unsigned int mainwindow_eth_tree_data_count (void *data)
{
    if(!data)
        return 0;

    Ecore_List* l = (Ecore_List*)data;
    return ecore_list_count(l)+1;
}

void* mainwindow_eth_tree_data_fetch (
        void        *data,
        unsigned int row,
        unsigned int column __UNUSED__)
{
    if(!data)
        return 0;

    if(row==0)
        return _("General");
    else
    {
        Ecore_List* l = (Ecore_List*)data;
        ecore_list_index_goto(l,row-1);
        char* interface =  ecore_list_current(l);
        return interface;
    }
}


void * mainwindow_eth_tree_header_data_fetch(void *data __UNUSED__, unsigned int column)
{
    return _("Interfaces");
}


Ewl_Widget * mainwindow_eth_tree_widget_fetch(void *data, unsigned int row,
        unsigned int column __UNUSED__)
{
    Ewl_Widget *w_icon,*w_status,*w_label,*w_hbox;
    char* interface = (char*) data;
    const char* icon;
    const char* status;

    if(row==0)
    {
        icon = EWL_ICON_PREFERENCES_SYSTEM;
        status=NULL;
    }
    else
    {
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
            else if(exalt_dbus_eth_is_up(exalt_conn,interface)
                    && !exalt_dbus_eth_is_link(exalt_conn,interface))
                status = EWL_ICON_NETWORK_ERROR;
            else
                status= EWL_ICON_NETWORK_OFFLINE;
        }
    }


    icon = ewl_icon_theme_icon_path_get(icon, EWL_ICON_SIZE_LARGE);
    if(status)
        status = ewl_icon_theme_icon_path_get(status, EWL_ICON_SIZE_LARGE);

    w_hbox = ewl_hbox_new();
    w_icon = ewl_image_new();
    ewl_image_file_path_set(EWL_IMAGE(w_icon), icon);
    w_status = ewl_image_new();
    if(status)
        ewl_image_file_path_set(EWL_IMAGE(w_status), status);
    w_label = ewl_label_new();
    ewl_label_text_set(EWL_LABEL(w_label), interface);

    ewl_container_child_append(EWL_CONTAINER(w_hbox), w_icon);
    ewl_container_child_append(EWL_CONTAINER(w_hbox), w_status);
    ewl_container_child_append(EWL_CONTAINER(w_hbox), w_label);

    ewl_widget_show(w_hbox);
    ewl_widget_show(w_icon);
    ewl_widget_show(w_label);
    ewl_widget_show(w_status);

    return w_hbox;
}

int mainWindow_free(main_window** win)
{
    if(win && *win)
    {
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
        ewl_mvc_dirty_set(EWL_MVC(win->eth_tree),TRUE);
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
    else if(action == EXALTD_ETH_CB_WAITINGBOOT_CHANGE )
        ewl_mvc_dirty_set(EWL_MVC(win->general_panel->boot->eth_tree), TRUE);
    else if(action==EXALTD_ETH_CB_WAITINGBOOT_TIMEOUT_CHANGE)
        bootpanel_update_timeout(win->general_panel->boot);
}

void mainwindow_add_interface(char* interface, main_window* win)
{
    EXALT_ASSERT_RETURN_VOID(interface!=NULL);
    EXALT_ASSERT_RETURN_VOID(win!=NULL);

    ecore_list_append(win->eth_list, strdup(interface));

    ewl_mvc_dirty_set(EWL_MVC(win->eth_tree), TRUE);
    ewl_mvc_dirty_set(EWL_MVC(win->general_panel->boot->eth_tree), TRUE);

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
    char* elt;
    Ecore_List *l = win->eth_list;
    ecore_list_first_goto(l);

    while( (elt = (char*)ecore_list_next(l)) )
    {
        if(elt && strcmp(elt, interface)==0)
        {
            int index = ecore_list_index(l);
            ecore_list_index_goto(l,index-1);
            ecore_list_remove_destroy(l);
            ewl_mvc_dirty_set(EWL_MVC(win->general_panel->boot->eth_tree), TRUE);
            ewl_mvc_dirty_set(EWL_MVC(win->eth_tree), TRUE);
            return ;
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
                wirelesspanel_hide(win->wireless_panel);
                generalpanel_hide(win->general_panel);
                ethpanel_show(win->eth_panel);
            }
            else
            {
                wirelesspanel_set_eth(win->wireless_panel,val);
                ethpanel_hide(win->eth_panel);
                generalpanel_hide(win->general_panel);
                wirelesspanel_show(win->wireless_panel);
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

    generalpanel_update_advanced_mode(win->general_panel);
    ethpanel_update_advanced_mode(win->eth_panel);
    wirelesspanel_update_advanced_mode(win->wireless_panel);
}


