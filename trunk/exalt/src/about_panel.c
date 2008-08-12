#include "about_panel.h"


about_panel* aboutpanel_create()
{
    Etk_Widget *text,*vbox, *hbox,*img;
    about_panel* pnl;
    char buf[2048];

    pnl = malloc(sizeof(pnl));

    vbox = etk_vbox_new(ETK_FALSE, 5);
    pnl->frame = vbox;
    hbox = etk_hbox_new(ETK_FALSE, 5);

    text = etk_text_view_new();
    etk_widget_size_request_set(text, -1, 150);
    etk_object_properties_set(ETK_OBJECT(text), "focusable", ETK_FALSE, NULL);
    snprintf(buf,2048,_("<p><b>Exalt</b> is a network manager that uses Etk as its toolkit. </p>"
             "\n<p>Exalt supports ethernets interfaces (wired or wireless) and allows you to configure your network configuration easily.</p>"
             "\n<p>Author: Watchwolf (watchwolf@watchwolf.fr)</p>"
             "\n<p>This software is provided as-is with no explicit or implied warranty. This software is governed by licensing conditions, so please see the COPYING and COPYING-PLAIN license files installed on your system.</p>"
             "\n<p align=\"center\"><style effect=glow color1=#fa14 color2=#fe87>"
             "<b>%s</b>"
             "</style></p>"),VERSION);
    etk_textblock_text_set(ETK_TEXT_VIEW(text)->textblock,
             buf,
             ETK_TRUE);
    etk_box_append(ETK_BOX(vbox), text, ETK_BOX_START, ETK_BOX_EXPAND_FILL, 0);

    etk_box_append(ETK_BOX(vbox), hbox, ETK_BOX_START, ETK_BOX_EXPAND_FILL, 0);


    img = etk_image_new_from_stock(ETK_STOCK_NETWORK_WIRED,ETK_STOCK_BIG);
    etk_box_append(ETK_BOX(hbox), img, ETK_BOX_START, ETK_BOX_EXPAND, 0);

    img = etk_image_new_from_stock(ETK_STOCK_NETWORK_WIRELESS,ETK_STOCK_BIG);
    etk_box_append(ETK_BOX(hbox), img, ETK_BOX_START, ETK_BOX_EXPAND, 0);

    return pnl;
}

void aboutpanel_free(about_panel** pnl)
{
    if(pnl)
        EXALT_FREE(*pnl);
}


void aboutpanel_show(about_panel* pnl)
{
	etk_widget_show_all(pnl->frame);
}

void aboutpanel_hide(about_panel* pnl)
{
	etk_widget_hide_all(pnl->frame);
}


