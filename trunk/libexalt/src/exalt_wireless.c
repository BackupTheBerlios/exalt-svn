/** @file exalt_wireless.c */
#include "exalt_wireless.h"

#ifdef DIST_DEBIAN
#include "debian.h"
#else
#ifdef DIST_GENTOO
#include "gentoo.h"
#else
#include "default.h"
#endif
#endif


/**
 * @addtogroup Exalt_wireless
 * @{
 */

/**
 * Private functions headers
 */


char* _exalt_wireless_get_save_essid(exalt_wireless* w);
int _exalt_wireless_set_save_essid(exalt_wireless* w,const char* essid);


/*
 * Constructor / Destructor
 */


/**
 * @brief create a new exalt_wireless
 * @return Returns the new exalt_wireless struct
 */
exalt_wireless* exalt_wireless_create(exalt_ethernet* eth)
{
    exalt_wireless *w;
    if(!eth)
    {
        fprintf(stderr,"exalt_wireless_create(): eth==null! \n");
        return NULL;
    }

    w = (exalt_wireless*)malloc((unsigned int)sizeof(exalt_wireless));
    if(!w)
    {
        fprintf(stderr,"exalt_wireless_create(): w==null, malloc error\n");
        return NULL;
    }
    w -> eth = eth;
    w -> new_essid = NULL;
    w -> new_passwd = NULL;
    w -> new_passwd_mode = 0;

    w -> _save_essid = NULL;
    _exalt_wireless_set_save_essid(w,exalt_wireless_get_essid(w));

    //init networks list
    w -> networks = ecore_list_new();
    w->networks->free_func =  ECORE_FREE_CB(exalt_wirelessinfo_free);
    ecore_list_init(w->networks);

    w->context=(wireless_scan_head*)malloc((unsigned int)sizeof(wireless_scan_head));
    w->context->result=NULL;
    w->context->retry= 0;
    w->scan_fd = iw_sockets_open();
    w->scan_cb_timer = NULL;

    return w;
}



/**
 * @brief free a exalt_wireless
 * @param w the exalt_wireless
 */
void exalt_wireless_free(exalt_wireless* w)
{
    if(!w)
    {
        fprintf(stderr,"exalt_wireless_free(): w==null\n");
        return ;
    }
    ecore_list_destroy(w->networks);

    EXALT_FREE(w->new_essid)
        EXALT_FREE(w->new_passwd)
        EXALT_FREE(w->_save_essid)
        EXALT_CLOSE(w->scan_fd)
        EXALT_DELETE_TIMER(w->scan_cb_timer)
        EXALT_FREE(w)
}




/*
 * Set/Get functions for exalt_wireless
 */




/**
 * @brief return the current essid
 * @param w the exalt_wireless
 * @return Return the current essid
 */
char* exalt_wireless_get_essid(exalt_wireless* w)
{
    char essid[IW_ESSID_MAX_SIZE +1];
    struct iwreq wrq;
    exalt_ethernet *eth;

    if(!w)
    {
        fprintf(stderr,"exalt_wireless_get_essid(): w==null! \n");
        return NULL;
    }

    eth = exalt_wireless_get_eth(w);

    strncpy(wrq.ifr_name, exalt_eth_get_name(eth), sizeof(wrq.ifr_name));
    wrq.u.essid.pointer = (caddr_t) essid;
    wrq.u.essid.length = IW_ESSID_MAX_SIZE+1;
    wrq.u.essid.flags = 0;
    if(exalt_ioctl(&wrq, SIOCGIWESSID) < 0)
        return "";

    if(wrq.u.essid.length>0)
        return strdup((char*) wrq.u.essid.pointer);
    else
        return "";
}


/**
 * @brief return the new essid
 * @param w the exalt_wireless
 * @return Return the new essid
 */
char* exalt_wireless_get_new_essid(exalt_wireless* w)
{
    if(!w)
    {
        fprintf(stderr,"wireless_get_new_essid(): w==null! \n");
        return NULL;
    }
    return w->new_essid;
}

/**
 * @brief set the new essid
 * @param w the exalt_wireless
 * @param essid the essid which will be set
 */
