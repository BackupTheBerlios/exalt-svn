/** @file exalt_sys_conf.c */
#include "exalt_sys_conf.h"
#include "config.h"
#include "libexalt_private.h"

typedef struct Exalt_Eth_Save
{
    Exalt_Enum_State state;
    char* driver;
    Exalt_Connection *connection;
}Exalt_Eth_Save;

Exalt_Eth_Save* _exalt_eet_eth_load(const char* file, const char* interface);
int _exalt_eet_eth_save(const char* file, Exalt_Eth_Save* s, const char* interface);
Eet_Data_Descriptor * _exalt_eth_save_edd_new();

Exalt_Connection* _exalt_eet_wireless_conn_load(const char* file, const char* essid);
int _exalt_eet_wireless_conn_save(const char* file,Exalt_Connection* c);



/**
 * @addgroup Exalt_System_Configuration
 *@{
 */



/**
 * @brief save the wpa_supplicant configuration for a wireless interface
 * @param w the wireless card
 * @return Return 1 if success, else -0
 */
int exalt_conf_save_wpasupplicant(Exalt_Wireless *w)
{
    FILE *fw;
    Exalt_Ethernet *eth;
    Exalt_Connection *c;
    int enc_mode;

    EXALT_ASSERT_QUIT(w!=NULL);

    eth = exalt_wireless_get_eth(w);
    c = exalt_eth_get_connection(eth);
    EXALT_ASSERT_QUIT(exalt_conn_is_valid(c));
    EXALT_ASSERT_QUIT(exalt_conn_is_wireless(c));

    //its more easy to recreate a new file
    //so we don't modify the old file, just delete it :)
    remove(EXALT_WPA_CONF_FILE);

    //recreate the file
    EXALT_ASSERT_QUIT(ecore_file_mkpath(EXALT_WPA_CONF_FILE_DIR) != 0);

    //save the new configuration
    fw = fopen(EXALT_WPA_CONF_FILE,"w");

    fprintf(fw,EXALT_WPA_CONF_HEADER);

    //add the new essid
    fprintf(fw,"network={\n");
    fprintf(fw,"\tssid=\"%s\"\n",exalt_conn_get_essid(c));

    enc_mode = exalt_conn_get_encryption_mode(c);

    if(enc_mode == EXALT_ENCRYPTION_WPA_PSK_CCMP_ASCII)
    {
        fprintf(fw,"\tscan_ssid=1\n");
        fprintf(fw,"\tproto=WPA\n");
        fprintf(fw,"\tkey_mgmt=WPA-PSK\n");
        fprintf(fw,"\tpairwise=CCMP\n");
        fprintf(fw,"\tgroup=CCMP\n");
        fprintf(fw,"\tpsk=\"%s\"\n",exalt_conn_get_key(c));
    }
    else if(enc_mode==EXALT_ENCRYPTION_WPA_PSK_TKIP_ASCII)
    {
        fprintf(fw,"\tscan_ssid=1\n");
        fprintf(fw,"\tproto=WPA\n");
        fprintf(fw,"\tkey_mgmt=WPA-PSK\n");
        fprintf(fw,"\tpairwise=TKIP\n");
        fprintf(fw,"\tgroup=TKIP\n");
        fprintf(fw,"\tpsk=\"%s\"\n",exalt_conn_get_key(c));
    }
    else if(enc_mode==EXALT_ENCRYPTION_WPA2_PSK_CCMP_ASCII)
    {
        fprintf(fw,"\tscan_ssid=1\n");
        fprintf(fw,"\tproto=WPA2\n");
        fprintf(fw,"\tkey_mgmt=WPA-PSK\n");
        fprintf(fw,"\tpairwise=CCMP\n");
        fprintf(fw,"\tgroup=CCMP\n");
        fprintf(fw,"\tpsk=\"%s\"\n",exalt_conn_get_key(c));
    }
    else if(enc_mode==EXALT_ENCRYPTION_WPA2_PSK_TKIP_ASCII)
    {
        fprintf(fw,"\tscan_ssid=1\n");
        fprintf(fw,"\tproto=WPA2\n");
        fprintf(fw,"\tkey_mgmt=WPA-PSK\n");
        fprintf(fw,"\tpairwise=TKIP\n");
        fprintf(fw,"\tgroup=TKIP\n");
        fprintf(fw,"\tpsk=\"%s\"\n",exalt_conn_get_key(c));
    }

    fprintf(fw,"}\n");

    fclose(fw);

    return 1;
}

/**
 * @brief save a connection
 * @param file the file where save
 * @param c the connection
 * @return Return 1 if success, else 0
 */
int exalt_wireless_conn_save(const char* file, Exalt_Connection* c)
{
    EXALT_ASSERT_QUIT(c!=NULL);
    return _exalt_eet_wireless_conn_save(file, c);
}

/**
 * @brief load the connection for a wireless network
 * @param file the file where load
 * @param essid the network
 * @return Return the conenction if success, else NULL
 */
Exalt_Connection* exalt_wireless_conn_load(const char* file, const char *essid)
{
    return _exalt_eet_wireless_conn_load(file,essid);
}


