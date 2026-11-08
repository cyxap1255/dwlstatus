#include <stdio.h>
#include <sys/utsname.h>

void output_kernel(void) {
	struct utsname kernel;
	uname(&kernel);
	printf("kern: %s %s", kernel.sysname, kernel.release);
}