void exalt_wireless_set_new_essid(exalt_wireless* w,const char* essid)
{
    if(!w || !essid)
    {
        fprintf(stderr,"wireless_set_new_essid(): w==%p  essid==%p ! \n",w,essid);
        return ;
    }

    if(!exalt_is_essid(essid))
    {
        fprintf(stderr,"wireless_set_new_essid(): essid(%s) is not a correct essid!\n",essid);
        return ;
    }
    EXALT_FREE(w->new_essid)
        w->new_essid=strdup(essid);
}

/**
 * @brief return the new password
 * @param w the exalt_wireless
 * @return Return the new passwd
 */
char* exalt_wireless_get_new_passwd(exalt_wireless* w)
{
    if(!w)
    {
        fprintf(stderr,"wireless_get_new_passwd(): w==null! \n");
        return NULL;
    }
    return w->new_passwd;
}

/**
 * @brief return the driver
 * @param w the exalt_wireless
 * @return Return the driver
 */
char* exalt_wireless_get_driver(exalt_wireless* w)
{
    if(!w)
    {
        fprintf(stderr,"exalt_wireless_get_driver(): w==null ! \n)");
        return NULL;
    }
    return exalt_sys_conf_get_driver(w);
}

/**
 * @brief set the new password
 * @param w the exalt_wireless
 * @param the new password
 */
void exalt_wireless_set_new_passwd(exalt_wireless* w,const char* passwd)
{
    if(!w || !passwd)
    {
        fprintf(stderr,"wireless_set_new_passwd(): w==%p  passwd==%p ! \n",w,passwd);
        return ;
    }

    if(!exalt_is_passwd(passwd,exalt_wireless_get_new_passwd_mode(w)))
    {
        fprintf(stderr,"wireless_set_new_passwd(): passwd(%s) is not a correct password!\n",passwd);
        return ;
    }
    EXALT_FREE(w->new_passwd)
        w->new_passwd=strdup(passwd);
}



/**
 * @brief get the new password mode {WEP hexa, WEP plain text, none, WPA ...}
 * @param w the exalt_wireless
 * @return the new password mode, -1 if a error occurs
 */
int exalt_wireless_get_new_passwd_mode(exalt_wireless* w)
{
    if(!w)
    {
        fprintf(stderr,"wireless_get_new_passwd_mode(): ==null! \n");
        return -1;
    }
    return w->new_passwd_mode;
}

/**
 * @brief get the new mode
 * @param w the exalt_wireless
 * @return the new mode, -1 if a error occurs
 */
int exalt_wireless_get_new_mode(exalt_wireless* w)
{
    if(!w)
    {
        fprintf(stderr,"wireless_get_new_mode(): ==null! \n");
        return -1;
    }
    return w->new_mode;
}

/**
 * @brief get the new security mode
 * @param w the exalt_wireless
 * @return the new security mode, -1 if a error occurs
 */
int exalt_wireless_get_new_security_mode(exalt_wireless* w)
{
    if(!w)
    {
        fprintf(stderr,"wireless_get_new_security_mode(): ==null! \n");
        return -1;
    }
    return w->new_security_mode;
}



/**
 * @brief set the new password mode {WEP hexa, WEP plain text, none}
 * @param w the exalt_wireless
 * @param passwd_mode the new password mode
 */
void exalt_wireless_set_new_passwd_mode(exalt_wireless* w,int passwd_mode)
{
    if(w)
    {
        w->new_passwd_mode = passwd_mode;
    }
}

/**
 * @brief set the new mode {Ad-hoc, Managed}
 * @param w the exalt_wireless
 * @param mode the new mode
 */
void exalt_wireless_set_new_mode(exalt_wireless* w,int mode)
{
    if(w)
    {
        w->new_mode = mode;
    }
}

/**
 * @brief set the new security mode {Open, Shared}
 * @param w the exalt_wireless
 * @param security_mode the new security mode
 */
void exalt_wireless_set_new_security_mode(exalt_wireless* w,int security_mode)
{
    if(w)
    {
        w->new_security_mode = security_mode;
    }
}


/*
 * @brief get the ethernet structure
 * @param w the exalt_wireless
 * @return Return the exalt_eth structure
 */
exalt_ethernet* exalt_wireless_get_ethernet(exalt_wireless* w)
{
    if(!w)
    {
        fprintf(stderr,"exalt_wireless_get_ethernet(): w==null! \n");
        return NULL;
    }
    return w->eth;
}



