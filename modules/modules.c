#include <stdio.h>
#include <unistd.h>
#include <sys/utsname.h>
#include <stdlib.h>
#include <string.h>

void output_ram(void) {
	long total_ram = 0, free_ram = 0;
	FILE *f = fopen("/proc/meminfo", "r");
	if (f) {
		char line[256];
		while (fgets(line, sizeof(line), f)) {
			sscanf(line, "MemTotal: %ld kB", &total_ram);
			sscanf(line, "MemAvailable: %ld kB", &free_ram);
		}
		fclose(f);
	}

	total_ram = total_ram / 1024;
	free_ram = free_ram / 1024;

	float real_total_ram = total_ram / 1024.0f;
	float real_free_ram = free_ram / 1024.0f;
	float real_used_ram = real_total_ram - real_free_ram;

	printf("RAM: %.2fGi/%.2fGi | ", real_used_ram, real_total_ram);
}

void output_kernel(void) {
	struct utsname kernel;
	uname(&kernel);
	printf("KERN: %s %s", kernel.sysname, kernel.release);
}

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

void output_battery(void) {
	long bat = 0;
	FILE *f = fopen("/sys/class/power_supply/BAT0/capacity", "r");
	if (f == NULL) {
		f = fopen("/sys/class/power_supply/BAT1/capacity", "r");
		if (f) {
			char line3[256];
			while (fgets(line3, sizeof(line3), f)) {
				sscanf(line3, "%ld", &bat);
			}
		}
		fclose(f);

	printf("BAT: %ld%% | ", bat);

	} else {
		f = fopen("/sys/class/power_supply/BAT0/capacity", "r");
		char line4[256];
		while (fgets(line4, sizeof(line4), f)) {
			sscanf(line4, "%ld", &bat);
		}
	fclose(f);

	printf("BAT: %ld%% | ", bat);

	}
}

void output_cpu(void) {
	char buff[256];
	const char *cpu_command = "LC_ALL=C top -bn1 | grep \"Cpu(s)\" | awk '{print 100 - $8 \"%\"}'";
	FILE *fp = popen(cpu_command, "r");
	if (fp) {
		while (fgets(buff, sizeof(buff), fp)) {
			buff[strcspn(buff, "\r\n")] = 0;
			printf("CPU: %s | ", buff);
		}
	pclose(fp);	
	}
}

void output_network(void) {
	char buff2[256];
	FILE *fp;

	if ((fp = popen("nmcli -g GENERAL.CONNECTION device show wlp0s20f3", "r"))) {
		if (fgets(buff2, sizeof(buff2), fp)) {
			buff2[strcspn(buff2, "\n")] = 0;
			printf("NET: %s | ", buff2);
		}
		pclose(fp);
	}

	if ((fp = popen("wpa_cli -i wlp0s20f3 status 2>/dev/null | grep '^ssid=' | cut -d= -f2", "r"))) {
		if (fgets(buff2, sizeof(buff2), fp)) {
			buff2[strcspn(buff2, "\n")] = 0;
			printf("NET: %s | ", buff2);
		}
		pclose(fp);
	}

	if ((fp = popen("iwctl station wlp0s20f3 show 2>/dev/null | grep 'Connected network' | awk '{print $3}'", "r"))) {
		if (fgets(buff2, sizeof(buff2), fp)) {
			buff2[strcspn(buff2, "\n")] = 0;
			printf("NET: %s | ", buff2);
		}
		pclose(fp);
	}
}


void output_volume(void) {
	FILE *fp;
	char buff3[256];
	if ((fp = popen("wpctl get-volume @DEFAULT_AUDIO_SINK@", "r"))) {
		if (fgets(buff3, sizeof(buff3), fp)) {
			buff3[strcspn(buff3, "\n")] = 0;
			printf("VOL: %s%% | ", buff3);
		}
	}
}
