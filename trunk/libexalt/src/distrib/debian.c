#include "debian.h"
#include "libexalt_private.h"

int exalt_sys_conf_save(exalt_ethernet* eth)
{
 	char save[1024];
	FILE* fr,*fw;
	char buf[1024];
	short find ;
        char* driver=NULL;
        exalt_wireless* w;

	if(!eth)
	{
		fprintf(stderr,"exalt_sys_conf_save(): eth==null ! \n");
		return -1;
	}

        w = exalt_eth_get_wireless(eth);
        if(!exalt_is_admin())
        {
            fprintf(stderr,"exalt_sys_conf_save(): you need to be administrator to save the configuration file! \n");
            return-1;
        }

 	if(!exalt_file_exist(CONF_FILE))
	 	if(exalt_file_create(CONF_FILE,NULL)==-1)
		{
			fprintf(stderr,"exalt_sys_conf_save(): error can't create the file! \n");
			return -1;
		}

        if(w)
            driver=exalt_wireless_get_driver(w);

 	exalt_file_create(FILE_TEMP,NULL);
 	//copy the conf file in the temp file
	fw = fopen(FILE_TEMP,"w");
	fr = fopen(CONF_FILE,"r");
	while(fgets(buf,1024,fr))
	 	 fprintf(fw,buf);
 	fclose(fw);
	fclose(fr);

 	//apply modification
  	sprintf(save,"iface %s inet",exalt_eth_get_name(eth));
	fr = fopen(FILE_TEMP,"r");
	fw = fopen(CONF_FILE,"w");
 	find = 0;
	while(!find && fgets(buf,1024,fr))
	{
		if(strncmp(save,buf,strlen(save))==0)
		 	find = 1;
		else
		 	fprintf(fw,buf);
	}

	//add the new configuration
	if(exalt_eth_is_new_dhcp(eth))
	 	fprintf(fw,"%s dhcp\n",save);
	else
	{
		fprintf(fw,"%s static\n",save);
		fprintf(fw,"address %s\n",exalt_eth_get_new_ip(eth));
		fprintf(fw,"netmask %s\n",exalt_eth_get_new_netmask(eth));
		if(exalt_is_address(exalt_eth_get_new_gateway(eth)))
                    fprintf(fw,"gateway %s\n",exalt_eth_get_new_gateway(eth));
	}

        if(w)
        {
            switch(exalt_wireless_get_new_passwd_mode(w))
            {
                case EXALT_WIRELESS_ENCRYPTION_WPA_PSK_CCMP_ASCII:
                case EXALT_WIRELESS_ENCRYPTION_WPA_PSK_TKIP_ASCII:
                case EXALT_WIRELESS_ENCRYPTION_WPA2_PSK_CCMP_ASCII:
                case EXALT_WIRELESS_ENCRYPTION_WPA2_PSK_TKIP_ASCII:
                    fprintf(fw,"wpa-conf /etc/wpa_supplicant.conf\n");
                    break;
                default: //WEP, no key
                    switch(exalt_wireless_get_new_mode(w))
                    {
                        case EXALT_WIRELESS_MODE_ADHOC:
                            fprintf(fw,"wireless-mode_ad-hoc\n");
                            break;
                        case EXALT_WIRELESS_MODE_MANAGED:
                            fprintf(fw,"wireless-mode managed\n");
                            break;
                    }

                    fprintf(fw,"wireless-essid %s\n", exalt_wireless_get_new_essid(w));

                    if(exalt_wireless_get_new_passwd_mode(w) != EXALT_WIRELESS_ENCRYPTION_NONE)
                    {
                        switch(exalt_wireless_get_new_security_mode(w))
                        {
                            case EXALT_WIRELESS_SECURITY_OPEN:
                                fprintf(fw,"wireless-keymode open\n");
                                break;
                            case EXALT_WIRELESS_SECURITY_SHARED:
                                fprintf(fw,"wireless-keymode shared\n");
                                break;
                        }

                        switch(exalt_wireless_get_new_passwd_mode(w))
                        {
                            case EXALT_WIRELESS_ENCRYPTION_WEP_ASCII:
                                fprintf(fw,"wireless-key s:%s\n",exalt_wireless_get_new_passwd(w));
                                break;
                            case EXALT_WIRELESS_ENCRYPTION_WEP_HEXA:
                                fprintf(fw,"wireless-key %s\n",exalt_wireless_get_new_passwd(w));
                                break;
                        }
                    }
                    break;
            }
            //we need to save the driver
            fprintf(fw,"wpa-driver %s\n",driver);
        }

        //jump the last configuration
        find = 0;
        while(!find && fgets(buf,1024,fr) )
            if (strncmp("iface",buf,strlen("iface"))==0 ||
                    strncmp("auto",buf,strlen("auto"))==0)
                {
                    fprintf(fw,buf);
                    find = 1;
                }

	//copy next lines
	while(fgets(buf,1024,fr))
	 	fprintf(fw,buf);

	fclose(fr);
	fclose(fw);

	//remove the temp file
	remove(FILE_TEMP);


        if(w)
            switch(exalt_wireless_get_new_passwd_mode(w))
            {
                case EXALT_WIRELESS_ENCRYPTION_WPA_PSK_CCMP_ASCII:
                case EXALT_WIRELESS_ENCRYPTION_WPA_PSK_TKIP_ASCII:
                case EXALT_WIRELESS_ENCRYPTION_WPA2_PSK_CCMP_ASCII:
                case EXALT_WIRELESS_ENCRYPTION_WPA2_PSK_TKIP_ASCII:
                    exalt_sys_conf_save_wpasupplicant(exalt_eth_get_wireless(eth));
                    break;
            }
        EXALT_FREE(driver)
	return 1;
}

