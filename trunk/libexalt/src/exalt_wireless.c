/** @file Exalt_Wireless.c */
#include "exalt_wireless.h"
#include "libexalt_private.h"
#include "../config.h"

/**
 * Private functions headers
 */

void _exalt_wireless_scan_free(wireless_scan **w);
char* _exalt_wireless_get_save_essid(Exalt_Wireless* w);
int _exalt_wireless_set_save_essid(Exalt_Wireless* w,const char* essid);

int _exalt_wireless_scan(void* data);

struct wpa_ctrl * _exalt_wpa_open_connection(const char *ifname);
int _exalt_wpa_ctrl_command(struct wpa_ctrl *ctrl, char *cmd);

/**
 * @addtogroup Exalt_Wireless
 * @{
 */


struct Exalt_Wireless
{
    Exalt_Ethernet* eth;
    char* wpasupplicant_driver;

    Ecore_List* networks;

    char* _save_essid;

    //use for scanning
    wireless_scan_head *context;
    int scan_fd;
    int scanning;
};




/*
 * Constructor / Destructor
 */


/**
 * @brief create a new Exalt_Wireless
 * @return Returns the new Exalt_Wireless struct
 */
Exalt_Wireless* exalt_wireless_new(Exalt_Ethernet* eth)
{
    Exalt_Wireless *w;
    char* str;

    EXALT_ASSERT_RETURN(eth!=NULL);

    w = (Exalt_Wireless*)malloc((unsigned int)sizeof(Exalt_Wireless));
    EXALT_ASSERT_RETURN(w!=NULL);

    w -> eth = eth;

    w->wpasupplicant_driver = NULL;
    w -> _save_essid = NULL;

    str = exalt_wireless_get_essid(w);
    _exalt_wireless_set_save_essid(w,str);
    EXALT_FREE(str);

    //init networks list
    w -> networks = ecore_list_new();
    w->networks->free_func =  ECORE_FREE_CB(exalt_wirelessinfo_free);
    ecore_list_init(w->networks);

    w->context=(wireless_scan_head*)malloc((unsigned int)sizeof(wireless_scan_head));
    w->context->result=NULL;
    w->context->retry= 0;
    w->scan_fd = iw_sockets_open();
    w->scanning = 0;

    //default driver
    EXALT_STRDUP(w->wpasupplicant_driver,"wext");
    return w;
}



/**
 * @brief free a Exalt_Wireless
 * @param w the Exalt_Wireless
 */
void exalt_wireless_free(Exalt_Wireless* w)
{
    EXALT_ASSERT_RETURN_VOID(w!=NULL);
    ecore_list_destroy(w->networks);

    EXALT_FREE(w->_save_essid);
    EXALT_FREE(w->wpasupplicant_driver);
    EXALT_CLOSE(w->scan_fd);
    EXALT_FREE(w);
}




/*
 * Set/Get functions for Exalt_Wireless
 */




/**
 * @brief return the current essid
 * @param w the Exalt_Wireless
 * @return Return the current essid
 */
char* exalt_wireless_get_essid(Exalt_Wireless* w)
{
    char essid[IW_ESSID_MAX_SIZE +1];
    struct iwreq wrq;
    Exalt_Ethernet *eth;

    EXALT_ASSERT_RETURN(w!=NULL);

    eth = exalt_wireless_get_eth(w);

    strncpy(wrq.ifr_name, exalt_eth_get_name(eth), sizeof(wrq.ifr_name));
    wrq.u.essid.pointer = (caddr_t) essid;
    wrq.u.essid.length = IW_ESSID_MAX_SIZE+1;
    wrq.u.essid.flags = 0;
    if(!exalt_ioctl(&wrq, SIOCGIWESSID))
        return NULL;

    if(wrq.u.essid.length>0)
        return strdup((char*) wrq.u.essid.pointer);
    else
        return NULL;
}

/**
 * @brief set the driver used by wpa_supplicant
 * @param w the Exalt_Wireless
 * @param driver the driver (wext ...)
 * @return 1 if the driver is set, else 0
 */
int exalt_wireless_set_wpasupplicant_driver(Exalt_Wireless* w, const char* driver)
{
    EXALT_ASSERT_RETURN(w!=NULL);
    EXALT_ASSERT_RETURN(driver!=NULL);
    EXALT_FREE(w->wpasupplicant_driver);
    w->wpasupplicant_driver = strdup(driver);
    return 1;
}

/**
 * @brief return the driver used by wpa_supplicant
 * @param w the Exalt_Wireless
 * @return Return the driver
 */
