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



boot_panel* bootpanel_create()
{
    boot_panel* boot_pnl;

    boot_pnl = malloc(sizeof(boot_panel));
    boot_pnl->frame  = etk_frame_new(_("Boot process"));

    return boot_pnl;
}


void bootpanel_show(boot_panel* pnl)
{
	etk_widget_show_all(pnl->frame);
}

void bootpanel_hide(boot_panel* pnl)
{
	etk_widget_hide_all(pnl->frame);
}


