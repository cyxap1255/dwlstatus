#ifndef CONFIG_H
#define CONFIG_H

struct block {
	void (*func)(void) ;
	const char *space;
};

static const unsigned int interval = 1; /* refresh time */
const char *net_int = "wlp0s20f3"; /* network interface */

static const struct block blocks[] = { 
	/*module*/   	/*space between module*/
	{ output_cpu,					" | " },
	{ output_ram, 					" | " },
	{ output_time,					""    },
};

#endif 
