/** @file exalt_wireless_info.c */
#include "exalt_wireless_info.h"
#include "libexalt_private.h"

/**
 * @addtogroup Exalt_Wireless_Info
 * @{
 */

/**
 * @brief informations about a wireless network
 * @structinfo
 */
struct exalt_wireless_info
{
    exalt_wireless* w;

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
 * Set/get functions for exalt_wireless_info
 */


/**
 * @brief init the informations about the wireless network
 * @param w the exalt_wireless_info
 */
exalt_wireless_info* exalt_wirelessinfo_create(exalt_wireless* w)
{
	exalt_wireless_info* wi = (exalt_wireless_info*)malloc((unsigned int)sizeof(exalt_wireless_info));

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
 * @brief free exalt_wireless_info
 * @param data the exalt_wireless_info
 */
void exalt_wirelessinfo_free(void* data)
{
 	exalt_wireless_info* wi = EXALT_WIRELESS_INFO(data);
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
 * @param w the exalt_wireless_info
 * @param address the new mac address
 */
void exalt_wirelessinfo_set_address(exalt_wireless_info* w, char* address)
{
	if(w && address)
	{
		EXALT_FREE(w->address)
			w->address = strdup(address);
	}
}




/**
 * @brief set the essid
 * @param w the exalt_wireless_info
 * @param essid the new essid
 */
void exalt_wirelessinfo_set_essid(exalt_wireless_info* w, char* essid)
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
 * @param w the exalt_wireless_info
 * @param mode the new mode
 */
void exalt_wirelessinfo_set_mode(exalt_wireless_info* w, char* mode)
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
 * @param w the exalt_wireless_info
 * @param protocol the new protocol
 */
void exalt_wirelessinfo_set_protocol(exalt_wireless_info* w, char* protocol)
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
 * @param w the exalt_wireless_info
 * @param channel the new channel
 */
void exalt_wirelessinfo_set_channel(exalt_wireless_info* w, char* channel)
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
 * @param w the exalt_wireless_info
 * @param encryption the new encryption
 */
void exalt_wirelessinfo_set_encryption(exalt_wireless_info* w, int  encryption)
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
 * @param w the exalt_wireless_info
 * @param bit_rates the new bit rates
 */
void exalt_wirelessinfo_set_bitrates(exalt_wireless_info* w, char* bit_rates)
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
 * @param w the exalt_wireless_info
 * @param quality the new quality
 */
void exalt_wirelessinfo_set_quality(exalt_wireless_info* w, int quality)
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
 * @param w the exalt_wireless_info
 * @param signal_lvl the new signal level
 */
void exalt_wirelessinfo_set_signalvl(exalt_wireless_info* w, int signal_lvl)
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
 * @param w the exalt_wireless_info
 * @param noise_lvl the new noise level
 */
void exalt_wirelessinfo_set_noiselvl(exalt_wireless_info* w, int noise_lvl)
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
 * @param w the exalt_wireless_info
 * @param ok 1 if the network is in the scan result, else 0
 */

void exalt_wirelessinfo_set_scanok(exalt_wireless_info *w,short ok)
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
 * @param w the exalt_wireless_info
 * @param known 1 if the network is know, else 0
 */

void exalt_wirelessinfo_set_known(exalt_wireless_info* w,short known)
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
 * @param w the exalt_wireless_info
 * @return Return the mac address
 */
char* exalt_wirelessinfo_get_addr(exalt_wireless_info* w)
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
 * @param w the exalt_wireless_info
 * @return Return the wireless struct of the card which provide the scan
 */
exalt_wireless* exalt_wirelessinfo_get_wireless(exalt_wireless_info* w)
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
 * @param w the exalt_wireless_info
 * @return Return the essid
 */
char* exalt_wirelessinfo_get_essid(exalt_wireless_info* w)
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
 * @param w the exalt_wireless_info
 * @return Return the protocol
 */
char* exalt_wirelessinfo_get_protocol(exalt_wireless_info* w)
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
 * @param w the exalt_wireless_info
 * @return Return the mode
 */
char* exalt_wirelessinfo_get_mode(exalt_wireless_info* w)
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
 * @param w the exalt_wireless_info
 * @return Return channel
 */
char* exalt_wirelessinfo_get_channel(exalt_wireless_info* w)
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
 * @param w the exalt_wireless_info
 * @return Return the encryption state (1 or 0)
 */
int exalt_wirelessinfo_get_encryption(exalt_wireless_info* w)
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
 * @param w the exalt_wireless_info
 * @return Return the bit rates
 */
char* exalt_wirelessinfo_get_bitrates(exalt_wireless_info* w)
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
 * @param w the exalt_wireless_info
 * @return Return the quality
 */
int exalt_wirelessinfo_get_quality(exalt_wireless_info* w)
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
 * @param w the exalt_wireless_info
 * @return Return signal level
 */
int exalt_wirelessinfo_get_signallvl(exalt_wireless_info* w)
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
 * @param w the exalt_wireless_info
 * @return Return the noise level
 */
int exalt_wirelessinfo_get_noiselvl(exalt_wireless_info* w)
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
int exalt_wirelessinfo_is_known(exalt_wireless_info* wi)
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
int exalt_wirelessinfo_is_scan(exalt_wireless_info* wi)
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
 * @param wi the exalt_wireless_info
 * @return Return the password
 */
char* exalt_wirelessinfo_get_default_passwd(exalt_wireless_info* wi)
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
 * @param wi the exalt_wireless_info
 * @param passwd the new password
 */
void exalt_wirelessinfo_set_default_passwd(exalt_wireless_info* wi,const char* passwd)
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
 * @param wi the exalt_wireless_info
 * @return Return the password mode, -1 if a error occurs
 */
int exalt_wirelessinfo_get_default_passwd_mode(exalt_wireless_info* wi)
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
 * @param wi the exalt_wireless_info
 * @param passwd_mode the new password mode
 */
void exalt_wirelessinfo_set_default_passwd_mode(exalt_wireless_info* wi,int passwd_mode)
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
 * @param wi the exalt_wireless_info
 * @return Return the security mode, -1 if a error occurs
 */
int exalt_wirelessinfo_get_default_security_mode(exalt_wireless_info* wi)
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
 * @param wi the exalt_wireless_info
 * @param security_mode the new security mode
 */
void exalt_wirelessinfo_set_default_security_mode(exalt_wireless_info* wi,int security_mode)
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
 * @param wi the exalt_wireless_info
 * @return Return the  mode, -1 if a error occurs
 */
int exalt_wirelessinfo_get_default_mode(exalt_wireless_info* wi)
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
 * @param wi the exalt_wireless_info
 * @param mode the new  mode
 */
void exalt_wirelessinfo_set_default_mode(exalt_wireless_info* wi,int mode)
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
 * @param wi the exalt_wireless_info
 * @return Returns the ip address
 */
char* exalt_wirelessinfo_get_default_ip(exalt_wireless_info* wi)
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
 * @param wi the exalt_wireless_info
 * @return Returns the netmask
 */
char* exalt_wirelessinfo_get_default_netmask(exalt_wireless_info* wi)
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
 * @param wi the exalt_wireless_info
 * @return Returns the gateway
 */
char* exalt_wirelessinfo_get_default_gateway(exalt_wireless_info* wi)
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
 * @param wi the exalt_wireless_info
 * @param ip the new ip address
 * @return Returns 1 if the new ip address is apply, 0 if the "ip" doesn't have a correct format else -1
 */
int exalt_wirelessinfo_set_default_ip(exalt_wireless_info* wi, const char* ip)
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
 * @param wi the exalt_wireless_info
 * @param netmask the new netmask address
 * @return Returns 1 if the new netmask address is apply, 0 if the "netmask" doesn't have a correct format else -1
 */
int exalt_wirelessinfo_set_default_netmask(exalt_wireless_info* wi, const char* netmask)
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
 * @param wi the exalt_wireless_info
 * @param gateway the new gateway address
 * @return Returns 1 if the new gateway address is apply, 0 if the "gateway" doesn't have a correct format else -1
 */
int exalt_wirelessinfo_set_default_gateway(exalt_wireless_info* wi, const char* gateway)
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
 * @param wi the exalt_wireless_info
 * @return Returns 1 if the network use DHCP, else 0
 */
short exalt_wirelessinfo_is_default_dhcp(exalt_wireless_info* wi)
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
 * @param wi the exalt_wireless_info
 * @param dhcp the mode: 1 -> dhcp, 0 -> static
 * @return returns 1 if the mode is apply, else 0
 */
int exalt_wirelessinfo_set_default_dhcp(exalt_wireless_info* wi, short dhcp)
{
	if(!wi)
	{
	 	print_error("ERROR", __FILE__, __LINE__,__func__,"wi=%p",wi);
		return 0;
	}
	wi->default_dhcp=dhcp;
	return 1;
}


