/*
 * =====================================================================================
 *
 *       Filename:  boot_panel.h
 *
 *    Description:  the panel about the boot process
 *
 *        Version:  1.0
 *        Created:  12/03/2007 08:23:09 PM CET
 *       Revision:  none
 *       Compiler:  gcc
 *
 *         Author:   (Watchwolf),Atton Jonathan <watchwolf@watchwolf.fr>
 *        Company:
 *
 * =====================================================================================
 */

#ifndef  BOOT_PANEL_INC
#define  BOOT_PANEL_INC

typedef struct boot_panel boot_panel;

#include "main_window.h"

struct boot_panel
{
    main_window *win;
    Ewl_Widget* frame;

    Ecore_List* eth_list;
    Ewl_Widget* eth_tree;

    Ewl_Widget *seeker;
    Ewl_Widget *entry_seeker;
} ;

boot_panel* bootpanel_create();
void bootpanel_show(boot_panel* pnl);
void bootpanel_hide(boot_panel* pnl);
void bootpanel_free(boot_panel** pnl);

void bootpanel_update_timeout(boot_panel* pnl);

void bootpanel_check_clicked_cb(Ewl_Widget *w __UNUSED__,
        void *ev_data __UNUSED__,
        void *user_data);


void bootpanel_seeker_changed_cb(Ewl_Widget *w __UNUSED__,
        void *ev_data __UNUSED__,
        void *user_data __UNUSED__);



unsigned int bootpanel_eth_tree_data_count (void *data);
void* bootpanel_eth_tree_data_fetch (
        void        *data,
        unsigned int row,
        unsigned int column __UNUSED__);
void * bootpanel_eth_tree_header_data_fetch(void *data __UNUSED__, unsigned int column);
Ewl_Widget * bootpanel_eth_tree_widget_fetch(void *data, unsigned int row __UNUSED__,
        unsigned int column);


#endif   /* ----- #ifndef BOOT_PANEL_INC  ----- */

