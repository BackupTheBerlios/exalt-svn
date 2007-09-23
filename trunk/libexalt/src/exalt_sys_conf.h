/** @file exalt_sys_conf.h */
#ifndef EXALT_SYS_CONF_H
#define EXALT_SYS_CONF_H

/**
 * @defgroup Exalt_System_Configuration
 * @brief all functions to save or load a configuration
 * @{
 */

#include <stdio.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <fcntl.h>
#include <unistd.h>
#include <Ecore_File.h>
#include "libexalt.h"
#include <Eet.h>
#include <Evas.h>

/** the configuration file of Exalt */
#define EXALT_CONF_FILE_EET PACKAGE_DATA_DIR "/exalt.conf.eet"


/** the location with the file configuration of wpa_supplicant */
#define EXALT_WPA_CONF_FILE_DIR "/etc/wpa_supplicant"
/** the location of the wpa_supplicant configuration file */
#define EXALT_WPA_CONF_FILE  EXALT_WPA_CONF_FILE_DIR "/wpa_supplicant.conf"
/** the file with interfaces of the wpa_supplicant daemon */
#define EXALT_WPA_INTERFACE_DIR "/var/run/wpa_supplicant"
/** the header of the wpa_supplicant configuration file */
#define EXALT_WPA_CONF_HEADER "ctrl_interface=" EXALT_WPA_INTERFACE_DIR "\n" \
                        "ctrl_interface_group=0\n" \
                        "eapol_version=1\n" \
                        "ap_scan=1\n" \
                        "fast_reauth=1\n"


int exalt_conf_wirelessinfo_save(exalt_wireless* w);
exalt_wireless_info* exalt_conf_wirelessinfo_load(const char*essid);

int exalt_conf_save_wpasupplicant(exalt_wireless *w);

int exalt_conf_save(exalt_ethernet* eth);
exalt_ethernet* exalt_conf_load(const char* name);

#endif

/** @} */