char* exalt_wireless_get_wpasupplicant_driver(Exalt_Wireless* w)
{
    EXALT_ASSERT_RETURN(w!=NULL);
    return w->wpasupplicant_driver;
}

/*
 * @brief get the ethernet structure
 * @param w the Exalt_Wireless
 * @return Return the exalt_eth structure
 */
Exalt_Ethernet* exalt_wireless_get_ethernet(Exalt_Wireless* w)
{
    EXALT_ASSERT_RETURN(w!=NULL);
    return w->eth;
}



/**
 * @brief return the wireless networks list
 * @param w the wireless interface
 */
Ecore_List* exalt_wireless_get_networks_list(Exalt_Wireless* w)
{
    EXALT_ASSERT_RETURN(w!=NULL);
    return w->networks;
}

/**
 * @brief return the Exalt_Ethernet struct
 * @param w the wireless interface
 * @return Return null if fail, else the Exalt_Ethernet struct
 */
Exalt_Ethernet* exalt_wireless_get_eth(Exalt_Wireless* w)
{
    EXALT_ASSERT_RETURN(w!=NULL);
    return w->eth;
}

/*
 * Set/get functions for Exalt_Wireless_Info
 */


/**
 * @brief get informations about the nb th wireless network (essid, quality ...)
 * @param w the Exalt_Wireless
 * @param nb the position of the wireless network
 * @return Return informations about the nb th network
 */
Exalt_Wireless_Info* exalt_wireless_get_networkinfo(Exalt_Wireless* w, int nb)
{
    EXALT_ASSERT_RETURN(w!=NULL);
    return Exalt_Wireless_Info(ecore_list_index_goto(w->networks,nb));
}



/**
 * @brief get informations about the wireless network named essid
 * @param w the Exalt_Wireless
 * @param essid the essid
 */
