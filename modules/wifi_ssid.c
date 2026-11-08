#include <stdio.h>
#include <string.h>
#include <unistd.h>
#include <sys/socket.h>
#include <sys/ioctl.h>
#include <net/if.h>
#include <arpa/inet.h>
#include <linux/wireless.h>

extern const char *net_int;

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
