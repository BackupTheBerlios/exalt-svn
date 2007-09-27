/** @file exalt_wireless_info.c */
#include "exalt_wireless_info.h"
#include "libexalt_private.h"

/**
 * @addtogroup Exalt_Wireless_Info
 * @{
 */

struct Exalt_Wireless_Info
{
    Exalt_Wireless* w;

    char* address;
    char* essid;
    int encryption;
    int quality;
    int signal_lvl;
    int noise_lvl;
    char* mode;

    //no more use
    char* protocol;
    char* channel;
    char* bit_rates;
    //


    short scan_ok; //0 if the network is not in the scan result
    short known;   //1 if the network is known, in the config file.

    //default configuration
    int default_passwd_mode;
    char* default_passwd;
    short default_dhcp;
    char* default_ip;
    char* default_gateway;
    char* default_netmask;
    int default_security_mode;
    int default_mode;
};


/*
 * Set/get functions for Exalt_Wireless_Info
 */


/**
 * @brief init the informations about the wireless network
 * @param w the Exalt_Wireless_Info
 */
Exalt_Wireless_Info* exalt_wirelessinfo_create(Exalt_Wireless* w)
{
	Exalt_Wireless_Info* wi = (Exalt_Wireless_Info*)malloc((unsigned int)sizeof(Exalt_Wireless_Info));

	if(!wi)
        {
            print_error("ERROR", __FILE__, __LINE__,__func__, "wi=%p",wi);
            return NULL;
        }

 	wi->w = w;
	wi->address = NULL;
	wi->essid= NULL;
	wi->encryption = 0;

	wi->protocol = NULL;
	wi->mode = NULL;
	wi->channel = NULL;
 	wi->bit_rates= NULL;
	wi->quality = 0;
	wi->signal_lvl = 0;
	wi->noise_lvl = 0;
	wi->scan_ok = 0;
	wi->known = 0;

 	wi->default_passwd_mode = 0;
	wi->default_passwd = NULL;
	wi->default_dhcp = 1;
	wi->default_ip = NULL;
	wi->default_gateway = NULL;
	wi->default_netmask = NULL;

	return wi;
}



/**
 * @brief free Exalt_Wireless_Info
 * @param data the Exalt_Wireless_Info
 */
void exalt_wirelessinfo_free(void* data)
{
 	Exalt_Wireless_Info* wi = Exalt_Wireless_Info(data);
 	EXALT_FREE(wi->address)
	EXALT_FREE(wi->essid)
	EXALT_FREE(wi->protocol)
	EXALT_FREE(wi->mode)
	EXALT_FREE(wi->channel)
	EXALT_FREE(wi->bit_rates)

	EXALT_FREE(wi->default_passwd)
	EXALT_FREE(wi->default_ip)
	EXALT_FREE(wi->default_gateway)
	EXALT_FREE(wi->default_netmask)
}



/**
 * @brief set the mac address
 * @param w the Exalt_Wireless_Info
 * @param address the new mac address
 */
void exalt_wirelessinfo_set_address(Exalt_Wireless_Info* w, const char* address)
{
	if(w && address)
	{
		EXALT_FREE(w->address)
			w->address = strdup(address);
	}
}




/**
 * @brief set the essid
 * @param w the Exalt_Wireless_Info
 * @param essid the new essid
 */
void exalt_wirelessinfo_set_essid(Exalt_Wireless_Info* w, const char* essid)
{
    if(!w || !essid)
    {
        print_error("ERROR", __FILE__, __LINE__,__func__, "w=%p essid=%p",w,essid);
        return ;
    }
    EXALT_FREE(w->essid)
        w->essid = strdup(essid);
}



/**
 * @brief set the mode
 * @param w the Exalt_Wireless_Info
 * @param mode the new mode
 */
void exalt_wirelessinfo_set_mode(Exalt_Wireless_Info* w, const char* mode)
{
    if(!w || !mode)
    {
        print_error("ERROR", __FILE__, __LINE__,__func__, "w=%p mode=%p",w,mode);
        return ;
    }
    EXALT_FREE(w->mode)
        w->mode = strdup(mode);
}



