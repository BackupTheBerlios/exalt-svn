
#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <Etk.h>
#include <stdio.h>
#include <Ecore_Data.h>

typedef struct _main_window main_window;

#include "exalt.h"
#include "eth_panel.h"
#include "wireless_panel.h"
#include "general_panel.h"
#include "about_panel.h"
#include "boot_panel.h"

struct _main_window
{
	Etk_Widget *win;

	Etk_Widget* eth_list;
	Etk_Tree_Col* eth_col0;
        Etk_Widget* btn_mode;

	eth_panel* eth_panel;
	wireless_panel* wireless_panel;
	general_panel* general_panel;

        int advanced_mode;
};

main_window* mainwindow_create();
void mainwindow_add_interface(char* interface, main_window* win);
void mainwindow_remove_interface(char* interface, main_window* win);

Etk_Bool mainWindow_free(main_window** win);
Etk_Bool mainWindow_close(Etk_Object *object, void *data);

void mainwindow_notify_cb(char* interface, Exalt_Enum_Action action, void* user_data);
void mainWindow_ethList_row_clicked_cb(Etk_Object *object, Etk_Tree_Row *row, Etk_Event_Mouse_Up *event, void *data);
Etk_Tree_Row * mainwindow_findrow(main_window* win, char* interface);
void mainwindow_btn_mode_clicked_cb(Etk_Object *object , void *data);


#endif
