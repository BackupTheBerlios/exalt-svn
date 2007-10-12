/** @file libexalt.c */
#include "libexalt.h"
#include "libexalt_private.h"
#include "config.h"

/**
 * @addtogroup Exalt_General
 * @{
 */

Exalt_Ethernets exalt_eth_interfaces;


/**
 * @brief test if a string is a correct adress
 * @param ip the string ip
 * return Return 1 if sucess, else 0
 */
short exalt_is_address(const char* ip)
{
    Exalt_Regex *r;
    short res;

    if(!ip) return 0;

    r = exalt_regex_create(ip,REGEXP_IS_IP,0);
    res = exalt_regex_execute(r);
    exalt_regex_free(&r);
    return res;
}

/**
 * @brief test if a string is a correct essid
 * @param essid the essid
 * @return Return 1 if success, else 0
 */
short exalt_is_essid(const char* essid)
{
    if(!essid) return 0;
    return strlen(essid);
}

/**
 * @brief test if a string is a correct key
 * @param key the key
 * @param encryption_mode the password mode (WEP, WPA ...)
 * @return Return 1 if success, else 0
 */
short exalt_is_key(const char* key, Exalt_Enum_Encryption_Mode encryption_mode)
{
    Exalt_Regex *r;
    short res;
    if(encryption_mode == EXALT_ENCRYPTION_NONE)
        return 1;

    if(!key)
        return -0;

    if(encryption_mode == EXALT_ENCRYPTION_WEP_ASCII)
    {
        if(strlen(key)>0)
            return 1;
        else
            return 0;
    }

    if(encryption_mode == EXALT_ENCRYPTION_WEP_HEXA)
    {
        r = exalt_regex_create(strdup(key), REGEXP_IS_WEP_HEXA, 0);
        res = exalt_regex_execute(r);
        exalt_regex_free(&r);
        return res;
    }

    if(encryption_mode == EXALT_ENCRYPTION_WPA_PSK_TKIP_ASCII
            || encryption_mode ==EXALT_ENCRYPTION_WPA_PSK_CCMP_ASCII
            || encryption_mode == EXALT_ENCRYPTION_WPA2_PSK_TKIP_ASCII
            || encryption_mode == EXALT_ENCRYPTION_WPA2_PSK_CCMP_ASCII)
    {
        if(strlen(key)>=8 && strlen(key)<=63)
            return 1;
        else
            return 0;
    }

    return 0;
}

/**
 * @brief return if we have the administrator right
 * @return Return EXALT_TRUE if yes, else EXALT_FALSE
 */
short exalt_is_admin()
{
    return exalt_eth_interfaces.admin;
}

/**
 * @brief return if libexalt is built with the support of wpa_supplicant
 * @return Return 1 if yes, else 0
 */
short exalt_wpasupplicant_is_support()
{
#ifdef WPA_SUPPLICANT_COMMAND_PATH
    return 1;
#else
    return 0;
#endif
}


/**
 * @brief return if libexalt is built with the support of dhcp
 * @return Return 1 if yes, else 0
 */
short exalt_dhcp_is_support()
{
#ifdef DHCP_COMMAND_PATH
    return 1;
#else
    return 0;
#endif
}