/**
 * @brief return the wireless networks list
 * @param w the wireless interface
 */
Ecore_List* exalt_wireless_get_networks_list(exalt_wireless* w)
{
    if(!w)
    {
        fprintf(stderr,"exalt_wireless_get_networks_list(): w==null ! \n");
        return NULL;
    }
    return w->networks;
}

/**
 * @brief return the exalt_ethernet struct
 * @param w the wireless interface
 * @return Return null if fail, else the exalt_ethernet struct
 */
exalt_ethernet* exalt_wireless_get_eth(exalt_wireless* w)
{
    if(!w)
    {
        fprintf(stderr,"exalt_wireless_get_eth(): w==null ! \n");
        return NULL;
    }
    return w->eth;
}

/*
 * Set/get functions for exalt_wireless_info
 */


/**
 * @brief get informations about the nb th wireless network (essid, quality ...)
 * @param w the exalt_wireless
 * @param nb the position of the wireless network
 * @return Return informations about the nb th network
 */
exalt_wireless_info* exalt_wireless_get_networkinfo(exalt_wireless* w, int nb)
{
    if(!w)
    {
        fprintf(stderr,"exalt_wireless_get_networkinfo(): w==null ! \n");
        return NULL;
    }

    return EXALT_WIRELESS_INFO(ecore_list_index_goto(w->networks,nb));
}



/**
 * @brief get informations about the wireless network named essid
 * @param w the exalt_wireless
 * @param essid the essid
 */
exalt_wireless_info* exalt_wireless_get_networkinfo_by_essid(exalt_wireless* w,char *essid)
{
    void* data;
    exalt_wireless_info* wi=NULL;

    if(!w)
    {
        fprintf(stderr,"exalt_wireless_get_networkinfo_by_essid(): w==null ! \n");
        return NULL;
    }

    ecore_list_first_goto(w->networks);
    data = ecore_list_next(w->networks);
    while(data)
    {
        wi = EXALT_WIRELESS_INFO(data);
        if(strcmp(essid, exalt_wirelessinfo_get_essid(wi))==0)
            return wi;

        data = ecore_list_next(w->networks);
    }
    return NULL;
}




/*
 * Scan functions
 */


/**
 * @brief scan networks
 * @param eth the card
 */