/**
 * @brief set the protocol
 * @param w the Exalt_Wireless_Info
 * @param protocol the new protocol
 */
void exalt_wirelessinfo_set_protocol(Exalt_Wireless_Info* w, const char* protocol)
{
    if(!w || !protocol)
    {
        print_error("ERROR", __FILE__, __LINE__,__func__, "w=%p protocol=%p",w,protocol);
        return ;
    }
    EXALT_FREE(w->protocol)
        w->protocol = strdup(protocol);
}



/**
 * @brief set the channel
 * @param w the Exalt_Wireless_Info
 * @param channel the new channel
 */
void exalt_wirelessinfo_set_channel(Exalt_Wireless_Info* w, const char* channel)
{
    if(!w || !channel)
    {
        print_error("ERROR", __FILE__, __LINE__,__func__, "w=%p channel=%p",w,channel);
        return ;
    }
    EXALT_FREE(w->channel)
        w->channel = strdup(channel);
}



/**
 * @brief set if ecryption state
 * @param w the Exalt_Wireless_Info
 * @param encryption the new encryption
 */
void exalt_wirelessinfo_set_encryption(Exalt_Wireless_Info* w, int  encryption)
{
    if(!w)
    {
        print_error("ERROR", __FILE__, __LINE__,__func__, "w=%p",w);
        return ;
    }
    w->encryption = encryption;
}



/**
 * @brief set the bit rates
 * @param w the Exalt_Wireless_Info
 * @param bit_rates the new bit rates
 */
void exalt_wirelessinfo_set_bitrates(Exalt_Wireless_Info* w, const char* bit_rates)
{
    if(!w || !bit_rates)
    {
        print_error("ERROR", __FILE__, __LINE__,__func__, "w=%p bit_rates=%p",w, bit_rates);
        return ;

    }
    EXALT_FREE(w->bit_rates)
        w->bit_rates = strdup(bit_rates);
}



/**
 * @brief set the quality
 * @param w the Exalt_Wireless_Info
 * @param quality the new quality
 */
void exalt_wirelessinfo_set_quality(Exalt_Wireless_Info* w, int quality)
{
	if(!w)
    {
        print_error("ERROR", __FILE__, __LINE__,__func__, "w=%p",w);
        return ;
    }
		w->quality = quality;
}



/**
 * @brief set the signal level
 * @param w the Exalt_Wireless_Info
 * @param signal_lvl the new signal level
 */
void exalt_wirelessinfo_set_signalvl(Exalt_Wireless_Info* w, int signal_lvl)
{
	if(!w)
    {
        print_error("ERROR", __FILE__, __LINE__,__func__, "w=%p",w);
        return ;
    }
		w->signal_lvl = signal_lvl;
}



/**
 * @brief set the noise level
 * @param w the Exalt_Wireless_Info
 * @param noise_lvl the new noise level
 */
void exalt_wirelessinfo_set_noiselvl(Exalt_Wireless_Info* w, int noise_lvl)
{
	if(!w)
    {
        print_error("ERROR", __FILE__, __LINE__,__func__, "w=%p",w);
        return ;
    }
		w->noise_lvl = noise_lvl;
}



/**
 * @brief set if the network is in the scan result (1 or 0)
 * @param w the Exalt_Wireless_Info
 * @param ok 1 if the network is in the scan result, else 0
 */

void exalt_wirelessinfo_set_scanok(Exalt_Wireless_Info *w,short ok)
{
	if(!w)
    {
        print_error("ERROR", __FILE__, __LINE__,__func__, "w=%p",w);
        return ;
    }
		w->scan_ok = ok;
}



/**
 * @brief set 1 if the network is know, if he is a in the configuration file
 * @param w the Exalt_Wireless_Info
 * @param known 1 if the network is know, else 0
 */

