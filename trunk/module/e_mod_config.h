/*
 * vim:ts=8:sw=3:sts=8:noexpandtab:cino=>5n-3f0^-2{2
 */
#ifndef E_MOD_CONFIG_H
#define E_MOD_CONFIG_H

#include "e_mod_main.h"
#include "e_mod_gadcon.h"
#include "e_mod_config.h"
#include <libexalt_dbus.h>
#include <e.h>

typedef struct _Config Config;

struct _Config
{
 	char* cmd;

	E_Module *module;
	E_Config_Dialog *config_dialog;
	Evas_List       *instances;
	E_Menu          *menu;

        exalt_dbus_conn* conn;
        Ecore_List* interfaces;
        Ecore_List* wireless;
};

extern Config *exalt_config;



#endif
