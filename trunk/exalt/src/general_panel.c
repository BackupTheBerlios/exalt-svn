#include "general_panel.h"


general_panel* generalpanel_create(main_window *win)
{
	Etk_Widget *vbox;
	general_panel* pnl;

	pnl=malloc(sizeof(general_panel));

        pnl->win = win;

	vbox = etk_vbox_new(ETK_FALSE, 5);
	pnl->frame = vbox;

        pnl->notebook = etk_notebook_new();
        etk_box_append(ETK_BOX(vbox), pnl->notebook, ETK_BOX_START, ETK_BOX_EXPAND_FILL, 0);

        pnl->dns = dnspanel_create();
        etk_notebook_page_append(ETK_NOTEBOOK(pnl->notebook),_("DNS (Dynamic Name Server)"),pnl->dns->frame);

        pnl->boot = bootpanel_create(win);

        pnl->about = aboutpanel_create();
        etk_notebook_page_append(ETK_NOTEBOOK(pnl->notebook),_("About"),pnl->about->frame);

        return pnl;
}

void generalpanel_free(general_panel** pnl)
{
    if(pnl && *pnl)
    {
        dnspanel_free(&(*pnl)->dns);
        bootpanel_free(&(*pnl)->boot);
        aboutpanel_free(&(*pnl)->about);
        EXALT_FREE(*pnl);
    }
}

void generalpanel_show(general_panel* pnl)
{
	etk_widget_show_all(pnl->frame);
}

void generalpanel_hide(general_panel* pnl)
{
	etk_widget_hide_all(pnl->frame);
}

void generalpanel_update_advanced_mode(general_panel *pnl)
{
    if(!pnl->win->advanced_mode)
    {
        int num = etk_notebook_page_index_get(ETK_NOTEBOOK(pnl->notebook), pnl->boot->frame);
        //remove a notebook segfault
        /*if(num>0)
            etk_notebook_page_remove(ETK_NOTEBOOK(pnl->notebook),num);
            */
    }
    else
        etk_notebook_page_append(ETK_NOTEBOOK(pnl->notebook),_("Boot process"),pnl->boot->frame);
    bootpanel_show(pnl->boot);
}