/**
 * @brief save the configuration of a card
 * @param eth the card
 * @return Return 1 if success, else 0
 */
int exalt_eth_save(const char* file, Exalt_Ethernet* eth)
{
    Exalt_Eth_Save s;

    EXALT_ASSERT_QUIT(eth!=NULL);

    s.state = _exalt_eth_get_state(eth);
    s.connection = exalt_eth_get_connection(eth);
    if(exalt_eth_is_wireless(eth))
        s.driver = exalt_wireless_get_wpasupplicant_driver(exalt_eth_get_wireless(eth));
    else
        s.driver = "wext";

    return _exalt_eet_eth_save(file, &s, exalt_eth_get_udi(eth));
}


Exalt_Enum_State exalt_eth_state_load(const char* file, const char* udi)
{
    Exalt_Eth_Save *s = _exalt_eet_eth_load(file, udi);
    EXALT_ASSERT_QUIT(s!=NULL);

    Exalt_Enum_State st = s->state;
    EXALT_FREE(s->driver);
    exalt_conn_free(s->connection);
    EXALT_FREE(s);
    return st;
}

char* exalt_eth_driver_load(const char* file, const char* udi)
{
    Exalt_Eth_Save *s = _exalt_eet_eth_load(file,  udi);
    EXALT_ASSERT_QUIT(s!=NULL);
    char* driver = s->driver;
    exalt_conn_free(s->connection);
    EXALT_FREE(s);
    return driver;
}


Exalt_Connection* exalt_eth_conn_load(const char* file, const char* udi)
{
    Exalt_Eth_Save *s = _exalt_eet_eth_load(file, udi);
    EXALT_ASSERT_QUIT(s!=NULL);
    Exalt_Connection *c = s->connection;
    EXALT_FREE(s->driver);
    EXALT_FREE(s);
    return c;
}


/** @} */



/* PRIVATES FUNCTIONS */

Exalt_Eth_Save* _exalt_eet_eth_load(const char* file, const char* udi)
{
    Exalt_Eth_Save *data = NULL;
    Eet_Data_Descriptor *edd;
    Eet_File *f;

    edd = _exalt_eth_save_edd_new();

    f = eet_open(file, EET_FILE_MODE_READ);
    EXALT_ASSERT_QUIT(f!=NULL);

    data = eet_data_read(f, edd, udi);

    eet_close(f);
    eet_data_descriptor_free(edd);
    return data;
}

int _exalt_eet_eth_save(const char* file, Exalt_Eth_Save* s, const char* interface)
{
    Eet_Data_Descriptor *edd;
    Eet_File* f;
    int res;

    edd = _exalt_eth_save_edd_new();
    f = eet_open(file, EET_FILE_MODE_READ_WRITE);
    if(!f)
        f = eet_open(file, EET_FILE_MODE_WRITE);
    res=eet_data_write(f, edd,interface, s, 0);
    EXALT_ASSERT(res!=0);

    eet_close(f);
    eet_data_descriptor_free(edd);
    return res;
}


Exalt_Connection* _exalt_eet_wireless_conn_load(const char*file, const char* essid)
{
    Exalt_Connection *data = NULL;
    Eet_Data_Descriptor *edd;
    Eet_File *f;

    edd = exalt_conn_edd_new();

    f = eet_open(file, EET_FILE_MODE_READ);
    EXALT_ASSERT_QUIT(f!=NULL);

    data = eet_data_read(f, edd, essid);

    eet_close(f);
    eet_data_descriptor_free(edd);
    return data;
}

int _exalt_eet_wireless_conn_save(const char*file, Exalt_Connection* c)
{
    Eet_Data_Descriptor* edd;
    Eet_File* f;
    int res;

    edd = exalt_conn_edd_new();
    f = eet_open(file, EET_FILE_MODE_READ_WRITE);
    if(!f)
        f = eet_open(file, EET_FILE_MODE_WRITE);
    res=eet_data_write(f, edd,exalt_conn_get_essid(c), c, 0);
    EXALT_ASSERT(res!=0);

    eet_close(f);
    eet_data_descriptor_free(edd);
    return res;
}

Eet_Data_Descriptor * _exalt_eth_save_edd_new()
{
    Eet_Data_Descriptor *edd, *edd_conn;

    edd_conn = exalt_conn_edd_new();

    edd = eet_data_descriptor_new("Interface", sizeof(Exalt_Eth_Save),
            evas_list_next,
            evas_list_append,
            evas_list_data,
            evas_list_free,
            evas_hash_foreach,
            evas_hash_add,
            evas_hash_free);

    EET_DATA_DESCRIPTOR_ADD_BASIC(edd, Exalt_Eth_Save, "up", state, EET_T_INT);
    EET_DATA_DESCRIPTOR_ADD_BASIC(edd, Exalt_Eth_Save, "driver", driver, EET_T_STRING);

    EET_DATA_DESCRIPTOR_ADD_SUB(edd, Exalt_Eth_Save, "connection", connection, edd_conn);

    return edd;
}
