#include <stdio.h>

void output_date(void) {
	long year = 0, month = 0, month_day = 0;
	FILE *f = fopen("/proc/driver/rtc", "r");
	if (f) {
		char line3[256];
		while (fgets(line3, sizeof(line3), f)) {
			sscanf(line3, "rtc_date	: %ld-%ld-%ld", &year, &month, &month_day);
		}

		fclose(f);
	}

	printf("%02ld:%02ld:%02ld", month_day, month, year);
}
