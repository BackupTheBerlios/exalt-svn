#ifndef ABOUT_PANEL_H
#define ABOUT_PANEL_H

typedef struct about_panel about_panel;

#include "main_window.h"
#include <Etk.h>

struct about_panel
{
    Ewl_Widget* frame;
} ;

about_panel* aboutpanel_create();
void aboutpanel_show(about_panel* pnl);
void aboutpanel_free(about_panel** pnl);

void aboutpanel_hide(about_panel* pnl);


#endif

