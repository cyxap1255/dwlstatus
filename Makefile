CC = gcc
CFLAGS = -Wall -Wextra -O3
TARGET = dwlstatus
PREFIX = /usr/local/bin

SRCS = dwlstatus.c \
       modules/battery.c \
       modules/date.c \
       modules/kernel.c \
       modules/time.c \
       modules/wifi_ssid.c \
       modules/cpu_usage.c \
       modules/ethernet_ssid.c \
       modules/mem_usage.c \
       modules/volume.c

OBJS = $(SRCS:.c=.o)

all: $(TARGET)

$(TARGET): $(OBJS)
	$(CC) $(OBJS) -o $(TARGET)

%.o: %.c
	$(CC) $(CFLAGS) -c $< -o $@

install: $(TARGET)
	mkdir -p $(DESTDIR)$(PREFIX)
	cp -f $(TARGET) $(DESTDIR)$(PREFIX)
	chmod 755 $(DESTDIR)$(PREFIX)/$(TARGET)

uninstall:
	rm -f $(DESTDIR)$(PREFIX)/$(TARGET)

clean:
	rm -f $(OBJS) dwlstatus.o modules/modules.o $(TARGET)

.PHONY: all install uninstall clean