int exalt_wireless_scan(void *data)
{
    //see iw/ilwlib.c::iw_scan() for a example & comments
    int fd;
    wireless_scan_head *context;
    int delay; //in ms
    Ecore_List* l;
    short find;
    exalt_wireless_info *wi_n,*wi_l;
    void *data_n, *data_l;
    exalt_wireless_info* wi= NULL;
    exalt_wireless* w;

    if(!data)
    {
        fprintf(stderr,"exalt_wireless_scan(): data==null ! \n");
        return -1;
    }
    exalt_ethernet* eth = EXALT_ETHERNET(data);
    w = exalt_eth_get_wireless(eth);
    if(!w)
    {
        fprintf(stderr,"exalt_wireless_scan(): w==null! %s\n",exalt_eth_get_name(eth));
        return -1;
    }

    context=w->context;
    if(!context)
    {
        fprintf(stderr,"exalt_wireless_scan(): context==null");
        return -1;
    }

    fd = w->scan_fd;
    if(fd<0)
    {
        fprintf(stderr,"exalt_wireless_scan(): fd==%d",fd);
        return -1;
    }
    delay = iw_process_scan(fd, exalt_eth_get_name(eth), exalt_eth_interfaces.we_version, context);
    if(delay<=0)
    {
        Ecore_List* temp;
        l = ecore_list_new();
        l->free_func =  ECORE_FREE_CB(exalt_wirelessinfo_free);
        ecore_list_init(l);


        wireless_scan * result = context->result;
        while(result)
        {
            //retrieve the essid
            if(result->b.has_essid && result->b.essid_on && strcmp(result->b.essid,"<hidden>")!=0)
            {
                //Mac address
                char buf[18];
                iw_ether_ntop((struct ether_addr* )result->ap_addr.sa_data,buf);
                wi = exalt_wirelessinfo_create(exalt_eth_get_wireless(eth));
                exalt_wirelessinfo_set_address(wi,buf);
                exalt_wirelessinfo_set_essid(wi,result->b.essid);
                exalt_wirelessinfo_set_encryption(wi,result->b.has_key);
                exalt_wirelessinfo_set_scanok(wi,1);
                exalt_wirelessinfo_set_known(wi,0);

                //search if the essid is in the list
                ecore_list_first_goto(l);
                data_l = ecore_list_next(l);
                find = 0;
                while(data_l && !find)
                {
                    wi_l = EXALT_WIRELESS_INFO(data_l);
                    if(strcmp(exalt_wirelessinfo_get_essid(wi_l),exalt_wirelessinfo_get_essid(wi))==0)
                        find = 1;
                    else
                        data_l = ecore_list_next(l);
                }
                if(!find)
                {
                    ecore_list_append(l,wi);
                    exalt_wireless_load(wi);
                }

                iwqual  qual = result->stats.qual;
                exalt_wirelessinfo_set_noiselvl(wi,qual.noise);
                exalt_wirelessinfo_set_signalvl(wi,qual.level);
                exalt_wirelessinfo_set_quality(wi,qual.qual);

            }

            result = result->next;
        }

        //compare w->networks and l
        //if a network is in l & not int w->networks, it's a new network
        //if a network is in w->networks & not in l, it's a old network
        temp = w->networks;
        w->networks = l;
        l = temp;

        //new networks
        ecore_list_first_goto(w->networks);
        data_l = ecore_list_next(w->networks);
        while(data_l)
        {
            find = 0;
            wi_l = EXALT_WIRELESS_INFO(data_l);
            ecore_list_first_goto(l);
            data_n = ecore_list_next(l);
            while(data_n)
            {
                wi_n = EXALT_WIRELESS_INFO(data_n);
                if(strcmp(exalt_wirelessinfo_get_essid(wi_l),exalt_wirelessinfo_get_essid(wi_n))==0)
                {
                    //not a new network
                    data_n = NULL;
                    find = 1;
                }
                else
                    data_n = ecore_list_next(l);
            }
            if(!find && exalt_eth_interfaces.wireless_scan_cb)
                exalt_eth_interfaces.wireless_scan_cb(wi_l,EXALT_WIRELESS_SCAN_CB_ACTION_NEW,exalt_eth_interfaces.wireless_scan_cb_user_data);
            data_l = ecore_list_next(w->networks);
        }

        //old networks
        ecore_list_first_goto(l);
        data_n = ecore_list_next(l);
        while(data_n)
        {
            find = 0;
            wi_n = EXALT_WIRELESS_INFO(data_n);
            ecore_list_first_goto(w->networks);
            data_l = ecore_list_next(w->networks);
            while(data_l)
            {
                wi_l = EXALT_WIRELESS_INFO(data_l);
                if(strcmp(exalt_wirelessinfo_get_essid(wi_l),exalt_wirelessinfo_get_essid(wi_n))==0)
                {
                    //not a new network
                    data_l = NULL;
                    find = 1;
                }
                else
                    data_l = ecore_list_next(w->networks);
            }
            if(!find && exalt_eth_interfaces.wireless_scan_cb)
                exalt_eth_interfaces.wireless_scan_cb(wi_n,EXALT_WIRELESS_SCAN_CB_ACTION_REMOVE,exalt_eth_interfaces.wireless_scan_cb_user_data);

            data_n = ecore_list_next(l);
        }

        ecore_list_destroy(l);

        delay = EXALT_WIRELESS_SCAN_UPDATE_TIME*1000;

        exalt_wireless_scan_free(&(context->result));
        context->retry=0;
    }

    if(w->scan_cb_timer)
        ecore_timer_interval_set(w->scan_cb_timer,delay/1000.);


    return delay;
}



/**
 * @brief start a scan
 * @param eth the exalt_ethernet where scan
 */
void exalt_wireless_scan_start(exalt_ethernet* eth)
{
    exalt_wireless* w;

    if(!eth)
    {
        fprintf(stderr,"exalt_wireless_scan_start(): eth==null ! \n");
        return;
    }
    if(!exalt_eth_is_wireless(eth))
    {
        fprintf(stderr,"exalt_wireless_scan_start(): eth is not a wireless card! \n");
        return;
    }
    w = exalt_eth_get_wireless(eth);

    if(w->scan_cb_timer)
    {
        fprintf(stderr,"exalt_wireless_scan_start(): you can't start 2 scan in the same time !\n");
        return ;
    }
    ecore_list_clear(w->networks);
    w->scan_cb_timer = ecore_timer_add(0, exalt_wireless_scan, eth);
}



