/** @file default.c */
#include "default.h"

/**
 * @addgroup Exalt_save_load
 *@{
 */


/**
 * @brief save the configuration of one interface in the file distribution (interfaces, net ...)
 * @param eth the interface
 * @return Return 1 if success, else -1
 */
int exalt_sys_conf_save(exalt_ethernet* eth)
{
 	fprintf(stderr,"Configuration save is not yet implemented for your distribution\n");
	return EXALT_ERROR;
}

/**
 * @brief save if the interface is load (or not) when the system start
 * @param eth the interface
 * @return Return 1 if sucess, else -1
 */
int exalt_sys_conf_save_autoload(exalt_ethernet* eth)
{
 	fprintf(stderr,"Configuration save is not yet implemented for your distribution\n");
	return EXALT_ERROR;
}

/**
 * @brief test if we use a dhcp or a static method
 * @param eth the interface
 * @return Return EXALT_TRUE if we use dhcp, EXALT_FALSE for a static method, else EXALT_ERROR
 */
int exalt_sys_conf_is_dhcp(exalt_ethernet* eth)
{
    fprintf(stderr,"exalt_sys_conf_is_dhcp is not yet implemented for your distribution\n");
    return EXALT_ERROR;
}

/**
 * @brief return the wireless driver
 * @param w the interface
 * @return Return the driver
 */
char* exalt_sys_conf_get_driver(exalt_wireless *w)
{
    fprintf(stderr,"exalt_sys_conf_get_driver is not yet implemented for your distribution\n");
    return NULL;
}

/** @} */

