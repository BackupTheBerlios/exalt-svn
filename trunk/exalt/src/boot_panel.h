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
#include <Etk.h>

struct boot_panel
{
    main_window *win;
    Etk_Widget* frame;

    Etk_Widget* eth_list;
    Etk_Tree_Col* eth_col0;

    Etk_Widget *slider;
    Etk_Widget *slider_entry;
} ;

boot_panel* bootpanel_create();
void bootpanel_show(boot_panel* pnl);
void bootpanel_hide(boot_panel* pnl);
void bootpanel_free(boot_panel** pnl);

void bootpanel_update_interface(char* interface, boot_panel* pnl);
void bootpanel_add_interface(char* interface, boot_panel* pnl);
void bootpanel_remove_interface(char* interface, boot_panel* pnl);
Etk_Tree_Row * bootpanel_findrow(char* interface, boot_panel* pnl);

Etk_Bool bootpanel_ethlist_checkbox_change_cb(Etk_Object *object, Etk_Tree_Row *row, void *data);


#endif   /* ----- #ifndef BOOT_PANEL_INC  ----- */

