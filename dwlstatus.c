#include <stdio.h>
#include <unistd.h>
#include "modules/modules.h"

int main(void) {
	while (1) {
		FILE *fo = freopen("/tmp/dwl-bar", "w", stdout);
		if (fo != NULL) {

			output_network(); /*----------				  	                  */
			output_cpu();     /*         |           	          	      */
			output_ram();     /*         |------ set your modules here 	*/ 
			output_time();	  /*         |	                          	*/
			printf("\n");     /*----------                             	*/           

			fflush(stdout);
		}
		sleep(1);
	}
	return 0;
}
