#include "e_mod_exalt.h"


void _cb_mouse_down (void *data, Evas * e, Evas_Object * obj,void *event_info)
{
	Instance *inst;
	Evas_Event_Mouse_Down *ev;

	inst = data;
	ev = event_info;
	if (ev->button == 1)
	{
		Evas_Coord x, y, w, h;
		int cx, cy, cw, ch;
		int dir;

		evas_object_geometry_get(inst->o_button, &x, &y, &w, &h);
		e_gadcon_canvas_zone_geometry_get(inst->gcc->gadcon,
				&cx, &cy, &cw, &ch);
		x += cx;
		y += cy;
		exalt_config->menu = _exalt_menu_new(inst);

		e_menu_post_deactivate_callback_set(exalt_config->menu, _menu_cb_post,
				inst);

		dir = E_MENU_POP_DIRECTION_AUTO;
		switch (inst->gcc->gadcon->orient)
		{
			case E_GADCON_ORIENT_TOP:
				dir = E_MENU_POP_DIRECTION_DOWN;
				break;
			case E_GADCON_ORIENT_BOTTOM:
				dir = E_MENU_POP_DIRECTION_UP;
				break;
			case E_GADCON_ORIENT_LEFT:
				dir = E_MENU_POP_DIRECTION_RIGHT;
				break;
			case E_GADCON_ORIENT_RIGHT:
				dir = E_MENU_POP_DIRECTION_LEFT;
				break;
			case E_GADCON_ORIENT_CORNER_TL:
				dir = E_MENU_POP_DIRECTION_DOWN;
				break;
			case E_GADCON_ORIENT_CORNER_TR:
				dir = E_MENU_POP_DIRECTION_DOWN;
				break;
			case E_GADCON_ORIENT_CORNER_BL:
				dir = E_MENU_POP_DIRECTION_UP;
				break;
			case E_GADCON_ORIENT_CORNER_BR:
				dir = E_MENU_POP_DIRECTION_UP;
				break;
			case E_GADCON_ORIENT_CORNER_LT:
				dir = E_MENU_POP_DIRECTION_RIGHT;
				break;
			case E_GADCON_ORIENT_CORNER_RT:
				dir = E_MENU_POP_DIRECTION_LEFT;
				break;
			case E_GADCON_ORIENT_CORNER_LB:
				dir = E_MENU_POP_DIRECTION_RIGHT;
				break;
			case E_GADCON_ORIENT_CORNER_RB:
				dir = E_MENU_POP_DIRECTION_LEFT;
				break;
			case E_GADCON_ORIENT_FLOAT:
			case E_GADCON_ORIENT_HORIZ:
			case E_GADCON_ORIENT_VERT:
			default:
				dir = E_MENU_POP_DIRECTION_AUTO;
				break;
		}
		e_menu_activate_mouse(exalt_config->menu,
				e_util_zone_current_get(e_manager_current_get()),
				x, y, w, h, dir, ev->timestamp);
		evas_event_feed_mouse_up(inst->gcc->gadcon->evas, ev->button,
				EVAS_BUTTON_NONE, ev->timestamp, NULL);
	}
	else if( ev->button == 3)
	{
		E_Menu *mn;
		E_Menu_Item *mi;
		int cx,cy,cw,ch;

		mn = e_menu_new();
		e_menu_post_deactivate_callback_set(mn, _menu_cb_post, inst);
		exalt_config->menu = mn;

		mi = e_menu_item_new(mn);
		e_menu_item_label_set(mi, _("Configuration"));
		e_util_menu_item_edje_icon_set(mi,  "enlightenment/configuration");
		e_menu_item_callback_set(mi, _exalt_cb_menu_configure, NULL);

		e_gadcon_client_util_menu_items_append(inst->gcc, mn, 0);

		e_gadcon_canvas_zone_geometry_get(inst->gcc->gadcon, &cx, &cy, &cw, &ch);
		e_menu_activate_mouse(mn,e_util_zone_current_get(e_manager_current_get()),
				cx + ev->output.x, cy + ev->output.y, 1, 1,
				E_MENU_POP_DIRECTION_DOWN, ev->timestamp);
		e_util_evas_fake_mouse_up_later(inst->gcc->gadcon->evas, ev->button);
	}
}

