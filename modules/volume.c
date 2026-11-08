#include <stdio.h>
#include <string.h>

void output_volume(void) {
	FILE *fp;
	char buff5[256];
	if ((fp = popen("wpctl get-volume @DEFAULT_AUDIO_SINK@", "r"))) {
		if (fgets(buff5, sizeof(buff5), fp)) {
			buff5[strcspn(buff5, "\n")] = 0;
			printf("vol: %s%%", buff5);
		}
		pclose(fp);
	}
}
