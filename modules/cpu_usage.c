#include <stdio.h>
#include <unistd.h>

void output_cpu(void) {
	unsigned long long user, nice, system, idle, iowait, irq, softirq, steal;
	unsigned long long user1, nice1, system1, idle1, iowait1, irq1, softirq1, steal1;
	FILE *fp;
	fp = fopen("/proc/stat", "r");
	int ret = fscanf(fp, "cpu %llu %llu %llu %llu %llu %llu %llu %llu", &user, &nice, &system, &idle, &iowait, &irq, &softirq, &steal);
	(void)ret;
	unsigned long long idle_total2 = idle + iowait;
	unsigned long long non_idle2 = user + nice + system + irq + softirq + steal;
	unsigned long long total1 = idle_total2 + non_idle2;
	fclose(fp);
	sleep(1);
	fp = fopen("/proc/stat", "r");
	int ret1 = fscanf(fp, "cpu %llu %llu %llu %llu %llu %llu %llu %llu", &user1, &nice1, &system1, &idle1, &iowait1, &irq1, &softirq1, &steal1);
	(void)ret1;
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
