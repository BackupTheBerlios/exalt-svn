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
    exalt_regex *r;
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
 * @brief test if a string is a correct password
 * @param passwd the password
 * @param passwd_mode the password mode (WEP, WPA ...)
 * @return Return 1 if success, else 0
 */
short exalt_is_passwd(const char* passwd, int passwd_mode)
{
    exalt_regex *r;
    short res;
    if(passwd_mode == EXALT_WIRELESS_ENCRYPTION_NONE)
        return 1;

    if(!passwd)
        return -0;

    if(passwd_mode == EXALT_WIRELESS_ENCRYPTION_WEP_ASCII)
    {
        if(strlen(passwd)>0)
            return 1;
        else
            return 0;
    }

    if(passwd_mode == EXALT_WIRELESS_ENCRYPTION_WEP_HEXA)
    {
        r = exalt_regex_create(strdup(passwd), REGEXP_IS_WEP_HEXA, 0);
        res = exalt_regex_execute(r);
        exalt_regex_free(&r);
        return res;
    }

    if(passwd_mode == EXALT_WIRELESS_ENCRYPTION_WPA_PSK_TKIP_ASCII
            || passwd_mode ==EXALT_WIRELESS_ENCRYPTION_WPA_PSK_CCMP_ASCII
            || passwd_mode == EXALT_WIRELESS_ENCRYPTION_WPA2_PSK_TKIP_ASCII
            || passwd_mode == EXALT_WIRELESS_ENCRYPTION_WPA2_PSK_CCMP_ASCII)
    {
        if(strlen(passwd)>=8 && strlen(passwd)<=63)
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
