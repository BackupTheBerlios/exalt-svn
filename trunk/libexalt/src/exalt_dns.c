/** @file exalt_dns.c */
#include "exalt_dns.h"
#include "libexalt_private.h"

/**
 * @addtogroup Exalt_DNS
 * @{
 */


/**
 * @brief get the dns list
 * @param nb_dns the number of dns
 * @return Return the dns list
 */
char** exalt_dns_get_list(int* nb_dns)
{
	FILE* f;
	char **tab;
	int i;
	if(!nb_dns)
	{
		print_error("ERROR", __FILE__, __LINE__,__func__,"nb_dns=%p",nb_dns);
		return NULL;
	}

	*nb_dns = 0;
	tab=(char**)malloc((unsigned int)sizeof(char*)*NB_DNS_MAX);
	if(!tab)
	{
		print_error("ERROR", __FILE__, __LINE__,__func__,"tab=%p, malloc() error", tab);
	}

	for(i=0;i<NB_DNS_MAX;i++)
		tab[i] = (char*)malloc((unsigned int)sizeof(char)*DNS_LENGTH);


	f = exalt_execute_command(DNS_GET_LIST);

	while(fgets(tab[*nb_dns],DNS_LENGTH,f) && *nb_dns<NB_DNS_MAX)
	{
		tab[*nb_dns][strlen(tab[*nb_dns])-1] = '\0';
		(*nb_dns)++;
	}
	EXALT_PCLOSE(f);
	return tab;
}



/**
 * @brief free the dns list
 * @param t the dns list
 * @param nb_dns the number of dns
 */
void exalt_dns_free_list(char** t, int nb_dns)
{
	int i;
	if(!t)
	{
		print_error("ERROR", __FILE__, __LINE__,__func__,"t=%p", t);
	}
	for(i=0;i<nb_dns;i++)
		EXALT_FREE(t[i]);
	EXALT_FREE(t);
}



/**
 * @brief add a dns
 * @param dns the news dns
 * @return Return 1 if the dns is add, else -1
 */
int exalt_dns_add(const char* dns)
{
	char buf[1024];
	FILE* f;
	if(!dns)
	{
		print_error("ERROR", __FILE__, __LINE__,__func__,"dns=%p", dns);
		return -1;
	}

	if(!exalt_is_address(dns))
	{
	 	print_error("WARNING", __FILE__, __LINE__,__func__,"dns(%s) is not a valid address",dns);
		return -1;
	}

	sprintf(buf,DNS_ADD,dns);
	f = exalt_execute_command(buf);
	while(fgets(buf,1024,f))
		;
	EXALT_PCLOSE(f);
	return 1;
}



/**
 * @brief delete a dns
 * @param dns the dns
 * @return Return 1 if the dns is delet, else -1
 */
int exalt_dns_delete(const char* dns)
{
	char buf[1024];
	FILE* f;
	if(!dns)
	{
		print_error("ERROR", __FILE__, __LINE__,__func__,"dns=%p", dns);
		return -1;
	}

	sprintf(buf,DNS_DELETE,dns);
	f = exalt_execute_command(buf);
	while(fgets(buf,1024,f))
		;
	EXALT_PCLOSE(f);
	return 1;
}



/**
 * @brief replace a dns by a new
 * @param old_dns the old dns
 * @param new_dns the new dns
 * @return Return 1 if the dns is repalce, else -1
 */
int exalt_dns_replace(const char* old_dns, const char* new_dns)
{
	char buf[1024];
	FILE* f;
	if(!old_dns || !new_dns)
	{
		print_error("ERROR", __FILE__, __LINE__,__func__,"old_dns=%p  new_dns=%p",old_dns, new_dns);
		return -1;
	}

	if(!exalt_is_address(new_dns))
	{
	 	print_error("WARNING", __FILE__, __LINE__,__func__,"dns(%s) is not a valid address",new_dns);
		return -1;
	}

	sprintf(buf,DNS_REPLACE,old_dns,new_dns);
	f = exalt_execute_command(buf);
	while(fgets(buf,1024,f))
		;
	EXALT_PCLOSE(f);
	return 1;
}



/**
 * @brief print the dns list in the standard output
 */
void exalt_dns_printf()
{
	int nb_dns;
	int i;
	char** t = exalt_dns_get_list(&nb_dns);
	printf("## DNS LIST ##\n");
	for(i=0;i<nb_dns;i++)
		printf("%s\n",t[i]);
	exalt_dns_free_list(t,nb_dns);
}


/** @} */

