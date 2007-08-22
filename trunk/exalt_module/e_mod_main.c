#include <e.h>
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
    bindtextdomain(PACKAGE, LOCALEDIR);
    bind_textdomain_codeset(PACKAGE, "UTF-8");

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
        exalt_config->cmd = strdup("gksu \"exalt -i %i -w %w\"");
    }

    exalt_config->module = m;
    _gc_register();
    return m;
}

    EAPI int
e_modapi_shutdown(E_Module *m)
{
    _gc_unregister();

    if(exalt_config->config_dialog)
        e_object_del(E_OBJECT(exalt_config->config_dialog));
    if(exalt_config->menu)
    {
        e_menu_post_deactivate_callback_set(exalt_config->menu, NULL, NULL);
        e_object_del(E_OBJECT(exalt_config->menu));
        exalt_config->menu = NULL;
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