void _popup_show(Instance *inst, char* interface, Exalt_Enum_Action action)
{
	E_Container *con;
	Evas_Object *bg,*content;
	Evas_Coord cx, cy, cw, ch;
	Evas_Coord ox, oy, ow, oh;
	Evas_List *l;
	int layer = -1;
	int wx, wy, ww, wh;
	char buf[PATH_MAX];
	Popup* popup;

	exalt_dbus_conn *conn = exalt_config->conn;

	switch(action)
	{
		case EXALT_ETH_CB_ACTION_ADD:
		case EXALT_ETH_CB_ACTION_REMOVE:
		case EXALT_ETH_CB_ACTION_UP:
		case EXALT_ETH_CB_ACTION_DOWN:
		case EXALT_ETH_CB_ACTION_LINK:
		case EXALT_ETH_CB_ACTION_UNLINK:
		case EXALT_WIRELESS_CB_ACTION_ESSIDCHANGE:
                    break;
		case EXALT_ETH_CB_ACTION_ADDRESS_NEW:
                    if(!exalt_dbus_eth_get_ip(conn,interface))
                        return ;
                    break;
                case EXALT_ETH_CB_ACTION_CONN_APPLY_START:
                    edje_object_signal_emit(inst->o_button, "e,icon,conf,start", "exalt");
                    return;
                case EXALT_ETH_CB_ACTION_CONN_APPLY_DONE:
                    edje_object_signal_emit(inst->o_button, "e,icon,conf,stop", "exalt");
		    return;
		default:
			return ;
	}

	popup = E_NEW(Popup, 1);
	snprintf(buf, sizeof(buf), "%s/exalt.edj", e_module_dir_get(exalt_config->module));
	con = e_container_current_get(e_manager_current_get());
	popup->win = e_popup_new(e_zone_current_get(con), 0, 0, 0, 0);
        bg = edje_object_add(popup->win->evas);
	content = edje_object_add(popup->win->evas);

        e_theme_edje_object_set(bg, "base/theme/menus","e/widgets/menu/default/background");

        if (!e_theme_edje_object_set(content, "base/theme/modules","e/modules/exalt/popup"))
          	edje_object_file_set(content, buf, "e/modules/exalt/popup");


	switch(action)
	{
		case EXALT_ETH_CB_ACTION_ADD:
			snprintf(buf, sizeof(buf), _("New card is <info>plugged</info>"));
			break;
		case EXALT_ETH_CB_ACTION_REMOVE:
			snprintf(buf, sizeof(buf), _("The card is <info>removed</info>"));
			break;
		case EXALT_ETH_CB_ACTION_UP:
			snprintf(buf, sizeof(buf), _("The card is <info>activated</info>"));
			break;
		case EXALT_ETH_CB_ACTION_DOWN:
			snprintf(buf, sizeof(buf), _("The card is <info>deactivated</info>"));
			break;
		case EXALT_ETH_CB_ACTION_LINK:
			snprintf(buf, sizeof(buf), _("The card is <info>link</info>"));
			break;
		case EXALT_ETH_CB_ACTION_UNLINK:
			snprintf(buf, sizeof(buf), _("The card is <info>unlink</info>"));
			break;
		case EXALT_WIRELESS_CB_ACTION_ESSIDCHANGE:
			snprintf(buf, sizeof(buf), _("New essid: <info>%s</info>"), exalt_dbus_wireless_get_essid(conn,interface));
			break;
		case EXALT_ETH_CB_ACTION_ADDRESS_NEW:
			snprintf(buf, sizeof(buf), _("New address: <info>%s</info>"), exalt_dbus_eth_get_ip(conn,interface));
			break;
                default:
			return ;
	}

	edje_object_part_text_set(content, "e.text", buf);

	snprintf(buf,sizeof(buf), interface);
	edje_object_part_text_set(content, "e.text.interface_name", buf);

	if(exalt_dbus_eth_is_wireless(conn,interface))
	{
		if(exalt_dbus_eth_is_up(conn,interface))
			edje_object_signal_emit(content, "e,state,wireless,up", "exalt");
		else
			edje_object_signal_emit(content, "e,state,wireless,down", "exalt");
	}
	else if(exalt_dbus_eth_is_up(conn,interface) && exalt_dbus_eth_is_link(conn,interface))
		edje_object_signal_emit(content, "e,state,ethernet,up", "exalt");
	else
		edje_object_signal_emit(content, "e,state,ethernet,down", "exalt");

	evas_object_show(content);

	edje_object_size_min_calc(content, &ww, &wh);
	evas_object_move(content, 0, 0);
	evas_object_resize(content, ww, wh);
        evas_object_move(bg, 0, 0);
	evas_object_resize(bg, ww, wh);
        evas_object_show(bg);
        evas_object_show(content);

	popup->o_content = content;
        popup->o_bg = bg;

	// Begin Butt Ugly hack for shelf "layer"/position changes
	cx = cy = cw = ch = -1;
	for (l = e_shelf_list(); l; l = l->next)
	{
		E_Shelf *es;

		es = l->data;
		if (es->gadcon != inst->gcc->gadcon) continue;
		layer = es->layer;
		cx = es->x;
		cy = es->y;
		cw = es->w;
		ch = es->h;
		break;
	}

	if (cx == -1) return;
	evas_object_geometry_get(inst->o_button, &ox, &oy, &ow, &oh);
	switch (inst->gcc->gadcon->orient)
	{
		case E_GADCON_ORIENT_CORNER_RT:
		case E_GADCON_ORIENT_CORNER_RB:
		case E_GADCON_ORIENT_RIGHT:
			wx = (cx - ww);
			if (layer == 1)
				wy = oy;
			else
				wy = (cy + oy);
			if ((wy + wh) > (cy + ch))
				wy = (cy + ch) - wh;
			break;
		case E_GADCON_ORIENT_LEFT:
		case E_GADCON_ORIENT_CORNER_LT:
		case E_GADCON_ORIENT_CORNER_LB:
			wx = (cx + cw);
			if (layer == 1)
				wy = oy;
			else
				wy = (cy + oy);
			if ((wy + wh) > (cy + ch))
				wy = (cy + ch) - wh;
			break;
		case E_GADCON_ORIENT_TOP:
		case E_GADCON_ORIENT_CORNER_TL:
			if (layer == 1)
				wx = ox;
			else
				wx = (cx + ox);
			wy = (cy + ch);
			break;
		case E_GADCON_ORIENT_CORNER_TR:
			if (layer == 1)
				wx = ox;
			else
				wx = (cx + ox);
			wy = (cy + ch);
			if ((wx + ww) > (cx + cw))
				wx = (cx + cw) - ww;
			break;
		case E_GADCON_ORIENT_BOTTOM:
		case E_GADCON_ORIENT_CORNER_BL:
			if (layer == 1)
				wx = ox;
			else
				wx = (cx + ox);
			wy = (cy - wh);
			break;
		case E_GADCON_ORIENT_CORNER_BR:
			if (layer == 1)
				wx = ox;
			else
				wx = (cx + ox);
			wy = (cy - wh);
			if ((wx + ww) > (cx + cw))
				wx = (cx + cw) - ww;
			break;
		default:
			break;
	}
	e_popup_move_resize(popup->win, wx, wy, ww, wh);
	e_popup_show(popup->win);
	popup->timer = ecore_timer_add(5,_popup_timer_cb,popup);
}