void exalt_wirelessinfo_set_known(Exalt_Wireless_Info* w,short known)
{
	if(!w)
    {
        print_error("ERROR", __FILE__, __LINE__,__func__, "w=%p",w);
        return ;
    }
		w->known = known;
}



/**
 * @brief get the mac address
 * @param w the Exalt_Wireless_Info
 * @return Return the mac address
 */
const char* exalt_wirelessinfo_get_addr(Exalt_Wireless_Info* w)
{
    if(!w)
    {
        print_error("ERROR", __FILE__, __LINE__,__func__, "w=%p",w);
        return NULL;
    }
		return w->address;
}

/**
 * @brief get the wireless struct
 * @param w the Exalt_Wireless_Info
 * @return Return the wireless struct of the card which provide the scan
 */
Exalt_Wireless* exalt_wirelessinfo_get_wireless(Exalt_Wireless_Info* w)
{
    if(!w)
    {
        print_error("ERROR", __FILE__, __LINE__,__func__, "w=%p",w);
        return NULL;
    }
		return w->w;
}

/**
 * @brief get the essid
 * @param w the Exalt_Wireless_Info
 * @return Return the essid
 */
const char* exalt_wirelessinfo_get_essid(Exalt_Wireless_Info* w)
{
	if(!w)
    {
        print_error("ERROR", __FILE__, __LINE__,__func__, "w=%p",w);
        return NULL;
    }
		return w->essid;
}



/**
 * @brief get the protocol
 * @param w the Exalt_Wireless_Info
 * @return Return the protocol
 */
const char* exalt_wirelessinfo_get_protocol(Exalt_Wireless_Info* w)
{
		if(!w)
    {
        print_error("ERROR", __FILE__, __LINE__,__func__, "w=%p",w);
        return NULL;
    }
		return w->protocol;
}



/**
 * @brief get the mode
 * @param w the Exalt_Wireless_Info
 * @return Return the mode
 */
const char* exalt_wirelessinfo_get_mode(Exalt_Wireless_Info* w)
{
    if(!w)
    {
        print_error("ERROR", __FILE__, __LINE__,__func__, "w=%p",w);
        return NULL;
    }
    return w->mode;
}



/**
 * @brief get the channel
 * @param w the Exalt_Wireless_Info
 * @return Return channel
 */
const char* exalt_wirelessinfo_get_channel(Exalt_Wireless_Info* w)
{
	if(!w)
    {
        print_error("ERROR", __FILE__, __LINE__,__func__, "w=%p",w);
        return NULL;
    }
		return w->channel;
}



/**
 * @brief get the encryption state
 * @param w the Exalt_Wireless_Info
 * @return Return the encryption state (1 or 0)
 */
int exalt_wirelessinfo_get_encryption(Exalt_Wireless_Info* w)
{
    if(!w)
    {
        print_error("ERROR", __FILE__, __LINE__,__func__, "w=%p",w);
        return -1;
    }
    return w->encryption;
}



/**
 * @brief get the bit rates
 * @param w the Exalt_Wireless_Info
 * @return Return the bit rates
 */
const char* exalt_wirelessinfo_get_bitrates(Exalt_Wireless_Info* w)
{
    if(!w)
    {
        print_error("ERROR", __FILE__, __LINE__,__func__, "w=%p",w);
        return NULL;
    }
    return w->bit_rates;
}



/**
 * @brief get the quality
 * @param w the Exalt_Wireless_Info
 * @return Return the quality
 */
int exalt_wirelessinfo_get_quality(Exalt_Wireless_Info* w)
{
	if(!w)
    {
        print_error("ERROR", __FILE__, __LINE__,__func__, "w=%p",w);
        return -1;
    }
		return w->quality;
}



/**
 * @brief get the signal level
 * @param w the Exalt_Wireless_Info
 * @return Return signal level
 */
int exalt_wirelessinfo_get_signallvl(Exalt_Wireless_Info* w)
{
	if(!w)
    {
        print_error("ERROR", __FILE__, __LINE__,__func__, "w=%p",w);
        return -1;
    }
		return w->signal_lvl;
}



