CC=gcc
CFLAGS=-fPIC -shared
LDFLAGS=
all: libppm.so

libppm.so: ppm.o
	$(CC) $(CFLAGS) -o $@ $< $(LDFLAGS)

%.o: %.c
	$(CC) -o $@ $< $(CFLAGS)

clean:
	-rm -f *.o *.so

PHONY: clean