int _popup_timer_cb(void* data)
{
	//close the popup
	Popup* popup = (Popup*)data;

	evas_object_del(popup->o_bg);
        evas_object_del(popup->o_content);
	e_object_del(E_OBJECT(popup->win));
	E_FREE(popup);
	return 0;
}

	static void
_exalt_cb_menu_configure(void *data, E_Menu *m, E_Menu_Item *mi)
{
	if (!exalt_config) return ;
	if(exalt_config->config_dialog) return ;
	_config_exalt_module();
}

	static E_Menu *
_exalt_menu_new(Instance *inst)
{
	E_Menu *m;

	m = e_menu_new();
	e_menu_title_set(m, _("Networks"));

	e_menu_pre_activate_callback_set(m, _exalt_menu_pre_cb, inst);
	return m;
}

	static void
_exalt_menu_pre_cb(void *data, E_Menu *m)
{
	E_Menu_Item *mi;
	Evas_List *lcards = NULL, *l, *borders = NULL, *alt = NULL;
	E_Zone *zone = NULL;
	E_Desk *desk = NULL;
	Instance *inst;

	inst = data;

	/* get the current clients */
	zone = e_util_zone_current_get (e_manager_current_get ());
	desk = e_desk_current_get(zone);
	e_menu_item_separator_set(e_menu_item_new(exalt_config->menu), 1);
	_exalt_menu_item_interfaces_load(exalt_config->conn,exalt_config->menu);
}

