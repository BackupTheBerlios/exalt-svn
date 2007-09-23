
#include "e_mod_gadcon.h"
short exalt_is_init = 0;

static E_Gadcon_Client *_gc_init(E_Gadcon *gc, const char *name, const char *id, const char *style);
static void _gc_shutdown(E_Gadcon_Client *gcc);
static void _gc_orient(E_Gadcon_Client *gcc);
static char *_gc_label(void);
static const char *_gc_id_new(void);

static Evas_Object *_gc_icon(Evas *evas);

static const E_Gadcon_Client_Class _gc_class =
{
    GADCON_CLIENT_CLASS_VERSION, "exalt",
    {_gc_init, _gc_shutdown, _gc_orient, _gc_label, _gc_icon, _gc_id_new},
    E_GADCON_CLIENT_STYLE_PLAIN
};

    static E_Gadcon_Client *
_gc_init(E_Gadcon *gc, const char *name, const char *id, const char *style)
{
    Instance *inst;
    E_Gadcon_Client *gcc;
    char buf[PATH_MAX];

    snprintf(buf, sizeof(buf), "%s/exalt.edj", e_module_dir_get(exalt_config->module));

    inst = E_NEW(Instance, 1);
    inst->o_button = edje_object_add(gc->evas);
    if (!e_theme_edje_object_set(inst->o_button, "base/theme/modules/exalt",
                "modules/exalt/main"))
        edje_object_file_set(inst->o_button, buf, "e/modules/exalt/main");


    gcc = e_gadcon_client_new(gc, name, id, style, inst->o_button);
    gcc->data = inst;
    inst->gcc = gcc;

    evas_object_event_callback_add(inst->o_button, EVAS_CALLBACK_MOUSE_DOWN, _cb_mouse_down, inst);

    exalt_config->instances = evas_list_append(exalt_config->instances, inst);

    exalt_dbus_notify_set( exalt_config->conn, _exalt_notify_cb, inst);

    return gcc;
}

    static void
_gc_shutdown(E_Gadcon_Client *gcc)
{
    Instance *inst;

    inst = gcc->data;

    exalt_config->instances = evas_list_remove(exalt_config->instances, inst);

    if (exalt_config->menu)
    {
        e_menu_post_deactivate_callback_set(exalt_config->menu, NULL, NULL);
        e_menu_pre_activate_callback_set(exalt_config->menu, NULL, NULL);
        e_object_del(E_OBJECT(exalt_config->menu));
        exalt_config->menu = NULL;
    }

    if (inst->o_button)
    {
        evas_object_event_callback_del(inst->o_button, EVAS_CALLBACK_MOUSE_DOWN,
                _cb_mouse_down);

        evas_object_del(inst->o_button);
    }

    exalt_dbus_notify_set( exalt_config->conn, NULL, NULL);
    E_FREE(inst);
}

    static void
_gc_orient(E_Gadcon_Client *gcc)
{
    e_gadcon_client_aspect_set(gcc, 16, 16);
    e_gadcon_client_min_size_set(gcc, 16, 16);
}

    static char *
_gc_label(void)
{
    return _("Exalt");
}

    static Evas_Object *
_gc_icon(Evas *evas)
{
    Evas_Object *o;
    char buf[PATH_MAX];

    snprintf(buf, sizeof(buf), "%s/e-module-exalt.edj", e_module_dir_get(exalt_config->module));
    o = edje_object_add(evas);
    edje_object_file_set(o, buf, "icon");
    return o;
}

    EAPI void
exalt_gc_register(void)
{
    e_gadcon_provider_register(&_gc_class);
}

    EAPI void
exalt_gc_unregister(void)
{
    e_gadcon_provider_unregister(&_gc_class);
}

    static const char *
_gc_id_new(void)
{
    return _gc_class.name;
}