Exalt_Wireless_Info* exalt_wireless_get_networkinfo_by_essid(Exalt_Wireless* w,const char *essid)
{
    void* data;
    Exalt_Wireless_Info* wi=NULL;

    EXALT_ASSERT_RETURN(w!=NULL);
    EXALT_ASSERT_RETURN(essid!=NULL);

    ecore_list_first_goto(w->networks);
    data = ecore_list_next(w->networks);
    while(data)
    {
        wi = Exalt_Wireless_Info(data);
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
 * @brief start a scan
 * @param eth the Exalt_Ethernet where scan
 */
void exalt_wireless_scan_start(Exalt_Ethernet* eth)
{
    Exalt_Wireless* w;

    EXALT_ASSERT_RETURN_VOID(eth!=NULL);
    EXALT_ASSERT_RETURN_VOID(exalt_eth_is_wireless(eth));
    w = exalt_eth_get_wireless(eth);
    if(w->scanning)
        return ;

    w->scanning = 1;
    ecore_timer_add(0, _exalt_wireless_scan, eth);
}


/**
 * @brief launch a wireless scan
 * @param w the wireless card
 */
int exalt_wireless_scan_wait(Exalt_Ethernet* eth)
{
    int fd;
    wireless_scan_head *context;
    int delay; //in ms
    Ecore_List* l;
    short find;
    Exalt_Wireless_Info *wi_l;
    void *data_l;
    Exalt_Wireless* w;
    Exalt_Wireless_Info* wi= NULL;

    w = exalt_eth_get_wireless(eth);
    context=w->context;
    EXALT_ASSERT_RETURN(context!=NULL);

    fd = w->scan_fd;
    EXALT_ASSERT_RETURN(w!=0);

    while(1)
    {
        char *cpy = strdup(exalt_eth_get_name(eth));
        delay = iw_process_scan(fd, cpy, exalt_eth_interfaces.we_version, context);
        EXALT_FREE(cpy);
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
                wi_l = Exalt_Wireless_Info(data_l);
                if(strcmp(exalt_wirelessinfo_get_essid(wi_l),exalt_wirelessinfo_get_essid(wi))==0)
                    find = 1;
                else
                    data_l = ecore_list_next(l);
            }
            if(!find)
            {
                ecore_list_append(l,wi);

                iwqual  qual = result->stats.qual;
                exalt_wirelessinfo_set_noiselvl(wi,qual.noise);
                exalt_wirelessinfo_set_signalvl(wi,qual.level);
                exalt_wirelessinfo_set_quality(wi,qual.qual);
            }
            else
                exalt_wirelessinfo_free(wi);
        }

        result = result->next;
    }

    ecore_list_destroy(w->networks);
    w->networks = l;

    _exalt_wireless_scan_free(&(context->result));
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
short exalt_wireless_radiobutton_ison(Exalt_Wireless *w)
{
    struct iwreq wrq;
    EXALT_ASSERT_RETURN(w!=NULL);

    strncpy(wrq.ifr_name, exalt_eth_get_name(exalt_wireless_get_eth(w)), sizeof(wrq.ifr_name));
    if(!exalt_ioctl(&wrq, SIOCGIWNAME))
        return -1;

    if(strcmp("radio off",wrq.u.name)==0)
        return 0;
    else
        return 1;
}



/**
 * @brief apply the connection (the current essid, current password ...)
 * @param eth the card
 * @return Return 1 if the configuration is apply, -1 if a error is occurs
 */
int exalt_wireless_apply_conn(Exalt_Wireless *w)
{
    struct wpa_ctrl *ctrl_conn;
    Exalt_Ethernet* eth;
    Exalt_Connection* c;
    Exalt_Enum_Encryption_Mode encryption_mode;
    char essid[IW_ESSID_MAX_SIZE+1];
    struct iwreq wrq;
    unsigned char key[IW_ENCODING_TOKEN_MAX];
    char buf[1024];
    int keylen = 0;

    EXALT_ASSERT_RETURN(w!=NULL);
    eth = exalt_wireless_get_eth(w);

    c = exalt_eth_get_connection(eth);
    EXALT_ASSERT_RETURN(exalt_conn_is_valid(c));
    EXALT_ASSERT_RETURN(exalt_conn_is_wireless(c));

    encryption_mode = exalt_conn_get_encryption_mode(c);
    switch(encryption_mode)
    {
        case EXALT_ENCRYPTION_NONE:
        case EXALT_ENCRYPTION_WEP_ASCII:
        case EXALT_ENCRYPTION_WEP_HEXA:
#ifdef  HAVE_WPA_SUPPLICANT
            _exalt_wpa_stop(w);
#endif
            strncpy(wrq.ifr_name, exalt_eth_get_name(eth), sizeof(wrq.ifr_name));
            wrq.u.essid.flags = 0;
            wrq.u.data.length = 0;
            wrq.u.data.pointer = (caddr_t) NULL;

            //set the key and the security mode
            switch(encryption_mode)
            {
                case EXALT_ENCRYPTION_NONE:
                    keylen = 0;
                    wrq.u.data.flags |= IW_ENCODE_DISABLED;
                    break;
                case EXALT_ENCRYPTION_WEP_ASCII:
                    keylen = sprintf(buf,"s:%s", exalt_conn_get_key(c));
                    iw_in_key(buf,key);
                    break;
                case EXALT_ENCRYPTION_WEP_HEXA:
                    keylen = iw_in_key(exalt_conn_get_key(c),key);
                    break;
                default:
                    break;
            }

            if(keylen > 0)
            {
                wrq.u.data.length = keylen;
                wrq.u.data.pointer = (caddr_t) key;
            }
            else
                wrq.u.data.flags |= IW_ENCODE_NOKEY;

            switch(exalt_conn_get_security_mode(c))
            {
                case EXALT_SECURITY_OPEN:
                    wrq.u.data.flags |= IW_ENCODE_OPEN;
                    break;
                default:
                    wrq.u.data.flags |= IW_ENCODE_RESTRICTED;
                    break;
            }

            if(!exalt_ioctl(&wrq, SIOCSIWENCODE))
                return -1;

            //set the mode (Ad-hoc, Managed)
            switch(exalt_conn_get_connection_mode(c))
            {
                case EXALT_CONNECTION_ADHOC:
                    wrq.u.mode = 1;
                    break;
                case EXALT_CONNECTION_MANAGED:
                    wrq.u.mode = 2;
                    break;
            }
            if(!exalt_ioctl(&wrq, SIOCSIWMODE))
                return -1;



            //set the essid
            strncpy(essid, exalt_conn_get_essid(c), IW_ESSID_MAX_SIZE+1);
            wrq.u.essid.pointer = (caddr_t) essid;
            wrq.u.essid.length = strlen(essid)+1;
            if(!exalt_ioctl(&wrq, SIOCSIWESSID))
                return -1;

            break;
        default:
#ifdef  HAVE_WPA_SUPPLICANT
            exalt_conf_save_wpasupplicant(w);
            //reload wpa_supplicant configuration
            //we stop the current instance, because maybe we want use a different driver
            _exalt_wpa_stop(w);
            ctrl_conn = _exalt_wpa_open_connection(exalt_eth_get_name(eth));
            if(!ctrl_conn)
            {
                int status;
                Ecore_Exe *exe;
                print_error(__FILE__,__func__,__LINE__,"Could not connect to wpa_supplicant, try to launch it");
                sprintf(buf,COMMAND_WPA,
                        exalt_wireless_get_wpasupplicant_driver(exalt_eth_get_wireless(eth)),
                        exalt_eth_get_name(eth),
                        EXALT_WPA_CONF_FILE,
                        EXALT_WPA_INTERFACE_DIR);
                //printf("%s\n",buf);
                exe = ecore_exe_run(buf, NULL);
                waitpid(ecore_exe_pid_get(exe), &status, 0);
                ecore_exe_free(exe);
                print_error(__FILE__,__func__,__LINE__,"Re-try to connect to wpa_supplicant");
                ctrl_conn = _exalt_wpa_open_connection(exalt_eth_get_name(eth));
                EXALT_ASSERT_RETURN(ctrl_conn!=NULL);
                print_error(__FILE__,__func__,__LINE__,"Connection succesfull");
            }

            EXALT_ASSERT_RETURN(_exalt_wpa_ctrl_command(ctrl_conn, "RECONFIGURE"));

            //close the connection
            wpa_ctrl_close(ctrl_conn);
            ctrl_conn=NULL;
#else
            EXALT_ASSERT_ADV(0,,"Your build of libexalt doesn't support wpa_supplicant");
#endif
            break;
    }

    return 1;
}



/**
 * @brief Print all informations about the card in stdout
 * @param w the Exalt_Wireless
 */
void exalt_wireless_printf(Exalt_Wireless *w)
{
    printf("\t## Connected essid: %s\n",exalt_wireless_get_essid(w));
    printf("\t## Radio button: %s\n",(exalt_wireless_radiobutton_ison(w)>0?"on":"off"));
    exalt_wireless_printf_scan(w);
}



/**
 * @brief Print a scan result in stdout
 * @param w the Exalt_Wireless
 */
void exalt_wireless_printf_scan(Exalt_Wireless *w)
{
    void* data;
    ecore_list_first_goto(w->networks);
    data = ecore_list_next(w->networks);
    while(data)
    {
        Exalt_Wireless_Info* wi = Exalt_Wireless_Info(data);
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




/** @} */



/**
 * Private functions bodies
 */

/**
 * @brief open a connection with the wpa daemon
 * @param ifname the interface name (eth1 ...)
 * @return Return the connection else NULL
 */
struct wpa_ctrl * _exalt_wpa_open_connection(const char *ifname)
{
    char *cfile;
    int flen;
    struct wpa_ctrl* ctrl_conn;
    EXALT_ASSERT_RETURN(ifname!=NULL);

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
 * @brief kill a wpa_supplicant daemon if he exists
 * @param w the wireless card
 */
void _exalt_wpa_stop(Exalt_Wireless* w)
{
#ifdef  HAVE_WPA_SUPPLICANT
    struct wpa_ctrl *ctrl_conn;
    Exalt_Ethernet* eth;
    EXALT_ASSERT_RETURN_VOID(w!=NULL);
    eth = exalt_wireless_get_eth(w);

    ctrl_conn = _exalt_wpa_open_connection(exalt_eth_get_name(eth));
    if(ctrl_conn)
    {
        _exalt_wpa_ctrl_command(ctrl_conn, "TERMINATE");
        wpa_ctrl_close(ctrl_conn);
        ctrl_conn=NULL;
        //the wpa_supplicant daemon deactivate the card ...
        while(exalt_eth_is_up(eth))
            ;
        exalt_eth_up(eth);
    }
#endif
}


/**
 * @brief send a command to the wpa_supplicant daemon
 * @param wpa_ctrl the connection to the wpa_supplicant daemon
 * @param cmd the command (RECONFIGURE for ex)
 * @return Return 1 if ok, else 0
 */
int _exalt_wpa_ctrl_command(struct wpa_ctrl *ctrl_conn, char *cmd)
{
    char buf[2048];
    size_t len;
    int ret;

    EXALT_ASSERT_RETURN(ctrl_conn!=NULL);
    len = sizeof(buf) - 1;
    ret = wpa_ctrl_request(ctrl_conn, cmd, strlen(cmd), buf, &len,
            /*wpa_cli_msg_cb*/ NULL);

    EXALT_ASSERT_RETURN(ret>=0);

    return 1;
}

/**
 * @brief called by _exalt_rtlink_watch_cb when the kernel send a information
 */
int _exalt_rtlink_essid_change(Exalt_Wireless *w)
{
    char* essid, *save_essid;

    EXALT_ASSERT_RETURN(w!=NULL);

    essid = exalt_wireless_get_essid(w);
    save_essid = _exalt_wireless_get_save_essid(w);

    if(   (!essid && save_essid)
            || (essid && !save_essid)
            || (essid && save_essid && strcmp(essid,save_essid)!=0))
    {
        _exalt_wireless_set_save_essid(w,exalt_wireless_get_essid(w));
        if(exalt_eth_interfaces.eth_cb)
            exalt_eth_interfaces.eth_cb(exalt_wireless_get_eth(w),EXALT_WIRELESS_CB_ACTION_ESSIDCHANGE,exalt_eth_interfaces.eth_cb_user_data);
        EXALT_FREE(essid);
        return 1;
    }

    EXALT_FREE(essid);
    return 0;
}

/**
 * @brief set the save essid of the card "w"
 * @param w the card
 * @param essid the save essid
 * @return Returns 1 if the save essid is apply, else -1
 */
int _exalt_wireless_set_save_essid(Exalt_Wireless* w,const char* essid)
{
    EXALT_ASSERT_RETURN(w!=NULL);
    EXALT_ASSERT_RETURN(essid!=NULL);

    EXALT_FREE(w->_save_essid);
    w->_save_essid=strdup(essid);
    return 1;
}

/**
 * @brief get the save essid of the card "w"
 * @param w the card
 * @return Returns the save essid
 */
char* _exalt_wireless_get_save_essid(Exalt_Wireless* w)
{
    EXALT_ASSERT_RETURN(w!=NULL);
    return w->_save_essid;
}

/**
 * @brief free a scan result of iwlib
 * @param w the first result of the list result
 */
void _exalt_wireless_scan_free(wireless_scan **ws)
{
    wireless_scan* result,*r_save;
    EXALT_ASSERT_RETURN_VOID(ws!=NULL);

    result = *ws;

    while(result)
    {
        r_save = result;
        result = result->next;
        free(r_save);
    }
    *ws=NULL;
}

/**
 * @brief scan networks
 * @param eth the card
 */
int _exalt_wireless_scan(void *data)
{
    //see iw/ilwlib.c::iw_scan() for an example & comments
    int fd;
    wireless_scan_head *context;
    int delay; //in ms
    Ecore_List* l;
    short find;
    Exalt_Wireless_Info *wi_l;
    void *data_l;
    Exalt_Wireless_Info* wi= NULL;
    Exalt_Wireless* w;
    char* cpy;

    EXALT_ASSERT_RETURN(data!=NULL);
    Exalt_Ethernet* eth = Exalt_Ethernet(data);
    w = exalt_eth_get_wireless(eth);
    EXALT_ASSERT_RETURN(w!=NULL);

    context=w->context;
    EXALT_ASSERT_RETURN(context!=NULL);

    fd = w->scan_fd;
    EXALT_ASSERT_RETURN(fd>=0);
    cpy = strdup(exalt_eth_get_name(eth));
    delay = iw_process_scan(fd, cpy, exalt_eth_interfaces.we_version, context);
    EXALT_FREE(cpy);
    if(delay<=0)
    {
        l = ecore_list_new();
        l->free_func =  ECORE_FREE_CB(exalt_wirelessinfo_free);

        ecore_list_init(l);


        wireless_scan * result = context->result;

        while(result)
        {
            //retrieve the essid
            if(result->b.has_essid
                    && result->b.essid_on
                    && strcmp(result->b.essid,"<hidden>")!=0)
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
                    wi_l = Exalt_Wireless_Info(data_l);
                    if(strcmp(exalt_wirelessinfo_get_essid(wi_l),exalt_wirelessinfo_get_essid(wi))==0)
                        find = 1;
                    else
                        data_l = ecore_list_next(l);
                }
                if(!find)
                {
                    ecore_list_append(l,wi);
                    iwqual  qual = result->stats.qual;
                    exalt_wirelessinfo_set_noiselvl(wi,qual.noise);
                    exalt_wirelessinfo_set_signalvl(wi,qual.level);
                    exalt_wirelessinfo_set_quality(wi,qual.qual);
                }
                else
                    exalt_wirelessinfo_free(wi);
            }

            result = result->next;
        }

        //send a broadcast
        if(exalt_eth_interfaces.wireless_scan_cb)
            exalt_eth_interfaces.wireless_scan_cb(eth, l, exalt_eth_interfaces.wireless_scan_cb_user_data);

        ecore_list_clear(w->networks);
        w->networks = l;
        context->retry=0;
        w->scanning=0;
        return 0;
    }

    return delay;
}


