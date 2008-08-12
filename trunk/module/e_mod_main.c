#include "e_mod_main.h"

static E_Config_DD *conf_edd = NULL;
Config *exalt_config = NULL;

EAPI E_Module_Api e_modapi =
{
    E_MODULE_API_VERSION, "Exalt"
};

    EAPI void *
e_modapi_init(E_Module *m)
{
    bindtextdomain("module_exalt", LOCALEDIR);
    textdomain( "module_exalt" );
    conf_edd = E_CONFIG_DD_NEW("Exalt_Config",Config);
#undef T
#undef D
#define T Config
#define D conf_edd
    E_CONFIG_VAL(D, T, cmd, STR);

    exalt_config = e_config_domain_load("module.exalt", conf_edd);
    if(!exalt_config)
    {
        exalt_config = E_NEW(Config,1);
        exalt_config->cmd = strdup("exalt -i %i -w %w");
    }

    exalt_config->module = m;
    exalt_config->menu = NULL;

    exalt_dbus_init();
    exalt_config->conn = exalt_dbus_connect();
    exalt_config->interfaces = NULL;
    exalt_config->wireless = ecore_list_new();
    exalt_config->wireless->free_func = ECORE_FREE_CB(ecore_list_destroy);
    exalt_gc_register();
    return m;
}

    EAPI int
e_modapi_shutdown(E_Module *m)
{
    exalt_gc_unregister();

    exalt_dbus_shutdown();

    if(exalt_config->config_dialog)
        e_object_del(E_OBJECT(exalt_config->config_dialog));
    if(exalt_config->menu)
    {
        e_menu_post_deactivate_callback_set(exalt_config->menu, NULL, NULL);
        e_object_del(E_OBJECT(exalt_config->menu));
        exalt_config->menu = NULL;
    }

    if(exalt_config->interfaces)
    {
        ecore_list_destroy(exalt_config->interfaces);
        exalt_config->interfaces = NULL;
    }

    if(exalt_config->wireless)
    {
        ecore_list_destroy(exalt_config->wireless);
        exalt_config->wireless= NULL;
    }

    free(exalt_config);
    exalt_config = NULL;
    E_CONFIG_DD_FREE(conf_edd);
    return 1;
}

    EAPI int
e_modapi_save(E_Module *m)
{
    e_config_domain_save("module.exalt",conf_edd, exalt_config);
    return 1;
}

    EAPI int
e_modapi_about(E_Module *m)
{
    e_module_dialog_show(m, _("Exalt Module"),
            _("This module allow you to configure yours networks cards."));
    return 1;
}


    EAPI int
e_modapi_config(E_Module *m)
{
    if (!exalt_config) return 0;
    if (exalt_config->config_dialog) return 0;
    _config_exalt_module();
    return 1;
}