/**
 * @brief stop a scan
 */
void exalt_wireless_scan_stop(exalt_ethernet* eth)
{
    exalt_wireless* w;

    w=exalt_eth_get_wireless(eth);
    if(!w)
    {
        fprintf(stderr,"exalt_wireless_scan_stop(): eth is not a wireless card ! \n");
        return ;
    }

    if(!w->scan_cb_timer)
    {
        fprintf(stderr,"exalt_wireless_scan_stop(): no scan launch !\n");
        return ;
    }
    EXALT_DELETE_TIMER(w->scan_cb_timer)
}



/**
 * @brief launch a wireless scan
 * @param w the wireless card
 */
int exalt_wireless_scan_wait(exalt_ethernet* eth)
{
    int fd;
    wireless_scan_head *context;
    int delay; //in ms
    Ecore_List* l;
    short find;
    exalt_wireless_info *wi_l;
    void *data_l;
    exalt_wireless_info* wi= NULL;

    context=eth->wireless->context;
    if(!context)
    {
        fprintf(stderr,"exalt_wireless_scan(): context==null");
        return -1;
    }

    fd = eth->wireless->scan_fd;
    if(fd<0)
    {
        fprintf(stderr,"exalt_wireless_scan(): fd==%d",fd);
        return -1;
    }
    exalt_wireless* w= exalt_eth_get_wireless(eth);

    while(1)
    {
        delay = iw_process_scan(fd, exalt_eth_get_name(eth), exalt_eth_interfaces.we_version, context);
        if(delay<=0)
            break;
        usleep(delay*1000);
    }

    ecore_list_clear(w->networks);
    l = ecore_list_new();
    l->free_func =  ECORE_FREE_CB(exalt_wirelessinfo_free);

    wireless_scan * result = context->result;
    while(result)
    {
        //retrieve the essid
        if(result->b.has_essid && result->b.essid_on && strcmp(result->b.essid,"<hidden>")!=0)
        {
            //Mac address
            char buf[18];
            iw_ether_ntop((struct ether_addr* )result->ap_addr.sa_data,buf);
            wi = exalt_wirelessinfo_create(exalt_eth_get_wireless(eth));
            exalt_wirelessinfo_set_address(wi,buf);
            exalt_wirelessinfo_set_essid(wi,result->b.essid);
            exalt_wirelessinfo_set_encryption(wi,result->b.has_key);
            exalt_wirelessinfo_set_scanok(wi,1);
            exalt_wirelessinfo_set_known(wi,0);

            //search if the essid is in the list
            ecore_list_first_goto(l);
            data_l = ecore_list_next(l);
            find = 0;
            while(data_l && !find)
            {
                wi_l = EXALT_WIRELESS_INFO(data_l);
                if(strcmp(exalt_wirelessinfo_get_essid(wi_l),exalt_wirelessinfo_get_essid(wi))==0)
                    find = 1;
                else
                    data_l = ecore_list_next(l);
            }
            if(!find)
            {
                ecore_list_append(l,wi);
                exalt_wireless_load(wi);
            }

            iwqual  qual = result->stats.qual;
            exalt_wirelessinfo_set_noiselvl(wi,qual.noise);
            exalt_wirelessinfo_set_signalvl(wi,qual.level);
            exalt_wirelessinfo_set_quality(wi,qual.qual);

        }

        result = result->next;
    }

    ecore_list_destroy(w->networks);
    w->networks = l;

    exalt_wireless_scan_free(&(context->result));
    context->retry=0;

    return delay;
}


/*
 * Others
 */


/**
 * @brief get the radio button state
 * @param eth the card
 * @return Return -1 if a error is occurs, 0 if the button is off, 1 if it is on
 */