void _exalt_menu_item_wireless_load(exalt_dbus_conn *conn, E_Menu *m, char* interface)
{
	E_Menu_Item *mi;
	Ecore_List *l;
	char* essid;

	char img1[] = PACKAGE_DATA_DIR ICONS_QUALITY_LESS_25;
	char img2[] = PACKAGE_DATA_DIR ICONS_QUALITY_LESS_50;
	char img3[] = PACKAGE_DATA_DIR ICONS_QUALITY_LESS_75;
	char img4[] = PACKAGE_DATA_DIR ICONS_QUALITY_LESS_100;
	char *img[4];
	img[0] = img1;img[1] = img2;img[2] = img3;img[3] = img4;
	char** data;

	if(!m || !interface)
		return;

	if(exalt_dbus_eth_is_wireless(conn,interface))
	{
		l = exalt_dbus_wireless_scan_wait(conn,interface);
		ecore_list_insert(exalt_config->wireless,l);
		ecore_list_first_goto(l);
		if(ecore_list_empty_is(l))
		{
			mi = e_menu_item_new(m);
			e_menu_item_label_set(mi,_("(no networks)"));
		}
		else
		{
			while( (essid=ecore_list_next(l)) )
			{
				data = malloc(sizeof(char*)*2);
				data[0] = interface;

				mi = e_menu_item_new(m);
				data[1] = essid;
				e_menu_item_icon_file_set(mi,img[(exalt_dbus_wirelessinfo_get_quality(conn,interface,essid))/25]);
				e_menu_item_label_set(mi, essid);
				e_menu_item_callback_set(mi, _exalt_wireless_cb, data);
				e_menu_item_check_set(mi,1);
				e_menu_item_toggle_set(mi, strcmp(exalt_dbus_wireless_get_essid(conn,interface),essid)==0);
			}
		}
	}
}

void _exalt_menu_item_interfaces_load(exalt_dbus_conn *conn, E_Menu *m)
{
	char* interface;
	E_Menu_Item *mi;
	Ecore_List *l;
	char buf[1024];
        char *ip;

	char wireless_img[] = PACKAGE_DATA_DIR ICONS_WIRELESS_ACTIVATE;
	char wireless_img_not_activate[] = PACKAGE_DATA_DIR ICONS_WIRELESS_NOT_ACTIVATE;
	char eth_img[] = PACKAGE_DATA_DIR ICONS_ETHERNET_ACTIVATE;
	char eth_img_not_activate[] = PACKAGE_DATA_DIR ICONS_ETHERNET_NOT_ACTIVATE;

	if(!m)
		return;

	l = exalt_dbus_eth_get_list(conn);
	exalt_config->interfaces = l;
	ecore_list_first_goto(l);
	while( (interface= ecore_list_next(l)))
	{
		mi = e_menu_item_new(m);
		char* img;
		if(exalt_dbus_eth_is_wireless(conn, interface))
		{
			if(exalt_dbus_eth_is_up(conn, interface))
				img = wireless_img;
			else
				img = wireless_img_not_activate;
		}
		else if(exalt_dbus_eth_is_up(conn, interface) && exalt_dbus_eth_is_link(conn, interface))
			img = eth_img;
		else
			img = eth_img_not_activate;

		e_menu_item_icon_file_set(mi,img);
                ip = exalt_dbus_eth_get_ip(conn,interface);
                if(ip)
		    sprintf(buf,"%s (%s)", interface,ip);
                else
                    sprintf(buf,"%s (%s)", interface,"no ip address");
                EXALT_FREE(ip);

		e_menu_item_label_set(mi,buf);

		e_menu_item_callback_set(mi, _exalt_interface_cb, interface);

		if(exalt_dbus_eth_is_wireless(conn,interface))
			_exalt_menu_item_wireless_load(conn,m,interface);

		e_menu_item_separator_set(e_menu_item_new(m), 1);
	}
}

