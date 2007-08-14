
#include "exalt.h"
#include <Ecore_Data.h>

char* exalt_default_interface = NULL;
char* exalt_default_network = NULL;

int main(int argc,char**argv)
{
	main_window* win;
 	int opt_i = 0;
	int opt_w = 0;

	if (!etk_init(&argc, &argv))
        {
            fprintf(stderr, "Unable to init ETK.\n");
            return 1;
        }


	setlocale( LC_ALL, "" );
      	bindtextdomain( "exalt", PACKAGE_LOCALE_DIR );
     	textdomain( "exalt" );

	exalt_eth_init();
        //eth_printf();
	win = mainwindow_create();
        exalt_main();

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

	etk_main();

	return 1;
}
