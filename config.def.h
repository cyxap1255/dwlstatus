#ifndef CONFIG_H
#define CONFIG_H

struct block {
	void (*func)(void) ;
	const char *space;
};

static const unsigned int interval = 1; /* refresh time */
const char *net_int = "wlp0s20f3"; /* network interface */

/* accessible modules:
   - output_ram
   - output_kernel
   - output_time
   - output_battery
   - output_cpu
   - output_ethernet
   - output_wifi
   - output_volume
   - output_date */

static const struct block blocks[] = { 
	/*module*/   	/*space and character between module*/
	{ output_wifi,                                  " | " },
	{ output_cpu,									" | " },
	{ output_ram, 									" | " },
	{ output_time,									""    },
};

#endif 