int exalt_sys_conf_save_autoload(exalt_ethernet* eth)
{
 	FILE* fr,*fw;
	char buf[1024];
	char regexp[1024];
	char* res = NULL;
	exalt_regex * r;
	if(!eth)
	{
	 	fprintf(stderr,"exalt_sys_conf_save_autoload(): eth==null ! \n");
		return -1;
	}

        if(!exalt_is_admin())
        {
            fprintf(stderr,"exalt_sys_conf_save_autoload(): you need to be administrator to save the configuration file! \n");
            return-1;
        }

 	if(!exalt_file_exist(CONF_FILE))
	 	if(exalt_file_create(CONF_FILE,NULL)==-1)
		{
			fprintf(stderr,"exalt_sys_conf_save_autoload(): error can't create the file! \n");
			return -1;
		}

 	exalt_file_create(FILE_TEMP,NULL);
 	//copy the conf file in the temp file
	fw = fopen(FILE_TEMP,"w");
	fr = fopen(CONF_FILE,"r");
	while(fgets(buf,1024,fr))
	 	 fprintf(fw,buf);
 	fclose(fw);
	fclose(fr);

	//remove the current autoload for this card if it is present
	fr = fopen(FILE_TEMP,"r");
	fw = fopen(CONF_FILE,"w");
	sprintf(regexp,REGEXP_SAVE_DEBIAN_IS_AUTO_ETH, exalt_eth_get_name(eth));
	r = exalt_regex_create("",regexp,0);
	while(fgets(buf,1024,fr))
	{
	 	exalt_regex_set_request(r,buf);
		if( exalt_regex_execute(r) && r->nmatch>0 )
		{
                    printf("BEFORE STR_REMOVE\n");
			res = str_remove(buf,exalt_eth_get_name(eth));
			printf("AFTER REMOVE\n");
                        if(res)
			{
				exalt_regex_set_request(r,res);
			 	exalt_regex_set_regex(r,REGEXP_SAVE_DEBIAN_IS_AUTO_ALONE);
			 	if(!exalt_regex_execute(r))
			  	 	fprintf(fw,res);
                                else
                                    printf("IS ALONE\n");
			 	EXALT_FREE(res)
			  	exalt_regex_set_regex(r,REGEXP_SAVE_DEBIAN_IS_AUTO_ETH);
			}
                        EXALT_FREE(res)
	 	}
		else
		 	fprintf(fw,buf);
	}
        printf("BEFORE UP\n");
	if(exalt_eth_is_up(eth))
		fprintf(fw,"auto %s\n",exalt_eth_get_name(eth));


 	fclose(fw);
	fclose(fr);
 	exalt_regex_free(&r);
	return 1;
}

int exalt_sys_conf_is_dhcp(exalt_ethernet* eth)
{
    FILE * fr;
    char buf[1024];
    char save[1024];
    short find;
    exalt_regex* r;
    int res = EXALT_ERROR;

    if(!eth)
    {
        fprintf(stderr,"exalt_sys_conf_is_dhcp(): eth==null ! \n");
        return -1;
    }

    fr = fopen(CONF_FILE,"r");
    if(!fr)
    {
        fprintf(stderr,"exalt_sys_conf_is_dhcp(): the configuration file doesn't exist !\n");
        return 0;
    }

    sprintf(save,"iface %s inet",exalt_eth_get_name(eth));
    find = 0;
    while(!find && fgets(buf,1024,fr))
        if(strncmp(save,buf,strlen(save))==0)
            find = 1;

    r = exalt_regex_create(buf,REGEXP_SAVE_DEBIAN_IS_DHCP,0);
    if(exalt_regex_execute(r))
        res= EXALT_TRUE;
    else
        res= EXALT_FALSE;

    fclose(fr);
    exalt_regex_free(&r);
    return res;
}

char* exalt_sys_conf_get_driver(exalt_wireless *w)
{
    FILE * fr;
    char buf[1024];
    char save[1024];
    short find;
    exalt_regex* r;
    char* driver=NULL;
    exalt_ethernet* eth;

    if(!w)
    {
        fprintf(stderr,"exalt_sys_conf_get_driver(): w==null ! \n");
        return NULL;
    }

    eth = exalt_wireless_get_eth(w);
    fr = fopen(CONF_FILE,"r");
    if(!fr)
    {
        fprintf(stderr,"exalt_sys_conf_get_driver(): the configuration file doesn't exist !\n");
        return NULL;
    }

    sprintf(save,"iface %s inet",exalt_eth_get_name(eth));
    find = 0;
    while(!find && fgets(buf,1024,fr))
        if(strncmp(save,buf,strlen(save))==0)
            find = 1;

    find = 0;
    r = exalt_regex_create("""",REGEXP_SAVE_DEBIAN_GET_DRIVER,0);
    while(fgets(buf,1024,fr)  && strncmp("iface",buf,strlen("iface"))!=0  && strncmp("auto",buf,strlen("auto"))!=0 && !find)
    {
        exalt_regex_set_request(r,buf);
        if(exalt_regex_execute(r) && r->nmatch>0)
        {
            driver =  strdup(r->res[1]);
            find = 1;
        }
    }
    fclose(fr);
    exalt_regex_free(&r);
    return driver;
}
