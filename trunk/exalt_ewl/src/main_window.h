
#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <Ewl.h>
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
	Ewl_Widget *win;

	Ewl_Widget* eth_tree;
        Ecore_List *eth_list;
        Ewl_Widget* btn_mode;

	eth_panel* eth_panel;
	wireless_panel* wireless_panel;
	general_panel* general_panel;

        int advanced_mode;
};

main_window* mainwindow_create();
void mainwindow_add_interface(char* interface, main_window* win);
void mainwindow_remove_interface(char* interface, main_window* win);

Etk_Bool mainWindow_free(main_window** win);
void mainWindow_close(Ewl_Widget *win, void *ev, void *data);

void mainwindow_notify_cb(char* interface, Exalt_Enum_Action action, void* user_data);
void mainWindow_ethList_row_clicked_cb(Etk_Object *object, Etk_Tree_Row *row, Etk_Event_Mouse_Up *event, void *data);
Etk_Tree_Row * mainwindow_findrow(main_window* win, char* interface);

void main_window_btn_mode_clicked_cb(Ewl_Widget *w __UNUSED__,
        void *ev_data __UNUSED__,
        void *user_data);

typedef struct
{
    char* name;
    char* image;
    char* status;
}Eth_Elt;

void eth_elt_free(void* elt);
unsigned int eth_ewl_package_tree_data_count (void *data);
void* eth_ewl_package_tree_data_fetch (
        void        *data,
        unsigned int row,
        unsigned int column);
void * eth_ewl_cb_header_data_fetch(void *data __UNUSED__, unsigned int column);
Ewl_Widget * eth_ewl_cb_widget_fetch(void *data, unsigned int row __UNUSED__,
        unsigned int column);
void main_window_eth_tree_changed_cb(Ewl_Widget *w, void *ev __UNUSED__, void *data);

#endif
