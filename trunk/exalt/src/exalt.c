
#include "exalt.h"
#include <Ecore_Data.h>

char* exalt_default_interface = NULL;
char* exalt_default_network = NULL;
exalt_dbus_conn* exalt_conn = NULL;

int main(int argc,char**argv)
{
	main_window* win;
 	int opt_i = 0;
	int opt_w = 0;

	if (!etk_init(argc, argv))
        {
            print_error(__FILE__,__func__,"Can not init ETK");
            return 1;
        }

        exalt_dbus_init();
        if(! (exalt_conn = exalt_dbus_connect()))
        {
            print_error( __FILE__,__func__,"Can not connect to DBUS");
            return -1;
        }

        setlocale( LC_ALL, "" );
      	bindtextdomain( "exalt", PACKAGE_LOCALE_DIR );
     	textdomain( "exalt" );

	argc--;
	argv++;
	while(argc)
	{
		if(*argv[0] == '-')
		{
 	 	 	if(strcmp(*argv, "-i")==0)
			{
				argc--;
				if(argc)
				{
					exalt_default_interface = *(++argv);
					opt_i=1;
				}
			}
			else if(strcmp(*argv,"-w")==0)
			{
			 	argc--;
				if(argc)
				{
					exalt_default_network = *(++argv);
					opt_w = 1;
				}
			}
		}
		if(argc) argc--;
 	 	argv++;
	}

        win = mainwindow_create();

	etk_main();

        exalt_dbus_wireless_scan_stop(exalt_conn, win->wireless_panel->interface);
        mainWindow_free(&win);

        if(exalt_conn)
        exalt_dbus_free(&exalt_conn);

        exalt_dbus_shutdown();

        etk_shutdown();
        return 1;
}



