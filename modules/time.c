#include <stdio.h>

void output_time(void) {
	long hour = 0, minute = 0, sec = 0;
	FILE *f = fopen("/proc/driver/rtc", "r");
	if (f) {
		char line2[256];
		while (fgets(line2, sizeof(line2), f)) {
			sscanf(line2, "rtc_time	: %ld:%ld:%ld", &hour, &minute, &sec);
		}

		fclose(f);
	}

	printf("%02ld:%02ld:%02ld", hour, minute, sec);
}