short exalt_wireless_radiobutton_ison(exalt_wireless *w)
{
    struct iwreq wrq;
    if(!w)
    {
        fprintf(stderr,"exalt_wireless_radiobutton_ison(): w==null\n");
        return EXALT_ERROR;
    }

    strncpy(wrq.ifr_name, exalt_eth_get_name(exalt_wireless_get_eth(w)), sizeof(wrq.ifr_name));
    if(exalt_ioctl(&wrq, SIOCGIWNAME) < 0)
        return EXALT_ERROR;

    if(strcmp("radio off",wrq.u.name)==0)
        return EXALT_FALSE;
    else
        return EXALT_TRUE;
}



/**
 * @brief apply the configuration (the current essid, current password ...)
 * @param eth the card
 * @return Return 1 if the configuration is apply, -1 if a error is occurs
 */
int exalt_wireless_apply_conf(exalt_wireless *w)
{
    struct wpa_ctrl *ctrl_conn;
    exalt_ethernet* eth;
    int passwd_mode;
    char essid[IW_ESSID_MAX_SIZE+1];
    struct iwreq wrq;
    unsigned char key[IW_ENCODING_TOKEN_MAX];
    char buf[1024];
    int keylen = 0;

    if(!w)
    {
        fprintf(stderr,"exalt_wireless_apply_conf(): w==null ! \n");
        return -1;
    }
    eth = exalt_wireless_get_eth(w);

    if(!exalt_is_admin())
    {
        fprintf(stderr,"exalt_wireless_apply_conf(): you need to be root if you want save the configuration file! \n");
        return -1;
    }

    passwd_mode = exalt_wireless_get_new_passwd_mode(w);
    switch(passwd_mode)
    {
        case EXALT_WIRELESS_ENCRYPTION_NONE:
        case EXALT_WIRELESS_ENCRYPTION_WEP_ASCII:
        case EXALT_WIRELESS_ENCRYPTION_WEP_HEXA:
            //we need to stop the daemon wpa_supplicant
            ctrl_conn = exalt_wpa_open_connection(exalt_eth_get_name(eth));
            if(ctrl_conn)
            {
                exalt_wpa_ctrl_command(ctrl_conn, "TERMINATE");
                wpa_ctrl_close(ctrl_conn);
                ctrl_conn=NULL;
                //the wpa_supplicant daemon deactivate the card ...
                while(exalt_eth_is_up(eth))
                    ;
                exalt_eth_up(eth);
            }

            strncpy(wrq.ifr_name, exalt_eth_get_name(eth), sizeof(wrq.ifr_name));
            wrq.u.essid.flags = 0;
            wrq.u.data.length = 0;
            wrq.u.data.pointer = (caddr_t) NULL;

            //set the key and the security mode
            switch(passwd_mode)
            {
                case EXALT_WIRELESS_ENCRYPTION_NONE:
                         keylen = 0;
                         wrq.u.data.flags |= IW_ENCODE_DISABLED;
                    break;
                case EXALT_WIRELESS_ENCRYPTION_WEP_ASCII:
                        keylen = sprintf(buf,"s:%s",exalt_wireless_get_new_passwd(w));
                        iw_in_key(buf,key);
                    break;
                case EXALT_WIRELESS_ENCRYPTION_WEP_HEXA:
                        keylen = iw_in_key(exalt_wireless_get_new_passwd(w),key);
                    break;
            }

            if(keylen > 0)
            {
                wrq.u.data.length = keylen;
                wrq.u.data.pointer = (caddr_t) key;
            }
            else
                wrq.u.data.flags |= IW_ENCODE_NOKEY;

            switch(exalt_wireless_get_new_security_mode(w))
            {
                case EXALT_WIRELESS_SECURITY_OPEN:
                    wrq.u.data.flags |= IW_ENCODE_OPEN;
                    break;
                default:
                    wrq.u.data.flags |= IW_ENCODE_RESTRICTED;
                    break;
            }

            if(exalt_ioctl(&wrq, SIOCSIWENCODE) < 0)
                return -1;

            //set the mode (Ad-hoc, Managed)
            switch(exalt_wireless_get_new_mode(w))
            {
                case EXALT_WIRELESS_MODE_ADHOC:
                    wrq.u.mode = 1;
                    break;
                case EXALT_WIRELESS_MODE_MANAGED:
                    wrq.u.mode = 2;
                    break;
            }
            if(exalt_ioctl(&wrq, SIOCSIWMODE) < 0)
                return -1;



            //set the essid
            strncpy(essid, exalt_wireless_get_new_essid(w), IW_ESSID_MAX_SIZE+1);
            wrq.u.essid.pointer = (caddr_t) essid;
            wrq.u.essid.length = strlen(essid)+1;
            if(exalt_ioctl(&wrq, SIOCSIWESSID) < 0)
                return -1;

            break;
        default:
            //reload wpa_supplicant configuration
            ctrl_conn = exalt_wpa_open_connection(exalt_eth_get_name(eth));
            if(!ctrl_conn)
            {
                char buf[1024];
                FILE* f;
                fprintf(stderr,"Could not connect to wpa_supplicant, try to launch it\n");
                sprintf(buf,COMMAND_WPA,
                        exalt_wireless_get_driver(exalt_eth_get_wireless(eth)),
                        exalt_eth_get_name(eth),
                        WPA_CONF_FILE,
                        EXALT_WPA_IFACE_DIR);
                f = exalt_execute_command(buf);
                while(fgets(buf,1024,f))
                    ;
                EXALT_PCLOSE(f)
                    fprintf(stderr,"Re-try to connect to wpa_supplicant\n");
                ctrl_conn = exalt_wpa_open_connection(exalt_eth_get_name(eth));
                if(!ctrl_conn)
                {
                    fprintf(stderr,"Could not connect to wpa_supplicant, abandon\n");
                    return -1;
                }
                else
                    fprintf(stderr,"Connection succesfull\n");
            }

            if( !exalt_wpa_ctrl_command(ctrl_conn, "RECONFIGURE"))
                fprintf(stderr,"exalt_wireless_apply_conf(): wpa reconfigure failed\n");

            //close the connection
            wpa_ctrl_close(ctrl_conn);
            ctrl_conn=NULL;
            break;
    }
    //save the configuration of the current network
    exalt_wireless_save(exalt_eth_get_wireless(eth));

    return 1;
}



