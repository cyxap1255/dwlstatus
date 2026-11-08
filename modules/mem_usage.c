#include <stdio.h>

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
