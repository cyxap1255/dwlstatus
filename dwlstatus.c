#include <stdio.h>
#include <unistd.h>
#include "modules/modules.h"
#include "config.def.h"

int main(void) {
	while (1) {
		FILE *fo = freopen("/tmp/dwl-bar", "w", stdout);
		if (fo != NULL) {
			for (size_t i = 0; i < sizeof(blocks) / sizeof(blocks[0]); i++) {
				blocks[i].func();
				printf("%s", blocks[i].space);
			}
			printf("\n");
			fflush(stdout);
		}
		sleep(interval);
	}
	return 0;
}
