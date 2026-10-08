CC = gcc
CFLAGS = -Wall -Wextra -O3
TARGET = dwlstatus
PREFIX = /usr/local/bin

SRCS = dwlstatus.c modules/modules.c
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
