#include "about_panel.h"


about_panel* aboutpanel_create()
{
    Etk_Widget *text;
    about_panel* about_pnl;

    about_pnl = malloc(sizeof(about_panel));
    about_pnl->frame  = etk_frame_new(_("About Exalt"));

    text = etk_text_view_new();
    etk_widget_size_request_set(text, -1, 150);
    etk_object_properties_set(ETK_OBJECT(text), "focusable", ETK_FALSE, NULL);
    etk_textblock_text_set(ETK_TEXT_VIEW(text)->textblock,
             "<p><b>Exalt</b> is a network manager that uses Etk as its toolkit. </p>"
             "\n<p>Exalt supports ethernets interfaces (wired or wireless) and allows you to configure your network configuration easily.</p>"
             "\n<p>Author: Watchwolf (watchwolf@watchwolf.fr)</p>"
             "\n<p align=\"center\"><style effect=glow color1=#fa14 color2=#fe87>"
             "<b>"VERSION"</b>"
             "</style></p>",
             ETK_TRUE);
    etk_container_add(ETK_CONTAINER(about_pnl->frame), text);

    return about_pnl;
}


void aboutpanel_show(about_panel* pnl)
{
	etk_widget_show_all(pnl->frame);
}

void aboutpanel_hide(about_panel* pnl)
{
	etk_widget_hide_all(pnl->frame);
}


