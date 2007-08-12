#include "gentoo.h"

int exalt_sys_conf_save(exalt_ethernet* eth)
{
 	char save[1024];
	FILE* fr,*fw;
	char buf[1024];
        char security[1024];
        char key[1024];
	short find ;
        char* driver = NULL;
        exalt_wireless* w;

	if(!eth)
	{
		fprintf(stderr,"exalt_sys_conf_save(): eth==null ! \n");
		return EXALT_ERROR;
	}
        w = exalt_eth_get_wireless(eth);

        if(!exalt_is_admin())
        {
            fprintf(stderr,"exalt_sys_conf_save(): you need to be administrator to save the configuration file! \n");
            return EXALT_ERROR;
        }

 	if(!exalt_file_exist(CONF_FILE))
	 	if(exalt_file_create(CONF_FILE,NULL)==-1)
		{
			fprintf(stderr,"exalt_sys_conf_save(): error can't create the file! \n");
			return EXALT_ERROR;
		}


        //BEFORE MODIFY THE FILE, WE NEED TO GET THE DRIVER IF WE USE A WIRELESS CARD
        if(w)
            driver = exalt_wireless_get_driver(w);

 	exalt_file_create(FILE_TEMP,NULL);
 	//copy the conf file in the temp file
	fw = fopen(FILE_TEMP,"w");
	fr = fopen(CONF_FILE,"r");
	while(fgets(buf,1024,fr))
	 	 fprintf(fw,buf);
 	fclose(fw);
	fclose(fr);

 	//apply modification
  	sprintf(save,"config_%s=(",exalt_eth_get_name(eth));
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
        {
            fprintf(fw,"%s \"dhcp\" )\n",save);
        }
        else
        {
            fprintf(fw,"%s \"%s netmask %s\" )\n",save,exalt_eth_get_new_ip(eth), exalt_eth_get_new_netmask(eth));
        }

        if(w)
        {
            switch(exalt_wireless_get_new_passwd_mode(w))
            {
                case EXALT_WIRELESS_ENCRYPTION_WPA_PSK_CCMP_ASCII:
                case EXALT_WIRELESS_ENCRYPTION_WPA_PSK_TKIP_ASCII:
                case EXALT_WIRELESS_ENCRYPTION_WPA2_PSK_CCMP_ASCII:
                case EXALT_WIRELESS_ENCRYPTION_WPA2_PSK_TKIP_ASCII:
                    fprintf(fw,"modules=( \"wpa_supplicant\" )\n");
                    break;
                default: //WEP, no key
                    switch(exalt_wireless_get_new_mode(w))
                    {
                        case EXALT_WIRELESS_MODE_ADHOC:
                            fprintf(fw,"mode_%s=\"ad-hoc\"\n", exalt_eth_get_name(eth));
                            break;
                        case EXALT_WIRELESS_MODE_MANAGED:
                            fprintf(fw,"mode_%s=\"managed\"\n", exalt_eth_get_name(eth));
                            break;
                    }

                    fprintf(fw,"essid_%s=\"%s\"\n", exalt_eth_get_name(eth), exalt_wireless_get_new_essid(w));

                    if(exalt_wireless_get_new_passwd_mode(w) != EXALT_WIRELESS_ENCRYPTION_NONE)
                    {
                        switch(exalt_wireless_get_new_security_mode(w))
                        {
                            case EXALT_WIRELESS_SECURITY_OPEN:
                                strncpy(security,"open", 1024);
                                break;
                            case EXALT_WIRELESS_SECURITY_SHARED:
                                strncpy(security,"shared",1024);
                                break;
                        }

                        switch(exalt_wireless_get_new_passwd_mode(w))
                        {
                            case EXALT_WIRELESS_ENCRYPTION_WEP_ASCII:
                                sprintf(key,"s:%s",exalt_wireless_get_new_passwd(w));
                                break;
                            case EXALT_WIRELESS_ENCRYPTION_WEP_HEXA:
                                strncpy(key, exalt_wireless_get_new_passwd(w), 1024);
                                break;
                        }
                        fprintf(fw,"key_%s=\"[1] %s [1] enc %s\"\n",exalt_eth_get_name(eth)
                                ,key,security);
                    }
                    fprintf(fw,"preferred_aps=( \"%s\" )\n",exalt_wireless_get_new_essid(w));
                    break;
            }
            //we need to save the driver
            fprintf(fw,"wpa_supplicant_%s=\"-D%s\"\n",exalt_eth_get_name(eth),driver);
        }
	fprintf(fw,"\n");

        //jump the previous configuration
        find = 0;
        while(fgets(buf,1024,fr) &&!find  && buf[0]!=')')
        {
            if(buf[0]=='\n' || strncmp("config_",buf,strlen("config_"))==0 )
            {
                find = 1;
                fprintf(fw,buf);
            }
        }
        //try to find the default gateway configuration line
        sprintf(save,"routes_%s=(",exalt_eth_get_name(eth));
	find = 0;
	while(!find && fgets(buf,1024,fr))
	{
		if(strncmp(save,buf,strlen(save))==0)
			find = 1;
		else
			fprintf(fw,buf);
	}

        if(exalt_is_address(exalt_eth_get_gateway(eth)) )
        {
            if(!exalt_eth_is_new_dhcp(eth))
            {
                //add the new gateway configuration
                fprintf(fw,"%s \"default via %s\" )\n",save, exalt_eth_get_gateway(eth));
            }
        }
	//copy next lines
	while(fgets(buf,1024,fr))
        {
             printf("BUF: %s",buf);
	 	fprintf(fw,buf);
        }
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
	char buf[1024];
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

 	if(exalt_eth_is_up(eth))
	{
	 	sprintf(buf,COMMAND_AUTO_LOAD,exalt_eth_get_name(eth));
	}
	else
	{
 	 	sprintf(buf,COMMAND_DEL_AUTO_LOAD,exalt_eth_get_name(eth));
	}

	exalt_execute_command(buf);

	return 1;
}