void _exalt_interface_cb(void *data, E_Menu *m, E_Menu_Item *mi)
{
	char* interface;
	char *command1=NULL,*command2=NULL;
	int pos;
	interface = data;
	if(!interface || !exalt_config || !exalt_config->cmd)
		return ;

	pos = str_istr(exalt_config->cmd,"%i");
	command1 = str_remplace(exalt_config->cmd,pos,strlen("%i"),interface);
	if(command1)
	{
		pos = str_istr(command1,"%w");
		command2 = str_remplace(command1,pos,strlen("%w"),"");
	}
	else
	{
		pos = str_istr(exalt_config->cmd,"%w");
		command2 = str_remplace(exalt_config->cmd,pos,strlen("%w"),"");
	}
	if(command2)
		ecore_exe_run(command2,NULL);
	else if(command1)
		ecore_exe_run(command1, NULL);
	else
		ecore_exe_run(exalt_config->cmd,NULL);

	EXALT_FREE(command1);
	EXALT_FREE(command2);
}

void _exalt_wireless_cb(void *data, E_Menu *m, E_Menu_Item *mi)
{
	char *command1=NULL,*command2=NULL;
	int pos;
	char** info;

	info = data;

	if(!info || !exalt_config || !exalt_config->cmd)
		return ;
	pos = str_istr(exalt_config->cmd,"%i");
	command1 = str_remplace(exalt_config->cmd,pos,strlen("%i"),info[0]);
	if(command1)
	{
		pos = str_istr(command1,"%w");
		command2 = str_remplace(command1,pos,strlen("%w"),info[1]);
	}
	else
	{
		pos = str_istr(exalt_config->cmd,"%w");
		command2 = str_remplace(exalt_config->cmd,pos,strlen("%w"),info[1]);
	}
	if(command2)
		ecore_exe_run(command2,NULL);
	else if(command1)
		ecore_exe_run(command1, NULL);
	else
		ecore_exe_run(exalt_config->cmd,NULL);


	EXALT_FREE(command1);
	EXALT_FREE(command2);
}

void _exalt_notify_cb(char* interface, Exalt_Enum_Action action, void* user_data)
{
	Instance* inst = user_data;
	_popup_show(inst,interface,action);
}

	static void
_menu_cb_post(void* data, E_Menu *m)
{
	if (exalt_config->menu)
	{
		e_object_del(E_OBJECT(exalt_config->menu));
		exalt_config->menu = NULL;
	}

	if(exalt_config->interfaces)
	{
		ecore_list_destroy(exalt_config->interfaces);
		exalt_config->interfaces = NULL;
	}

	if(exalt_config->wireless)
		ecore_list_clear(exalt_config->wireless);
}

int str_istr (const char *cs, const char *ct)
{
	int index = -1;

	if (cs && ct)
	{
		char *ptr_pos = NULL;

		ptr_pos = strstr (cs, ct);
		if (ptr_pos)
		{
			index = ptr_pos - cs;
		}
	}
	return index;
}


char *str_remplace (const char *s, int start, int lenght, const char *ct)
{
	char *new_s = NULL;

	if (s && ct && start >= 0 && lenght >= 0)
	{
		size_t size = strlen (s);

		new_s = malloc (sizeof (*new_s) * (size - lenght + strlen (ct) +1));
		if (new_s)
		{
			memcpy (new_s, s, start);
			memcpy (&new_s[start], ct, strlen (ct));
			memcpy (&new_s[start + strlen (ct)], &s[start + lenght], size - lenght - start + 1);
			return new_s;
		}
		else
			return NULL;
	}
	else
		return NULL;
}




