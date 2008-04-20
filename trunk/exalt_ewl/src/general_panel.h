#ifndef GENERALPANEL_H
#define GENERALPANEL_H


#include <Etk.h>
#include <stdio.h>

typedef struct _general_panel general_panel;

#include "exalt.h"
#include "about_panel.h"
#include "boot_panel.h"
#include "dns_panel.h"

struct _general_panel
{
    main_window* win;

    Ewl_Widget *frame;

    Ewl_Widget *notebook;

    about_panel *about;
    dns_panel* dns;
    boot_panel *boot;
};

general_panel* generalpanel_create(main_window* win);
void generalpanel_free(general_panel** pnl);
void generalpanel_show(general_panel* pnl);
void generalpanel_hide(general_panel* pnl);

void generalpanel_update_advanced_mode(general_panel *pnl);


#endif
