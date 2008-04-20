#include "about_panel.h"


about_panel* aboutpanel_create()
{
    Ewl_Widget *text,*vbox,*img, *grid, *border;
    about_panel* pnl;

    pnl = malloc(sizeof(pnl));

    vbox = ewl_vbox_new();

    pnl->frame = vbox;

    border = ewl_border_new();
    ewl_border_label_set(EWL_BORDER(border),_("About"));
    ewl_object_fill_policy_set(EWL_OBJECT(border),EWL_FLAG_FILL_HSHRINK);
    text = ewl_text_new();

    ewl_text_styles_set(EWL_TEXT(text), EWL_TEXT_STYLE_GLOW
            );
    ewl_text_glow_color_set(EWL_TEXT(text), 230, 80, 5, 200);
    ewl_text_text_append(EWL_TEXT(text),_("Exalt"));
    ewl_text_styles_set(EWL_TEXT(text), EWL_TEXT_STYLE_NONE);
    ewl_text_text_append(EWL_TEXT(text),
            _(" is a network manager that uses Ewl as its toolkit. "
                "\n\nExalt supports ethernets interfaces (wired or wireless) \nand allows you to configure your network configuration easily."
                "\n\nAuthor: Watchwolf (watchwolf@watchwolf.fr)"
                "\n\nThis software is provided as-is with no explicit or implied warranty. \nThis software is governed by licensing conditions, so please see \nthe COPYING and COPYING-PLAIN license files installed on your system."
                "\n\n"));

    ewl_text_styles_set(EWL_TEXT(text), EWL_TEXT_STYLE_GLOW
            );
    ewl_text_glow_color_set(EWL_TEXT(text), 230, 80, 5, 200);

    ewl_text_align_set(EWL_TEXT(text),EWL_FLAG_ALIGN_CENTER);
    ewl_text_text_append(EWL_TEXT(text),VERSION);

    ewl_container_child_append(EWL_CONTAINER(border), text);
    ewl_container_child_append(EWL_CONTAINER(vbox), border);

    grid = ewl_grid_new();
    ewl_container_child_append(EWL_CONTAINER(vbox), grid);
    ewl_object_fill_policy_set(EWL_OBJECT(grid), EWL_FLAG_FILL_HFILL);
    ewl_grid_dimensions_set(EWL_GRID(grid), 2, 1);
    ewl_widget_show(grid);

    img = ewl_image_new();
    ewl_image_file_path_set(EWL_IMAGE(img), ewl_icon_theme_icon_path_get(EWL_ICON_NETWORK_WIRED, EWL_ICON_SIZE_LARGE));
    ewl_container_child_append(EWL_CONTAINER(grid), img);
    ewl_widget_show(img);
    ewl_object_alignment_set(EWL_OBJECT(img),EWL_FLAG_ALIGN_CENTER);



    img = ewl_image_new();
    ewl_image_file_path_set(EWL_IMAGE(img), ewl_icon_theme_icon_path_get(EWL_ICON_NETWORK_WIRELESS, EWL_ICON_SIZE_LARGE));
    ewl_container_child_append(EWL_CONTAINER(grid), img);
    ewl_widget_show(img);
    ewl_object_alignment_set(EWL_OBJECT(img),EWL_FLAG_ALIGN_CENTER);

    ewl_widget_show(border);
    ewl_widget_show(text);
    ewl_widget_show(pnl->frame);

    return pnl;
}

void aboutpanel_free(about_panel** pnl)
{
    if(pnl)
        EXALT_FREE(*pnl);
}


void aboutpanel_show(about_panel* pnl)
{
    ewl_widget_show(pnl->frame);
}

void aboutpanel_hide(about_panel* pnl)
{
    ewl_widget_hide(pnl->frame);
}


