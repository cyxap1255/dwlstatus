#include <stdio.h>
#include <time.h>

void output_date(void) {
	long year = 0, month = 0, month_day = 0;
	char wday[64];
	FILE *f = fopen("/proc/driver/rtc", "r");
	if (f) {
		char line3[256];
		while (fgets(line3, sizeof(line3), f)) {
			sscanf(line3, "rtc_date	: %ld-%ld-%ld", &year, &month, &month_day);
		}

		fclose(f);
	}
	struct tm timeinfo = {0};
	timeinfo.tm_year = year - 1900;
	timeinfo.tm_mon = month - 1;
	timeinfo.tm_mday = month_day;
	mktime(&timeinfo);
	strftime(wday, sizeof(wday), "%a", &timeinfo);
	printf("%s, %02ld-%02ld-%02ld", wday, month_day, month, year);
}
