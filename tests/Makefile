CC = cc
CFLAGS = -Wall -g -I/usr/include/glib-2.0 -I/usr/lib/glib-2.0/include
LDFLAGS = -lX11 -lXext
PKG_CONFIG = pkg-config

SRC = $(wildcard *.c)
OBJ = $(SRC:.c=.o)
BIN = $(SRC:.c=)

all: $(BIN)

%.o: %.c
	$(CC) $(CFLAGS) `$(PKG_CONFIG) --cflags glib-2.0` -c $< -o $@

%: %.o
	$(CC) $(CFLAGS) $^ `$(PKG_CONFIG) --libs glib-2.0` $(LDFLAGS) -o $@

clean:
	rm -f $(OBJ) $(BIN)

.PHONY: all clean
