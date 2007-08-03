#ifndef INFO_PANEL_H
#define INFO_PANEL_H

typedef struct info_panel info_panel;

#include "main_window.h"
#include <Etk.h>
#include <libexalt.h>

struct info_panel
{
    Etk_Widget* frame;
    Etk_Widget* hbox;
} ;

info_panel* infopanel_create();
void infopanel_eth_cb(exalt_ethernet* eth, int action, void* user_data);


#endif

