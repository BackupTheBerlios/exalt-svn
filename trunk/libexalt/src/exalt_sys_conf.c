/** @file exalt_sys_conf.c */
#include "exalt_sys_conf.h"
#include "config.h"
#include "libexalt_private.h"


exalt_ethernet* _exalt_conf_eet_eth_load(const char* interface);
int _exalt_conf_eet_eth_save(exalt_ethernet* eth);
exalt_wireless_info* _exalt_conf_eet_wirelessinfo_load(const char* essid);
int _exalt_conf_eet_wirelessinfo_save(exalt_wireless_info* wi);



/**
 * @addgroup Exalt_System_Configuration
 *@{
 */



/**
 * @brief save the wpa_supplicant configuration for a wireless interface
 * @param w the wireless card
 * @return Return 1 if success, else -1
 */
int exalt_conf_save_wpasupplicant(exalt_wireless *w)
{
    FILE *fw;
    exalt_ethernet *eth;
    int enc_mode;

    if(!w)
    {
        print_error("ERROR", __FILE__, __LINE__,__func__,"w=%p",w);
        return -1;
    }

    if(!exalt_is_admin())
    {
        print_error("ERROR", __FILE__, __LINE__,__func__,"you need to be root if you want save the configuration file");
        return -1;
    }

    //its more easy to recreate a new file
    //so we don't modify the old file, just delete it :)
    remove(EXALT_WPA_CONF_FILE);

    eth = exalt_wireless_get_eth(w);
    enc_mode = exalt_wireless_get_new_passwd_mode(w);

    //recreate the file
    if(ecore_file_mkpath(EXALT_WPA_CONF_FILE_DIR) == 0)
    {
        print_error("ERROR", __FILE__, __LINE__,__func__,"error can't create the WPA conf file");
        return -1;
    }


    //save the new configuration
    fw = fopen(EXALT_WPA_CONF_FILE,"w");

    fprintf(fw,EXALT_WPA_CONF_HEADER);

    //add the new essid
    fprintf(fw,"network={\n");
    fprintf(fw,"\tssid=\"%s\"\n",exalt_wireless_get_new_essid(w));

    if(enc_mode == EXALT_WIRELESS_ENCRYPTION_WPA_PSK_CCMP_ASCII)
    {
        fprintf(fw,"\tscan_ssid=1\n");
        fprintf(fw,"\tproto=WPA\n");
        fprintf(fw,"\tkey_mgmt=WPA-PSK\n");
        fprintf(fw,"\tpairwise=CCMP\n");
        fprintf(fw,"\tgroup=CCMP\n");
        fprintf(fw,"\tpsk=\"%s\"\n",exalt_wireless_get_new_passwd(w));
    }
    else if(enc_mode==EXALT_WIRELESS_ENCRYPTION_WPA_PSK_TKIP_ASCII)
    {
        fprintf(fw,"\tscan_ssid=1\n");
        fprintf(fw,"\tproto=WPA\n");
        fprintf(fw,"\tkey_mgmt=WPA-PSK\n");
        fprintf(fw,"\tpairwise=TKIP\n");
        fprintf(fw,"\tgroup=TKIP\n");
        fprintf(fw,"\tpsk=\"%s\"\n",exalt_wireless_get_new_passwd(w));
    }
    else if(enc_mode==EXALT_WIRELESS_ENCRYPTION_WPA2_PSK_CCMP_ASCII)
    {
        fprintf(fw,"\tscan_ssid=1\n");
        fprintf(fw,"\tproto=WPA2\n");
        fprintf(fw,"\tkey_mgmt=WPA-PSK\n");
        fprintf(fw,"\tpairwise=CCMP\n");
        fprintf(fw,"\tgroup=CCMP\n");
        fprintf(fw,"\tpsk=\"%s\"\n",exalt_wireless_get_new_passwd(w));
    }
    else if(enc_mode==EXALT_WIRELESS_ENCRYPTION_WPA2_PSK_TKIP_ASCII)
    {
        fprintf(fw,"\tscan_ssid=1\n");
        fprintf(fw,"\tproto=WPA2\n");
        fprintf(fw,"\tkey_mgmt=WPA-PSK\n");
        fprintf(fw,"\tpairwise=TKIP\n");
        fprintf(fw,"\tgroup=TKIP\n");
        fprintf(fw,"\tpsk=\"%s\"\n",exalt_wireless_get_new_passwd(w));
    }

    fprintf(fw,"}\n");

    fclose(fw);

    return 1;
}

/**
 * @brief save the configuration for one interface
 * @param eth the interface
 * @return Return 1 if success, else -1
 */