/**
 * @brief Print all informations about the card in stdout
 * @param w the exalt_wireless
 */
void exalt_wireless_printf(exalt_wireless w)
{
    printf("\t## Connected essid: %s\n",exalt_wireless_get_essid(&w));
    printf("\t## Radio button: %s\n",(exalt_wireless_radiobutton_ison(&w)>0?"on":"off"));
    exalt_wireless_printf_scan(w);
}



/**
 * @brief Print a scan result in stdout
 * @param w the exalt_wireless
 */
void exalt_wireless_printf_scan(exalt_wireless w)
{
    void* data;
    ecore_list_first_goto(w.networks);
    data = ecore_list_next(w.networks);
    while(data)
    {
        exalt_wireless_info* wi = EXALT_WIRELESS_INFO(data);
        printf("\n\t\tAddress: %s\n",exalt_wirelessinfo_get_addr(wi));
        printf("\t\tEssid: %s\n",exalt_wirelessinfo_get_essid(wi));
        printf("\t\tKnown: %s\n",(exalt_wirelessinfo_is_known(wi)==1?"yes":"no"));
        printf("\t\tScan ok: %s\n",(exalt_wirelessinfo_is_scan(wi)==1?"yes":"no"));
        printf("\t\tProtocol: %s\n",exalt_wirelessinfo_get_protocol(wi));
        printf("\t\tMode: %s\n",exalt_wirelessinfo_get_mode(wi));
        printf("\t\tChannel: %s\n",exalt_wirelessinfo_get_channel(wi));
        printf("\t\tEncryption: %d\n",exalt_wirelessinfo_get_encryption(wi));
        printf("\t\tBit rates: %s\n",exalt_wirelessinfo_get_bitrates(wi));
        printf("\t\tQuality: %d\n",exalt_wirelessinfo_get_quality(wi));
        printf("\t\tSignal lvl: %d\n",exalt_wirelessinfo_get_signallvl(wi));
        printf("\t\tNoise lvl: %d\n",exalt_wirelessinfo_get_noiselvl(wi));
    }
}




/*
 * iwlib free
 */


/**
 * @brief free a scan result of iwlib
 * @param w the first result of the list result
 */
