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
    Etk_Widget* frame;
} ;

boot_panel* bootpanel_create();
void bootpanel_show(boot_panel* pnl);
void bootpanel_hide(boot_panel* pnl);




#endif   /* ----- #ifndef BOOT_PANEL_INC  ----- */

