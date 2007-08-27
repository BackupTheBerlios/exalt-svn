/** @file libexalt.c */
#include "libexalt.h"
#include "config.h"

/**
 * @addtogroup Exalt_common
 * @{
 */


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
 * @brief convert a hexadecimal address to a decimal address (xxx.xxx.xxx.xxx)
 * @param addr the address
 * @return Return the address in decimal format
 */
char* exalt_addr_hexa_to_dec(char* addr)
{
    char *res;
    char buf[4];
    char* end;
    int i;
    int n;
    if(strlen(addr)!=8)
    {
        fprintf(stderr,"addr_hexa_to_dec(): the hexadecimal address is not correct: %s\n",addr);
        return NULL;
    }

    res = (char*)malloc((unsigned int)sizeof(char)*16);
    res[0] = '\0';
    for(i=0;i<8;i+=2)
    {
        buf[0] = addr[7-i-1];
        buf[1] = addr[7-i];
        buf[2] = '\0';
        n = strtoul(buf,&end,16);
        sprintf(buf,"%d",n);
        strcat(res,buf);
        if(i<6)
            strcat(res,".");
    }
    return res;
}

/**
 * @brief remove a substring in a string
 * @param s1 the string
 * @param ct the substring
 * @return Return the new string
 */
char *str_remove (const char *s, const char *ct)
{
    char *new_s = NULL;
    char* start = strstr(s,ct);
    char* end_s = s + strlen(s)+1;
    char* c_new_s;
    if (s && ct && start)
    {
        size_t size = strlen (s) - strlen(ct);

        new_s = malloc (sizeof (char) * (size + 1));
        c_new_s = new_s;
        if (new_s)
        {
            while(s<end_s)
            {
                if(s<start || s>=start + strlen(ct))
                {
                    *c_new_s = *s;
                    c_new_s++;
                }
                s++;
            }
        }
    }
    return new_s;
}

/**
 * @brief execute a ioctl call
 * @param argp the strucuture with data (struct ifreq, rtentry, iwreq)
 * @param request the request key (SIOCGIWNAME ...)
 * @return Return 1 if ok, else -1
 */
short exalt_ioctl(void* argp, int request)
{
    int fd;

    //edit param: SIOCSIFFLAGS SIOCSIFFLAGS SIOCDELRT SIOCSIFADDR SIOCSIFNETMASK SIOCADDRT
    //read param: SIOCGIWNAME SIOCGIWESSID SIOCGIWNAME SIOCGIFFLAGS SIOCGIFADDR SIOCGIFNETMASK SIOCGIFHWADDR SIOCETHTOOL


    if(!exalt_is_admin() &&
            ( request == SIOCSIFFLAGS || request == SIOCSIFFLAGS
              || request == SIOCDELRT || request == SIOCSIFADDR
              || request == SIOCSIFNETMASK || request == SIOCADDRT))
    {
        fprintf(stderr,"exalt_iotl(): you need to be admnistrator if you want modify the configuration !\n");
        return -2;
    }

    if(!argp)
    {
        fprintf(stderr,"exalt_iotl(): argp==null ! \n");
        return -3;
    }

    fd=iw_sockets_open();
    if (fd < 0)
    {
        fprintf(stderr,"exalt_ifreq_iotl(): fd==%d",fd);
        return -4;
    }

    if( ioctl(fd, request, argp) < 0)
    {
        fprintf(stderr,"exalt_ifreq_iotl(): ioctl (%d): %s\n",request,strerror(errno));
        close(fd);
        return -5;
    }
    close(fd);
    return 1;
}

/**
 * @brief return if we have the administrator right
 * @return Return EXALT_TRUE if yes, else EXALT_FALSE
 */
short exalt_is_admin()
{
    return exalt_eth_interfaces.admin;
}
