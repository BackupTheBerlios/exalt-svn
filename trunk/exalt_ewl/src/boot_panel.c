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
    Ewl_Widget *scroll,*vbox, *hbox,*text,*border;
    Ewl_Model *model;
    Ewl_View *view;

    pnl = malloc(sizeof(boot_panel));
    pnl->win = win;

    vbox = ewl_vbox_new();
    hbox = ewl_hbox_new();

    pnl->frame = hbox;
    ewl_container_child_append(EWL_CONTAINER(hbox), vbox);
    ewl_widget_show(vbox);
    ewl_widget_show(hbox);

    scroll = ewl_scrollpane_new();
    ewl_container_child_append(EWL_CONTAINER(vbox), scroll);
    ewl_widget_show(scroll);

    pnl->eth_list = ecore_list_new();

    pnl->eth_tree = ewl_tree_new();
    ewl_container_child_append(EWL_CONTAINER(scroll), pnl->eth_tree);
    //ewl_object_fill_policy_set(EWL_OBJECT(pnl->eth_tree), EWL_FLAG_FILL_FILL);
    ewl_tree_column_count_set (EWL_TREE (pnl->eth_tree), 1);
    ewl_widget_show(pnl->eth_tree);
    /* model */
    model = ewl_model_new();
    ewl_model_data_fetch_set (model,EWL_MODEL_DATA_FETCH (bootpanel_eth_tree_data_fetch));
    ewl_model_data_count_set (model,EWL_MODEL_DATA_COUNT (bootpanel_eth_tree_data_count));
    ewl_model_data_header_fetch_set(model, bootpanel_eth_tree_header_data_fetch);

    /* view */
    view = ewl_label_view_get();
    ewl_view_widget_fetch_set(view, bootpanel_eth_tree_widget_fetch);

    /* MVC */
    ewl_mvc_selection_mode_set (EWL_MVC (pnl->eth_tree), EWL_SELECTION_MODE_SINGLE);
    ewl_mvc_model_set (EWL_MVC (pnl->eth_tree), model);
    ewl_mvc_view_set (EWL_MVC(pnl->eth_tree), view);
    ewl_mvc_data_set(EWL_MVC(pnl->eth_tree),pnl->win->eth_list);


    border=ewl_border_new();
    text = ewl_text_new();
    ewl_text_styles_set(EWL_TEXT(text), EWL_TEXT_STYLE_GLOW);
    ewl_text_glow_color_set(EWL_TEXT(text), 230, 80, 5, 200);
    ewl_text_align_set(EWL_TEXT(text),EWL_FLAG_ALIGN_CENTER);

    ewl_text_text_append(EWL_TEXT(text),_("Help!"));

    ewl_text_styles_set(EWL_TEXT(text), EWL_TEXT_STYLE_NONE);
    ewl_text_text_append(EWL_TEXT(text),
            _("\n\nWhen your computer boot, exalt starts and \nconfigure yours networks interfaces in the \nbackground. If you use the DHCP, your interface \nwill get a IP address after a while."
                "\n\nYou can have a problem if you use a daemon as \nnfs-client. If the daemon need a network\n connection when it starts and your interface has no \nIP address, the daemon will not work. To avoid this \nproblem Exalt can configure your interface in the\n foreground. If you select the interface in the list,\n Exalt will wait until your interface gets a IP address."
                "\n\nIf the interface doesn't get a IP address before the\n timeout, your computer will continue the boot\n process."
             ));

    ewl_container_child_append(EWL_CONTAINER(border), text);
    ewl_container_child_append(EWL_CONTAINER(hbox), border);
    ewl_widget_show(text);
    ewl_widget_show(border);


    border=ewl_border_new();
    ewl_object_fill_policy_set(EWL_OBJECT(border),EWL_FLAG_FILL_SHRINK);
    ewl_border_label_set(EWL_BORDER(border),_("Timeout"));
    ewl_container_child_append(EWL_CONTAINER(vbox), border);

    hbox = ewl_hbox_new();
    ewl_container_child_append(EWL_CONTAINER(border), hbox);

    pnl->seeker = ewl_hseeker_new();
    ewl_container_child_append(EWL_CONTAINER(hbox), pnl->seeker);
    ewl_callback_append(pnl->seeker, EWL_CALLBACK_VALUE_CHANGED,
            bootpanel_seeker_changed_cb, pnl);


    pnl->entry_seeker = ewl_entry_new();
    ewl_container_child_append(EWL_CONTAINER(hbox), pnl->entry_seeker);
    ewl_object_fill_policy_set(EWL_OBJECT(pnl->entry_seeker),EWL_FLAG_FILL_SHRINK);
    ewl_entry_editable_set(EWL_ENTRY(pnl->entry_seeker),FALSE);

    ewl_widget_show(border);
    ewl_widget_show(hbox);
    ewl_widget_show(pnl->seeker);
    ewl_widget_show(pnl->entry_seeker);
    bootpanel_update_timeout(pnl);

    return pnl;
}

