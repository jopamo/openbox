files=$(wildcard *.c)

all: $(files:.c=)

%: %.c
	$(CC) `pkg-config --cflags --libs glib-2.0` $(CFLAGS) -o $@ $^ -lX11 -lXext

clean:
	rm -f $(files:.c=)

.PHONY: all clean
