#ifndef COMMANDE_H
#define COMMANDE_H

#include <stdio.h>
#include <stdlib.h>
#include "libexalt.h"
#include "../config.h"

/** Command to lauch the wpa_supplicant daemon */
#ifdef WPA_SUPPLICANT_COMMAND_PATH
    #define COMMAND_WPA WPA_SUPPLICANT_COMMAND_PATH " -D%s -i%s -c%s -P%s -B"
#endif

/** Command to launch a dhcp */
#ifdef DHCP_COMMAND_PATH
    #define COMMAND_DHCLIENT DHCP_COMMAND_PATH " %s -1 -q"
#endif

/** Command to get the dns list */
#define DNS_GET_LIST "grep nameserver /etc/resolv.conf | cut -d' ' -f2"
/** Command to add a dns */
#define DNS_ADD "echo \"nameserver %s\" >> /etc/resolv.conf"
/** Command to delete a dns */
#define DNS_DELETE "sed -i '/^nameserver %s$/d' /etc/resolv.conf"
/** Command to replace a dns */
#define DNS_REPLACE "sed -i 's/^nameserver %s$/nameserver %s/g' /etc/resolv.conf"



FILE* exalt_execute_command(char* cmd);


#endif

