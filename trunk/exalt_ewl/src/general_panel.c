#include "general_panel.h"


general_panel* generalpanel_create(main_window *win)
{
	Ewl_Widget *vbox, *label;
	general_panel* pnl;

	pnl=malloc(sizeof(general_panel));

        pnl->win = win;

	vbox = ewl_vbox_new();
	pnl->frame = vbox;

        pnl->notebook = ewl_notebook_new();
        ewl_container_child_append(EWL_CONTAINER(vbox), pnl->notebook);

        pnl->dns = dnspanel_create();
        pnl->boot = bootpanel_create(win);
        pnl->about = aboutpanel_create();

        label = ewl_label_new();
        ewl_label_text_set(EWL_LABEL(label), _("DNS (Dynamic Name Server)"));
        ewl_widget_show(label);

        ewl_container_child_append(EWL_CONTAINER(pnl->notebook), pnl->dns->frame);
        ewl_notebook_page_tab_widget_set(EWL_NOTEBOOK(pnl->notebook) ,pnl->dns->frame, label);

        label = ewl_label_new();
        ewl_label_text_set(EWL_LABEL(label), _("Boot process"));
        ewl_widget_show(label);

        ewl_container_child_append(EWL_CONTAINER(pnl->notebook), pnl->boot->frame);
        ewl_notebook_page_tab_widget_set(EWL_NOTEBOOK(pnl->notebook) ,pnl->boot->frame, label);


        label = ewl_label_new();
        ewl_label_text_set(EWL_LABEL(label), _("About"));
        ewl_widget_show(label);

        ewl_container_child_append(EWL_CONTAINER(pnl->notebook), pnl->about->frame);
        ewl_notebook_page_tab_widget_set(EWL_NOTEBOOK(pnl->notebook) ,pnl->about->frame, label);

        ewl_widget_show(pnl->frame);
        ewl_widget_show(pnl->notebook);
        ewl_widget_show(pnl->about->frame);
        ewl_widget_show(label);

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
	ewl_widget_show(pnl->frame);
}

void generalpanel_hide(general_panel* pnl)
{
	ewl_widget_hide(pnl->frame);
}

void generalpanel_update_advanced_mode(general_panel *pnl)
{
    pnl=pnl;
    /*if(!pnl->win->advanced_mode)
    {
        Ewl_Widget *w = ewl_notebook_page_tab_widget_get(EWL_NOTEBOOK(pnl->notebook) ,pnl->boot->frame) ;
        ewl_widget_hide(w);
    }
    else
    {
       ewl_widget_show(pnl->boot->frame);
    }
    bootpanel_show(pnl->boot);
    */
}

