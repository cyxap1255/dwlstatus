#include <stdio.h>
#include <string.h>
#include <unistd.h>
#include <sys/socket.h>
#include <sys/ioctl.h>
#include <net/if.h>
#include <arpa/inet.h>
#include <linux/wireless.h>

extern const char *net_int;

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
