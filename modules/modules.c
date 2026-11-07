#include <stdio.h>
#include <unistd.h>
#include <sys/utsname.h>
#include <stdlib.h>
#include <string.h>
#include <sys/socket.h>
#include <sys/ioctl.h>
#include <net/if.h>
#include <arpa/inet.h>
#include <linux/wireless.h>

extern const char *net_int;

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

	printf("mem: %.2fGi/%.2fGi", real_used_ram, real_total_ram);
}

void output_kernel(void) {
	struct utsname kernel;
	uname(&kernel);
	printf("kern: %s %s", kernel.sysname, kernel.release);
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

void output_cpu(void) {
	unsigned long long user, nice, system, idle, iowait, irq, softirq, steal;
	unsigned long long user1, nice1, system1, idle1, iowait1, irq1, softirq1, steal1;
	FILE *fp;
	fp = fopen("/proc/stat", "r");
	fscanf(fp, "cpu %llu %llu %llu %llu %llu %llu %llu %llu", &user, &nice, &system, &idle, &iowait, &irq, &softirq, &steal);
	unsigned long long idle_total2 = idle + iowait;
	unsigned long long non_idle2 = user + nice + system + irq + softirq + steal;
	unsigned long long total1 = idle_total2 + non_idle2;
	fclose(fp);
	sleep(1);
	fp = fopen("/proc/stat", "r");
	fscanf(fp, "cpu %llu %llu %llu %llu %llu %llu %llu %llu", &user1, &nice1, &system1, &idle1, &iowait1, &irq1, &softirq1, &steal1);
	unsigned long long idle_total3 = idle1 + iowait1;
	unsigned long long non_idle3 = user1 + nice1 + system1 + irq1 + softirq1 + steal1;
	unsigned long long total2 = idle_total3 + non_idle3;
	unsigned long long delta_idle = idle_total3 - idle_total2;
	unsigned long long delta_total = total2 - total1;
	fclose(fp);
	double w = 0.0;
	if (delta_total > 0) {
		w = (double)(delta_total - delta_idle) / delta_total * 100.0;
	}
	printf("cpu: %.1f%%", w);
}

void output_wifi(void) {
	struct iwreq inquiry;
	char ssid[IW_ESSID_MAX_SIZE + 1] = {0};
	int sock;
	char interface[256];
	char wifi_state[32] = "down";
	snprintf(interface, sizeof(interface), "/sys/class/net/%s/operstate", net_int);
	FILE *fp;
	fp = fopen(interface, "r");
	if (fp) {
		if (fscanf(fp, "%31s", wifi_state) == 1) {
			wifi_state[strcspn(wifi_state, "\n")] = 0;
		}
		fclose(fp);
	}
	if (strcmp(wifi_state, "up") != 0) {
		printf("net: n/a");
		return;
	} else {
		sock = socket(AF_INET, SOCK_DGRAM, 0);
		if (sock != -1) {
			memset(&inquiry, 0, sizeof(inquiry));
			strncpy(inquiry.ifr_name, net_int, IFNAMSIZ - 1);
			inquiry.u.essid.pointer = ssid;
			inquiry.u.essid.length = sizeof(ssid);
			inquiry.u.essid.flags = 0;
			if (ioctl(sock, SIOCGIWESSID, &inquiry) != -1 && strlen(ssid) > 0) {
				printf("net: %s", ssid);
			}
	close(sock);	
		}
	}
}

void output_ethernet(void) {
	struct ifreq inquiry2;
	int sock2;
	char interface2[256];
	char ethernet_state[32] = "down";
	snprintf(interface2, sizeof(interface2), "/sys/class/net/%s/operstate", net_int);
	FILE *fp;
	fp = fopen(interface2, "r");
	if (fp) {
		if (fscanf(fp, "%31s", ethernet_state) == 1) {
			ethernet_state[strcspn(ethernet_state, "\n")] = 0;
		}
		fclose(fp);
	}
	if (strcmp(ethernet_state, "up") != 0) {
		printf("net: n/a");
		return;
	} else {
		sock2 = socket(AF_INET, SOCK_DGRAM, 0);
		memset(&inquiry2, 0, sizeof(inquiry2));
		strncpy(inquiry2.ifr_name, net_int, IFNAMSIZ - 1);
		if (ioctl(sock2, SIOCGIFADDR, &inquiry2) != -1) {
			struct sockaddr_in *ip_addr = (struct sockaddr_in *)&inquiry2.ifr_addr;
			printf("net: %s", inet_ntoa(ip_addr->sin_addr));
		}
	}
	close(sock2);
}
	
void output_volume(void) {
	FILE *fp;
	char buff5[256];
	if ((fp = popen("wpctl get-volume @DEFAULT_AUDIO_SINK@", "r"))) {
		if (fgets(buff5, sizeof(buff5), fp)) {
			buff5[strcspn(buff5, "\n")] = 0;
			printf("vol: %s%%", buff5);
		}
		fclose(fp);
	}
}

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
