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
    char *addr;
    Ecore_List* l;

    f = fopen(EXALT_RESOLVCONF_FILE, "ro");
    if(!f)
    {
        print_error("ERROR", __FILE__, __LINE__,__func__,"Can't open the file: %s\n", EXALT_RESOLVCONF_FILE);
        return NULL;
    }

    l = ecore_list_new();
    l->free_func = free;
    while(fgets(buf,1024,f))
    {
        buf[strlen(buf)-1] = '\0';
        //jump nameserver
        if(strlen(buf) > 13)
        {
            addr = buf + 11;
            if(exalt_is_address(addr))
                ecore_list_append(l, strdup(addr));
        }
    }
    EXALT_FCLOSE(f);
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
        return 0;
    }

    if(!exalt_is_address(dns))
    {
        print_error("WARNING", __FILE__, __LINE__,__func__,"dns(%s) is not a valid address",dns);
        return 0;
    }

    f = fopen(EXALT_RESOLVCONF_FILE, "a");
    if(!f)
    {
        print_error("ERROR", __FILE__, __LINE__,__func__,"Can't open the file: %s\n", EXALT_RESOLVCONF_FILE);
        return 0;
    }

    sprintf(buf,"nameserver %s\n", dns);
    fwrite( buf, sizeof(char), strlen(buf), f);

    EXALT_FCLOSE(f);
    return 1;
}



/**
 * @brief delete a dns
 * @param dns the dns
 * @return Return 1 if the dns is delet, else -1
 */
int exalt_dns_delete(const char* dns)
{
    char buf[1024], buf2[1024];
    FILE* fw, *fr;
    if(!dns)
    {
        print_error("ERROR", __FILE__, __LINE__,__func__,"dns=%p", dns);
        return -1;
    }

    ecore_file_cp(EXALT_RESOLVCONF_FILE, EXALT_TEMP_FILE);

    fw = fopen(EXALT_RESOLVCONF_FILE, "w");
    fr = fopen(EXALT_TEMP_FILE, "ro");

    if(!fw || !fr)
    {
        print_error("ERROR", __FILE__, __LINE__,__func__,"Can't open one of these file: %s %s\n", EXALT_RESOLVCONF_FILE, EXALT_TEMP_FILE);
        return 0;
    }

    sprintf(buf,"nameserver %s\n",dns);
    while(fgets(buf2,1024,fr))
        if( strcmp(buf,buf2) != 0)
            fwrite( buf2, sizeof(char), strlen(buf2), fw);
    EXALT_FCLOSE(fr);
    EXALT_FCLOSE(fw);
    remove(EXALT_TEMP_FILE);
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
    char buf[1024], buf2[1024], buf3[1024];;
    FILE* fw, *fr;

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

    ecore_file_cp(EXALT_RESOLVCONF_FILE, EXALT_TEMP_FILE);

    fw = fopen(EXALT_RESOLVCONF_FILE, "w");
    fr = fopen(EXALT_TEMP_FILE, "ro");

    if(!fw || !fr)
    {
        print_error("ERROR", __FILE__, __LINE__,__func__,"Can't open one of these file: %s %s\n", EXALT_RESOLVCONF_FILE, EXALT_TEMP_FILE);
        return 0;
    }

    sprintf(buf,"nameserver %s\n",old_dns);
    sprintf(buf3,"nameserver %s\n",new_dns);
    while(fgets(buf2,1024,fr))
        if( strcmp(buf,buf2) != 0)
            fwrite( buf2, sizeof(char), strlen(buf2), fw);
        else
            fwrite( buf3, sizeof(char), strlen(buf3), fw);
    EXALT_FCLOSE(fr);
    EXALT_FCLOSE(fw);
    remove(EXALT_TEMP_FILE);
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