int exalt_sys_conf_is_dhcp(exalt_ethernet* eth)
{
	FILE * fr;
	char buf[1024];
	char save[1024];
	short find;
 	exalt_regex* r;

	if(!eth)
	{
		fprintf(stderr,"exalt_sys_conf_is_dhcp(): eth==null ! \n");
		return EXALT_ERROR;
	}

	fr = fopen(CONF_FILE,"r");
	if(!fr)
	{
	 	fprintf(stderr,"exalt_sys_conf_is_dhcp(): the configuration file doesn't exist !\n");
		return EXALT_ERROR;
	}

 	sprintf(save,"config_%s=(",exalt_eth_get_name(eth));
	find = 0;
	while(!find && fgets(buf,1024,fr))
		if(strncmp(save,buf,strlen(save))==0)
		 	find = 1;

	if(find)
	{
 	 	r = exalt_regex_create(buf,REGEXP_SAVE_GENTOO_IS_DHCP,0);
		if(exalt_regex_execute(r))
		{
		    fclose(fr);
                    return EXALT_DHCP;
		}
		else
		{
                    fclose(fr);
                    return EXALT_STATIC;
		}
		exalt_regex_free(&r);
	}

 	fclose(fr);

	return EXALT_ERROR;
}

char* exalt_sys_conf_get_driver(exalt_wireless *w)
{
    FILE * fr;
    char buf[1024];
    char save[1024];
    short find;
    exalt_regex* r;
    char* driver=NULL;

    if(!w)
    {
        fprintf(stderr,"exalt_sys_conf_get_driver(): w==null ! \n");
        return NULL;
    }

    fr = fopen(CONF_FILE,"r");
    if(!fr)
    {
        fprintf(stderr,"exalt_sys_conf_get_driver(): the configuration file doesn't exist !\n");
        return NULL;
    }
        find = EXALT_FALSE;

        sprintf(save,"wpa_supplicant_%s=",exalt_eth_get_name(exalt_wireless_get_eth(w)));
    while(!find && fgets(buf,1024,fr))
        if(strncmp(save,buf,strlen(save))==0)
            find = 1;
    if(find)
    {
        r = exalt_regex_create(buf,REGEXP_SAVE_GENTOO_GET_DRIVER,0);
        if(exalt_regex_execute(r) && r->nmatch>0)
            driver = strdup(r->res[1]);
        exalt_regex_free(&r);
    }
    fclose(fr);
    return driver;
}
