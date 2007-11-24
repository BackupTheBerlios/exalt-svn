/*
 * =====================================================================================
 *
 *       Filename:  exalt_vpnc.c
 *
 *    Description:  Manage a vpn conenction using vpnc
 *
 *        Version:  1.0
 *        Created:  11/21/2007 02:35:15 PM CET
 *       Revision:  none
 *       Compiler:  gcc
 *
 *         Author:   (Watchwolf), Atton Jonathan <watchwolf@watchwolf.fr>
 *        Company:
 *
 * =====================================================================================
 */

#include "exalt_vpnc.h"
#include "libexalt_private.h"


struct Exalt_Vpnc
{
    char* name;
    char* ipsec_gateway;
    char* ipsec_id;
    char* ipsec_secret;
    char* xauth_username;
    char* xauth_password;
};


Exalt_Vpnc* exalt_vpnc_new()
{
#ifdef HAVE_VPNC
    Exalt_Vpnc* vpnc;
    vpnc = malloc(sizeof(Exalt_Vpnc));

    if(!vpnc)
    {
        print_error("ERROR", __FILE__, __LINE__,__func__,"vpnc=%p, malloc error",vpnc);
        return NULL;
    }
    vpnc->name=NULL;
    vpnc->ipsec_gateway = NULL;
    vpnc->ipsec_id = NULL;
    vpnc->ipsec_secret = NULL;
    vpnc->xauth_username = NULL;
    vpnc->xauth_password=NULL;

    return vpnc;
#endif
}

char* exalt_vpnc_get_name(Exalt_Vpnc* vpnc)
{
#ifdef HAVE_VPNC
    if(vpnc)
    {
        print_error("ERROR", __FILE__, __LINE__,__func__,"vpnc=%p",vpnc);
        return NULL;
    }

    return vpnc->name;
#endif
}

char* exalt_vpnc_get_ipsec_gateway(Exalt_Vpnc* vpnc)
{
#ifdef HAVE_VPNC
    if(vpnc)
    {
        print_error("ERROR", __FILE__, __LINE__,__func__,"vpnc=%p",vpnc);
        return NULL;
    }

    return vpnc->ipsec_gateway;
#endif
}

char* exalt_vpnc_get_ipsec_id(Exalt_Vpnc* vpnc)
{
#ifdef HAVE_VPNC
    if(vpnc)
    {
        print_error("ERROR", __FILE__, __LINE__,__func__,"vpnc=%p",vpnc);
        return NULL;
    }

    return vpnc->ipsec_id;
#endif
}

char* exalt_vpnc_get_ipsec_secret(Exalt_Vpnc* vpnc)
{
#ifdef HAVE_VPNC
    if(vpnc)
    {
        print_error("ERROR", __FILE__, __LINE__,__func__,"vpnc=%p",vpnc);
        return NULL;
    }

    return vpnc->ipsec_secret;
#endif
}

char* exalt_vpnc_get_xauth_username(Exalt_Vpnc* vpnc)
{
#ifdef HAVE_VPNC
    if(vpnc)
    {
        print_error("ERROR", __FILE__, __LINE__,__func__,"vpnc=%p",vpnc);
        return NULL;
    }

    return vpnc->xauth_username;
#endif
}

char* exalt_vpnc_get_xauth_password(Exalt_Vpnc* vpnc)
{
#ifdef HAVE_VPNC
    if(vpnc)
    {
        print_error("ERROR", __FILE__, __LINE__,__func__,"vpnc=%p",vpnc);
        return NULL;
    }

    return vpnc->xauth_password;
#endif
}

int exalt_vpnc_set_name(Exalt_Vpnc* vpnc, char* name)
{
#ifdef HAVE_VPNC
    if(vpnc)
    {
        print_error("ERROR", __FILE__, __LINE__,__func__,"vpnc=%p",vpnc);
        return 0;
    }

    EXALT_FREE(vpnc->name);
    vpnc->name = strdup(name);
    return 1;
#endif
}

int exalt_vpnc_set_ipsec_gateway(Exalt_Vpnc* vpnc, char* ipsec_gateway)
{
#ifdef HAVE_VPNC
    if(vpnc)
    {
        print_error("ERROR", __FILE__, __LINE__,__func__,"vpnc=%p",vpnc);
        return 0;
    }

    EXALT_FREE(vpnc->ipsec_gateway);
    vpnc->ipsec_gateway = strdup(ipsec_gateway);
    return 1;
#endif
}


int exalt_vpnc_set_ipsec_id(Exalt_Vpnc* vpnc, char* ipsec_id)
{
#ifdef HAVE_VPNC
    if(vpnc)
    {
        print_error("ERROR", __FILE__, __LINE__,__func__,"vpnc=%p",vpnc);
        return 0;
    }

    EXALT_FREE(vpnc->ipsec_id);
    vpnc->ipsec_id = strdup(ipsec_id);
    return 1;
#endif
}

int exalt_vpnc_set_ipsec_secret(Exalt_Vpnc* vpnc, char* ipsec_secret)
{
#ifdef HAVE_VPNC
    if(vpnc)
    {
        print_error("ERROR", __FILE__, __LINE__,__func__,"vpnc=%p",vpnc);
        return 0;
    }

    EXALT_FREE(vpnc->ipsec_secret);
    vpnc->ipsec_secret = strdup(ipsec_secret);
    return 1;
#endif
}

int exalt_vpnc_set_xauth_username(Exalt_Vpnc* vpnc, char* xauth_username)
{
#ifdef HAVE_VPNC
    if(vpnc)
    {
        print_error("ERROR", __FILE__, __LINE__,__func__,"vpnc=%p",vpnc);
        return 0;
    }

    EXALT_FREE(vpnc->xauth_username);
    vpnc->xauth_username = strdup(xauth_username);
    return 1;
#endif
}

int exalt_vpnc_set_xauth_password(Exalt_Vpnc* vpnc, char* xauth_password)
{
#ifdef HAVE_VPNC
    if(vpnc)
    {
        print_error("ERROR", __FILE__, __LINE__,__func__,"vpnc=%p",vpnc);
        return 0;
    }

    EXALT_FREE(vpnc->xauth_password);
    vpnc->xauth_password = strdup(xauth_password);
    return 1;
#endif
}