int exalt_conf_wirelessinfo_save(exalt_wireless* w)
{
    exalt_wireless_info* wi = exalt_wireless_get_networkinfo_by_essid(w, exalt_wireless_get_essid(w));
    exalt_ethernet* eth = exalt_wireless_get_eth(w);

    exalt_wirelessinfo_set_default_passwd_mode(wi,exalt_wireless_get_new_passwd_mode(w));
    exalt_wirelessinfo_set_default_passwd(wi,exalt_wireless_get_new_passwd(w));
    exalt_wirelessinfo_set_default_mode(wi,exalt_wireless_get_new_mode(w));
    exalt_wirelessinfo_set_default_security_mode(wi,exalt_wireless_get_new_security_mode(w));

    exalt_wirelessinfo_set_default_ip(wi,exalt_eth_get_new_ip(eth));
    exalt_wirelessinfo_set_default_netmask(wi,exalt_eth_get_new_netmask(eth));
    exalt_wirelessinfo_set_default_gateway(wi,exalt_eth_get_new_gateway(eth));
    exalt_wirelessinfo_set_default_dhcp(wi,exalt_eth_is_new_dhcp(eth));

    return _exalt_conf_eet_wirelessinfo_save(wi);
}

/**
 * @brief load the configuration for a wireless network
 * @param wi the wireless network
 * @return Return 1 if success, else -1
 */
exalt_wireless_info* exalt_conf_wirelessinfo_load(const char *essid)
{
    return _exalt_conf_eet_wirelessinfo_load(essid);
}


/**
 * @brief save the configuration of a card
 * @param eth the card
 * @return Return 1 if success, else 0
 */
int exalt_conf_save(exalt_ethernet* eth)
{
    exalt_eth_set_new_up(eth,exalt_eth_is_up(eth));

    return _exalt_conf_eet_eth_save(eth);
}


exalt_ethernet* exalt_conf_load(const char* name)
{
    return _exalt_conf_eet_eth_load(name);
}


/** @} */



/* PRIVATES FUNCTIONS */

exalt_ethernet* _exalt_conf_eet_eth_load(const char* interface)
{
    exalt_ethernet *data = NULL;
    Eet_Data_Descriptor *edd_w, *edd_eth;
    Eet_File *f;

    edd_w = exalt_wireless_edd_new();
    edd_eth = exalt_eth_edd_new(edd_w);

    f = eet_open(EXALT_CONF_FILE_EET, EET_FILE_MODE_READ);
    if(!f)
        f = eet_open(EXALT_CONF_FILE_EET, EET_FILE_MODE_WRITE);

    data = eet_data_read(f, edd_eth, interface);

    eet_close(f);
    eet_data_descriptor_free(edd_w);
    eet_data_descriptor_free(edd_eth);
    return data;
}

int _exalt_conf_eet_eth_save(exalt_ethernet* eth)
{
    Eet_Data_Descriptor *edd_w,* edd_eth;
    Eet_File* f;
    int res;

    edd_w = exalt_wireless_edd_new();
    edd_eth = exalt_eth_edd_new(edd_w);
    f = eet_open(EXALT_CONF_FILE_EET, EET_FILE_MODE_READ_WRITE);
    if(!f)
        f = eet_open(EXALT_CONF_FILE_EET, EET_FILE_MODE_WRITE);
    res=eet_data_write(f, edd_eth,exalt_eth_get_name(eth), eth, 0);
    if(!res)
        print_error("ERROR", __FILE__, __LINE__,__func__,"Can not save the configuration file (%s) f=%p, edd_w=%p,  edd_eth=%p", EXALT_CONF_FILE_EET,f, edd_w, edd_eth);

    eet_close(f);
    eet_data_descriptor_free(edd_w);
    eet_data_descriptor_free(edd_eth);
    return res;
}

exalt_wireless_info* _exalt_conf_eet_wirelessinfo_load(const char* essid)
{
    exalt_wireless_info *data = NULL;
    Eet_Data_Descriptor *edd_wi;
    Eet_File *f;

    edd_wi = exalt_wirelessinfo_edd_new();

    f = eet_open(EXALT_CONF_FILE_EET, EET_FILE_MODE_READ);
    if(!f)
        f = eet_open(EXALT_CONF_FILE_EET, EET_FILE_MODE_WRITE);

    data = eet_data_read(f, edd_wi, essid);

    eet_close(f);
    eet_data_descriptor_free(edd_wi);
    return data;
}

int _exalt_conf_eet_wirelessinfo_save(exalt_wireless_info* wi)
{
    Eet_Data_Descriptor* edd_wi;
    Eet_File* f;
    int res;

    edd_wi = exalt_wirelessinfo_edd_new();
    f = eet_open(EXALT_CONF_FILE_EET, EET_FILE_MODE_READ_WRITE);
    if(!f)
        f = eet_open(EXALT_CONF_FILE_EET, EET_FILE_MODE_WRITE);
    res=eet_data_write(f, edd_wi,exalt_wirelessinfo_get_essid(wi), wi, 0);
    if(!res)
        print_error("ERROR", __FILE__, __LINE__,__func__,"Can not save the configuration file (%s) f=%p, edd_wi=%p", EXALT_CONF_FILE_EET,f, edd_wi);

    eet_close(f);
    eet_data_descriptor_free(edd_wi);
    return res;
}

