/** @file exalt_sys_conf.h */
#ifndef EXALT_SYS_CONF_H
#define EXALT_SYS_CONF_H

/**
 * @defgroup Exalt_save_load
 * @brief all functions to save or load a configuration
 * @{
 */

#include <stdio.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <fcntl.h>
#include <unistd.h>
#include <Ecore_Config.h>
#include "libexalt.h"

/** the configuration file of Exalt */
#define EXALT_CONF_FILE PACKAGE_DATA_DIR "/exalt.conf"
/** The file temp */
#define FILE_TEMP "/tmp/exalt_tmp"

int exalt_file_create(char* file,char* header);
int exalt_file_exist(char* file);

int exalt_sys_conf_save(exalt_ethernet* eth);
int exalt_sys_conf_save_autoload(exalt_ethernet* eth);


int exalt_wireless_save(exalt_wireless* w);
int exalt_wireless_load(exalt_wireless_info* wi);

int exalt_sys_conf_save_wpasupplicant(exalt_wireless *w);

int exalt_sys_conf_is_dhcp(exalt_ethernet* eth);
char* exalt_sys_conf_get_driver(exalt_wireless *w);

#endif

/** @} */
