#include <stdio.h>

void output_battery(void) {
	long bat = 0;
	FILE *f = fopen("/sys/class/power_supply/BAT0/capacity", "r");
	if (f == NULL) {
		f = fopen("/sys/class/power_supply/BAT1/capacity", "r");
		char line3[256];
		while (fgets(line3, sizeof(line3), f)) {
			sscanf(line3, "%ld", &bat);
		}
		fclose(f);
		printf("bat: %ld%%", bat);

	} else {
		char line4[256];
		while (fgets(line4, sizeof(line4), f)) {
			sscanf(line4, "%ld", &bat);
		}
	fclose(f);

	printf("bat: %ld%%", bat);

	}
}