void bootpanel_free(boot_panel** pnl)
{
    if(pnl)
    {
        ecore_list_destroy((*pnl)->eth_list);
        EXALT_FREE(*pnl);
    }
}


void bootpanel_show(boot_panel* pnl)
{
    ewl_widget_show(pnl->frame);
}

void bootpanel_hide(boot_panel* pnl)
{
    ewl_widget_hide(pnl->frame);
}


void bootpanel_update_timeout(boot_panel* pnl)
{
    int timeout;
    char buf[1024];
    EXALT_ASSERT_RETURN_VOID(pnl!=NULL);

    timeout = exalt_dbus_bootprocess_timeout_get(exalt_conn);
    snprintf(buf,1024,"%d",timeout);
    ewl_text_text_set(EWL_TEXT(pnl->entry_seeker),buf);
    ewl_range_value_set(EWL_RANGE(pnl->seeker),timeout);
}

void bootpanel_check_clicked_cb(Ewl_Widget *w ,
        void *ev_data __UNUSED__,
        void *user_data)
{
    const char* eth = (char *)user_data;

    if(ewl_check_is_checked(EWL_CHECK(w)))
    {
        exalt_dbus_bootprocess_iface_add(exalt_conn, eth);
        ewl_check_checked_set(EWL_CHECK(w),FALSE);
    }
    else
    {
        exalt_dbus_bootprocess_iface_remove(exalt_conn,eth );
        ewl_check_checked_set(EWL_CHECK(w),TRUE);
    }

}


void bootpanel_seeker_changed_cb(Ewl_Widget *w __UNUSED__,
        void *ev_data __UNUSED__,
        void *user_data __UNUSED__)
{
    Ewl_Range *r;

    r = EWL_RANGE(w);
    exalt_dbus_bootprocess_timeout_set(exalt_conn,(int)ewl_range_value_get(r));
}



unsigned int bootpanel_eth_tree_data_count (void *data)
{
    if(!data)
        return 0;

    Ecore_List* l = (Ecore_List*)data;
    return ecore_list_count(l);
}


void* bootpanel_eth_tree_data_fetch (
        void        *data,
        unsigned int row,
        unsigned int column __UNUSED__)
{
    if(!data)
        return 0;

    Ecore_List* l = (Ecore_List*)data;
    ecore_list_index_goto(l,row);
    char * elt =  ecore_list_current(l);
    return elt;
}

void * bootpanel_eth_tree_header_data_fetch(void *data __UNUSED__, unsigned int column )
{
    return "Interfaces";
}

Ewl_Widget * bootpanel_eth_tree_widget_fetch(void *data, unsigned int row __UNUSED__,
        unsigned int column __UNUSED__)
{
    Ewl_Widget *w_hbox,*w_label,*w_check;
    char* eth = (char*)data;

    w_hbox = ewl_hbox_new();

    int check = exalt_dbus_bootprocess_iface_is(exalt_conn, eth);
    w_check = ewl_check_new();
    ewl_check_checked_set(EWL_CHECK(w_check),check);
    ewl_callback_append(w_check, EWL_CALLBACK_CLICKED,
            bootpanel_check_clicked_cb, eth);

    w_label = ewl_label_new();
    ewl_label_text_set(EWL_LABEL(w_label), eth);

    ewl_container_child_append(EWL_CONTAINER(w_hbox),w_check);
    ewl_container_child_append(EWL_CONTAINER(w_hbox),w_label);

    ewl_widget_show(w_hbox);
    ewl_widget_show(w_label);
    ewl_widget_show(w_check);
    return w_hbox;
}