/**
 * @brief get the noise level
 * @param w the Exalt_Wireless_Info
 * @return Return the noise level
 */
int exalt_wirelessinfo_get_noiselvl(Exalt_Wireless_Info* w)
{
	if(!w)
        {
            print_error("ERROR", __FILE__, __LINE__,__func__, "w=%p",w);
            return -1;
        }
        return w->noise_lvl;
}



/**
 * @brief check if a wireless network is known
 * @param wi the network
 * @return Return 1 if the network is known, else 0
 */
int exalt_wirelessinfo_is_known(Exalt_Wireless_Info* wi)
{
 	if(!wi)
	{
	 	print_error("ERROR", __FILE__, __LINE__,__func__,"wi=%p",wi);
		return -1;
	}

	return wi->known;
}
//}}}


/**
 * @brief check if a wireless network is known
 * @param wi the network
 * @return Return 1 if the network is known, else 0
 */
int exalt_wirelessinfo_is_scan(Exalt_Wireless_Info* wi)
{
 	if(!wi)
	{
	 	print_error("ERROR", __FILE__, __LINE__,__func__,"wi=",wi);
		return -1;
	}

	return wi->scan_ok;
}
//}}}




/**
 * @brief get password
 * @param wi the Exalt_Wireless_Info
 * @return Return the password
 */
const char* exalt_wirelessinfo_get_default_passwd(Exalt_Wireless_Info* wi)
{
	if(!wi)
	{
	 	print_error("ERROR", __FILE__, __LINE__,__func__,"wi=",wi);
		return NULL;
	}
	return wi->default_passwd;
}



/**
 * @brief set the password
 * @param wi the Exalt_Wireless_Info
 * @param passwd the new password
 */
void exalt_wirelessinfo_set_default_passwd(Exalt_Wireless_Info* wi,const char* passwd)
{
	if(!wi || !passwd)
	{
	 	print_error("ERROR", __FILE__, __LINE__,__func__,"wi=%p  passwd=%p",wi,passwd);
		return ;
	}

	if(!exalt_is_passwd(passwd,exalt_wirelessinfo_get_default_passwd_mode(wi)))
	{
		print_error("WARNING", __FILE__, __LINE__,__func__,"passwd(%s) is not a correct password",passwd);
		return ;
	}
	EXALT_FREE(wi->default_passwd)
		wi->default_passwd=strdup(passwd);
}



/**
 * @brief get the password mode {WEP hexa, WEP plain text, none, WPA ...}
 * @param wi the Exalt_Wireless_Info
 * @return Return the password mode, -1 if a error occurs
 */
int exalt_wirelessinfo_get_default_passwd_mode(Exalt_Wireless_Info* wi)
{
 	if(!wi)
	{
	 	print_error("ERROR", __FILE__, __LINE__,__func__,"wi=%p",wi);
		return -1;
	}
	return wi->default_passwd_mode;
}



/**
 * @brief set the password mode {WEP hexa, WEP plain text, none, WPA ...}
 * @param wi the Exalt_Wireless_Info
 * @param passwd_mode the new password mode
 */
void exalt_wirelessinfo_set_default_passwd_mode(Exalt_Wireless_Info* wi,int passwd_mode)
{
	if(!wi )
	{
		print_error("ERROR", __FILE__, __LINE__,__func__,"wi=%p",wi);
		return ;
	}
	wi->default_passwd_mode = passwd_mode;
}


/**
 * @brief get the security mode
 * @param wi the Exalt_Wireless_Info
 * @return Return the security mode, -1 if a error occurs
 */
int exalt_wirelessinfo_get_default_security_mode(Exalt_Wireless_Info* wi)
{
 	if(!wi)
	{
	 	print_error("ERROR", __FILE__, __LINE__,__func__,"wi=%p",wi);
		return -1;
	}
	return wi->default_security_mode;
}



/**
 * @brief set the security mode
 * @param wi the Exalt_Wireless_Info
 * @param security_mode the new security mode
 */
