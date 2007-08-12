
#include "info_panel.h"

info_panel* infopanel_create()
{
    Etk_Widget *hbox,*vbox,*frame,*lbl;
    info_panel* info_pnl;
    int nb_dns,i;
    char buf[1024];
    char** dns;
    info_pnl = malloc(sizeof(info_panel));

    info_pnl->frame  = etk_frame_new(_("Current configuration"));

    vbox = etk_vbox_new(ETK_FALSE,5);
    etk_container_add(ETK_CONTAINER(info_pnl->frame), vbox);

    info_pnl->hbox = etk_hbox_new(ETK_FALSE,5);
    etk_container_add(ETK_CONTAINER(vbox), info_pnl->hbox);


    frame  = etk_frame_new(_("DNS configuration"));
    etk_box_append(ETK_BOX(vbox), frame, ETK_BOX_START, ETK_BOX_EXPAND_FILL, 0);
    hbox = etk_hbox_new(ETK_FALSE,5);
    etk_container_add(ETK_CONTAINER(frame), hbox);

    dns = exalt_dns_get_list(&nb_dns);
    for(i=0;i<nb_dns;i++)
    {
        sprintf(buf,"DNS%d: %s",i+1,dns[i]);
        lbl = etk_label_new(buf);
        etk_box_append(ETK_BOX(hbox),lbl, ETK_BOX_START, ETK_BOX_EXPAND_FILL, 0);
    }

    etk_box_append(ETK_BOX(vbox),etk_label_new(_("TIPS: If you want modify your configuration, you have to launch Exalt as root")), ETK_BOX_START, ETK_BOX_EXPAND_FILL, 0);



    return info_pnl;
}


void infopanel_eth_cb(exalt_ethernet* eth, int action, void* user_data)
{
    main_window* win =  (main_window*) user_data;
    Etk_Widget *frame,*table , *hbox, *label;
    char buf[1024];
    int i;

    if(action!= EXALT_ETH_CB_ACTION_NEW)
        return ;

    hbox = win->info_panel->hbox;

    if(exalt_eth_is_wireless(eth))
        sprintf(buf,_("Wireless card: %s"), exalt_eth_get_name(eth));
    else
        sprintf(buf,_("Network card: %s"), exalt_eth_get_name(eth));

    frame = etk_frame_new(buf);
    etk_box_append(ETK_BOX(hbox), frame, ETK_BOX_START, ETK_BOX_EXPAND_FILL, 0);

    if(exalt_eth_is_wireless(eth))
    {
        table = etk_table_new(2, 4,ETK_TABLE_HOMOGENEOUS);
        label = etk_label_new(_("Essid: "));
 	etk_table_attach(ETK_TABLE(table), label, 0, 0, 0, 0, 0, 0, ETK_TABLE_HFILL);
	etk_table_attach_default(ETK_TABLE(table), etk_label_new(exalt_wireless_get_essid(exalt_eth_get_wireless(eth))), 1, 1, 0, 0);
        i = 1;
    }
    else
    {
        table = etk_table_new(2, 3,ETK_TABLE_HOMOGENEOUS);
        i=0;
    }
    etk_container_add(ETK_CONTAINER(frame), table);

    label = etk_label_new(_("IP address: "));
    etk_table_attach(ETK_TABLE(table), label, 0, 0, i, i, 0, 0, ETK_TABLE_HFILL);
    etk_table_attach_default(ETK_TABLE(table), etk_label_new(exalt_eth_get_ip(eth)), 1, 1, i, i);

    i++;
    label = etk_label_new(_("Network mask: "));
    etk_table_attach(ETK_TABLE(table), label, 0, 0, i, i, 0, 0, ETK_TABLE_HFILL);
    etk_table_attach_default(ETK_TABLE(table), etk_label_new(exalt_eth_get_netmask(eth)), 1, 1, i, i);

    i++;
    label = etk_label_new(_("Gateway: "));
    etk_table_attach(ETK_TABLE(table), label, 0, 0, i, i, 0, 0, ETK_TABLE_HFILL);
    etk_table_attach_default(ETK_TABLE(table), etk_label_new(exalt_eth_get_gateway(eth)), 1, 1, i, i);


    etk_widget_show_all(frame);
}


