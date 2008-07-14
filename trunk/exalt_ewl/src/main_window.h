
#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <Ewl.h>
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

int mainWindow_free(main_window** win);
void mainWindow_close(Ewl_Widget *win, void *ev, void *data);

void mainwindow_notify_cb(char* interface, Exalt_Enum_Action action, void* user_data);

void main_window_btn_mode_clicked_cb(Ewl_Widget *w __UNUSED__,
        void *ev_data __UNUSED__,
        void *user_data);

void main_window_eth_tree_changed_cb(Ewl_Widget *w, void *ev __UNUSED__, void *data);


unsigned int mainwindow_eth_tree_data_count (void *data);
void* mainwindow_eth_tree_data_fetch (
        void        *data,
        unsigned int row,
        unsigned int column);
void * mainwindow_eth_tree_header_data_fetch(void *data __UNUSED__, unsigned int column);
Ewl_Widget * mainwindow_eth_tree_widget_fetch(void *data, unsigned int row,
        unsigned int column __UNUSED__);

#endif