void exalt_wirelessinfo_set_default_security_mode(Exalt_Wireless_Info* wi,int security_mode)
{
	if(!wi )
	{
		print_error("ERROR", __FILE__, __LINE__,__func__,"wi=%p",wi);
		return ;
	}
	wi->default_security_mode = security_mode;
}

/**
 * @brief get the  mode
 * @param wi the Exalt_Wireless_Info
 * @return Return the  mode, -1 if a error occurs
 */
int exalt_wirelessinfo_get_default_mode(Exalt_Wireless_Info* wi)
{
 	if(!wi)
	{
	 	print_error("ERROR", __FILE__, __LINE__,__func__,"wi=%p",wi);
		return -1;
	}
	return wi->default_mode;
}



/**
 * @brief set the  mode
 * @param wi the Exalt_Wireless_Info
 * @param mode the new  mode
 */
void exalt_wirelessinfo_set_default_mode(Exalt_Wireless_Info* wi,int mode)
{
	if(!wi )
	{
		print_error("ERROR", __FILE__, __LINE__,__func__,"wi=%p",wi);
		return ;
	}
	wi->default_mode = mode;
}



/**
 * @brief get the default ip address
 * @param wi the Exalt_Wireless_Info
 * @return Returns the ip address
 */
const char* exalt_wirelessinfo_get_default_ip(Exalt_Wireless_Info* wi)
{
	if(!wi)
	{
	 	print_error("ERROR", __FILE__, __LINE__,__func__,"wi=%p",wi);
		return NULL;
	}
	return wi->default_ip;
}



/**
 * @brief get the default netmask
 * @param wi the Exalt_Wireless_Info
 * @return Returns the netmask
 */
const char* exalt_wirelessinfo_get_default_netmask(Exalt_Wireless_Info* wi)
{
	if(!wi)
	{
	 	print_error("ERROR", __FILE__, __LINE__,__func__,"wi=%p",wi);
		return NULL;
	}
	return wi->default_netmask;
}



/**
 * @brief get the default gateway
 * @param wi the Exalt_Wireless_Info
 * @return Returns the gateway
 */
const char* exalt_wirelessinfo_get_default_gateway(Exalt_Wireless_Info* wi)
{
	if(!wi)
	{
	 	print_error("ERROR", __FILE__, __LINE__,__func__,"wi=%p",wi);
		return NULL;
	}
	return wi->default_gateway;
}



/**
 * @brief set the default ip address
 * @param wi the Exalt_Wireless_Info
 * @param ip the new ip address
 * @return Returns 1 if the new ip address is apply, 0 if the "ip" doesn't have a correct format else -1
 */
int exalt_wirelessinfo_set_default_ip(Exalt_Wireless_Info* wi, const char* ip)
{
 	if(!wi || !ip)
	{
	 	print_error("ERROR", __FILE__, __LINE__,__func__,"wi=%p ip=%p",wi,ip);
		return -1;
	}
	if(!exalt_is_address(ip))
	{
	 	print_error("WARNING", __FILE__, __LINE__,__func__,"ip(%s) is not a valid address",ip);
		return 0;
	}

	EXALT_FREE(wi->default_ip)
	wi->default_ip=strdup(ip);
	return 1;
}



/**
 * @brief set the default netmask address
 * @param wi the Exalt_Wireless_Info
 * @param netmask the new netmask address
 * @return Returns 1 if the new netmask address is apply, 0 if the "netmask" doesn't have a correct format else -1
 */
int exalt_wirelessinfo_set_default_netmask(Exalt_Wireless_Info* wi, const char* netmask)
{
 	if(!wi || !netmask)
	{
	 	print_error("ERROR", __FILE__, __LINE__,__func__,"wi=%p netmask=%p !",wi,netmask);
		return -1;
	}
	if(!exalt_is_address(netmask))
	{
	 	print_error("WARNING", __FILE__, __LINE__,__func__,"netmask(%s) is not a valid netmask",netmask);
		return 0;
	}

	EXALT_FREE(wi->default_netmask)
	wi->default_netmask=strdup(netmask);
	return 1;
}