void exalt_wireless_scan_free(wireless_scan **ws)
{
    wireless_scan* result,*r_save;
    if(!ws)
    {
        fprintf(stderr,"exalt_wireless_scan_free(): ws==null ! \n");
        return ;
    }

    result = *ws;

    while(result)
    {
        r_save = result;
        result = result->next;
        free(r_save);
    }
    *ws=NULL;
}


/*
 * wpa_supplicant functions
 */


/**
 * @brief open a connection with the wpa daemon
 * @param ifname the interface name (eth1 ...)
 * @return Return the connection else NULL
 */
struct wpa_ctrl * exalt_wpa_open_connection(const char *ifname)
{
    char *cfile;
    int flen;
    struct wpa_ctrl* ctrl_conn;
    if (ifname == NULL)
    {
        fprintf(stderr,"exalt_wpa_open_connection(): ifname == null ! \n");
        return NULL;
    }

    flen = strlen(EXALT_WPA_IFACE_DIR) + strlen(ifname) + 2;
    cfile = malloc(flen);
    if (cfile == NULL)
        return NULL;
    snprintf(cfile, flen, "%s/%s", EXALT_WPA_IFACE_DIR, ifname);
    ctrl_conn = wpa_ctrl_open(cfile);
    free(cfile);
    return ctrl_conn;
}



/**
 * @brief send a command to the wpa_supplicant daemon
 * @param wpa_ctrl the connection to the wpa_supplicant daemon
 * @param cmd the command (RECONFIGURE for ex)
 * @return Return 1 if ok, -2 if timeout, -1 if failed
 */
int exalt_wpa_ctrl_command(struct wpa_ctrl *ctrl_conn, char *cmd)
{
    char buf[2048];
    size_t len;
    int ret;

    if (ctrl_conn == NULL) {
        fprintf(stderr,"exalt_wpa_ctrl_command(): ctrl_conn==null !\n");
        return -1;
    }
    len = sizeof(buf) - 1;
    ret = wpa_ctrl_request(ctrl_conn, cmd, strlen(cmd), buf, &len,
            /*wpa_cli_msg_cb*/ NULL);
    if (ret == -2) {
        fprintf(stderr,"'%s' wpa command timed out.\n", cmd);
        return -2;
    } else if (ret < 0) {
        fprintf(stderr,"'%s' wpa command failed.\n", cmd);
        return -1;
    }
    return 1;
}


int exalt_rtlink_essid_change(exalt_wireless *w)
{
   char* essid, *save_essid;

   if(!w)
   {
    fprintf(stderr,"exalt_rtlink_essid_change(): w==null !\n");
    return -1;
   }

   essid = exalt_wireless_get_essid(w);
   save_essid = _exalt_wireless_get_save_essid(w);

    printf("(%s)(%s)\n",essid,save_essid);

    if(   (!essid && save_essid)
            || (essid && !save_essid)
            || (essid && save_essid && strcmp(essid,save_essid)!=0))
    {
        _exalt_wireless_set_save_essid(w,exalt_wireless_get_essid(w));
        if(exalt_eth_interfaces.eth_cb)
            exalt_eth_interfaces.eth_cb(exalt_wireless_get_eth(w),EXALT_WIRELESS_CB_ACTION_ESSIDCHANGE,exalt_eth_interfaces.eth_cb_user_data);
        return 1;
    }

    return 0;
}



/**
 * Private functions bodies
 */



/**
 * @brief set the save essid of the card "w"
 * @param w the card
 * @param essid the new essid
 * @return Returns 1 if the save essid is apply, else -1
 */
int _exalt_wireless_set_save_essid(exalt_wireless* w,const char* essid)
{
 	if(!w || !essid)
	{
	 	fprintf(stderr,"_exalt_wireless_set_save_essid(): w==%p essid==%p !\n",w,essid);
		return -1;
	}

	EXALT_FREE(w->_save_essid)
	w->_save_essid=strdup(essid);
	return 1;
}

/**
 * @brief get the save essid of the card "w"
 * @param w the card
 * @return Returns the save essid
 */
char* _exalt_wireless_get_save_essid(exalt_wireless* w)
{
    if(!w)
    {
        fprintf(stderr,"_exalt_wireless_get_save_essid(): w==null ! \n");
        return NULL;
    }
    else
        return w->_save_essid;
}

/** @} */


