#include <stdio.h>
#include <unistd.h>
#include "modules/modules.h"

int main(void) {
	sleep(1);
	while (1) {
		FILE *fo = freopen("/tmp/dwl-bar", "w", stdout);
		if (fo != NULL) {

			output_network_nmcli();   /* configuration */
			printf(" | ");
			output_cpu();
			printf(" | ");
			output_ram();
			printf(" | ");
			output_time();	
			printf("\n");                                  	         

			fflush(stdout);
		}
		sleep(1);
	}
	return 0;
}