/**
 * @brief set the default gateway address
 * @param wi the Exalt_Wireless_Info
 * @param gateway the new gateway address
 * @return Returns 1 if the new gateway address is apply, 0 if the "gateway" doesn't have a correct format else -1
 */
int exalt_wirelessinfo_set_default_gateway(Exalt_Wireless_Info* wi, const char* gateway)
{
 	if(!wi || !gateway)
	{
	 	print_error("ERROR", __FILE__, __LINE__,__func__,"wi=%p gateway=%p !",wi,gateway);
		return -1;
	}
	if(!exalt_is_address(gateway))
	{
	 	print_error("WARNING", __FILE__, __LINE__,__func__,"gateway(%s) is not a valid address",gateway);
		return 0;
	}

	EXALT_FREE(wi->default_gateway)
	wi->default_gateway=strdup(gateway);
	return 1;
}



/**
 * @brief get if network "wi" use DHCP or static as default configuration
 * @param wi the Exalt_Wireless_Info
 * @return Returns 1 if the network use DHCP, else 0
 */
short exalt_wirelessinfo_is_default_dhcp(Exalt_Wireless_Info* wi)
{
	if(!wi)
	{
	 	print_error("ERROR", __FILE__, __LINE__,__func__,"wi=%p",wi);
		return -1;
	}
	return wi->default_dhcp;
}



/**
 * @brief set the default dhcp mode of the network "wi"
 * @param wi the Exalt_Wireless_Info
 * @param dhcp the mode: 1 -> dhcp, 0 -> static
 * @return returns 1 if the mode is apply, else 0
 */
int exalt_wirelessinfo_set_default_dhcp(Exalt_Wireless_Info* wi, short dhcp)
{
	if(!wi)
	{
	 	print_error("ERROR", __FILE__, __LINE__,__func__,"wi=%p",wi);
		return 0;
	}
	wi->default_dhcp=dhcp;
	return 1;
}


/**
 * @brief create an ett descriptor for saving a struct Exalt_Wireless_Info
 * @return Return the descriptor
 */
Eet_Data_Descriptor * exalt_wirelessinfo_edd_new()
{
    Eet_Data_Descriptor *edd_wi;

    edd_wi = eet_data_descriptor_new("wireless_info", sizeof(Exalt_Wireless_Info),
            evas_list_next,
            evas_list_append,
            evas_list_data,
            evas_list_free,
            evas_hash_foreach,
            evas_hash_add,
            evas_hash_free);

    EET_DATA_DESCRIPTOR_ADD_BASIC(edd_wi, Exalt_Wireless_Info, "essid", essid, EET_T_STRING);
    EET_DATA_DESCRIPTOR_ADD_BASIC(edd_wi, Exalt_Wireless_Info, "key", default_passwd, EET_T_STRING);

    EET_DATA_DESCRIPTOR_ADD_BASIC(edd_wi, Exalt_Wireless_Info, "key_mode", default_passwd_mode, EET_T_INT);
    EET_DATA_DESCRIPTOR_ADD_BASIC(edd_wi, Exalt_Wireless_Info, "mode", default_mode, EET_T_INT);
    EET_DATA_DESCRIPTOR_ADD_BASIC(edd_wi, Exalt_Wireless_Info, "security_mode", default_security_mode, EET_T_INT);

    EET_DATA_DESCRIPTOR_ADD_BASIC(edd_wi, Exalt_Wireless_Info, "ip", default_ip, EET_T_STRING);
    EET_DATA_DESCRIPTOR_ADD_BASIC(edd_wi, Exalt_Wireless_Info, "netmask", default_netmask, EET_T_STRING);
    EET_DATA_DESCRIPTOR_ADD_BASIC(edd_wi, Exalt_Wireless_Info, "gateway", default_gateway, EET_T_STRING);
    EET_DATA_DESCRIPTOR_ADD_BASIC(edd_wi, Exalt_Wireless_Info, "dhcp", default_dhcp, EET_T_SHORT);


    return edd_wi;
}

