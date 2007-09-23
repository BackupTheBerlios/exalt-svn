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
Ecore_List* exalt_dns_get_list()
{
	FILE* f;
        char buf[1024];
        Ecore_List* l;


	f = exalt_execute_command(DNS_GET_LIST);
        l = ecore_list_new();
        l->free_func = free;

	while(fgets(buf,1024,f))
        {
            buf[strlen(buf)-1] = '\0';
            ecore_list_append(l, strdup(buf));
        }
        EXALT_PCLOSE(f);
	return l;
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
	Ecore_List* l = exalt_dns_get_list();
	char *dns;

        printf("## DNS LIST ##\n");
        ecore_list_first_goto(l);
	while (( dns=ecore_list_next(l)))
		printf("%s\n",dns);
	ecore_list_destroy(l);
}


/** @} */

