cc=gcc

all: libmystr.so program

libmystr.so: mystr.c mystr.h
	$(CC) -fPIC -shared -I. -o $@ $<

program: main.c mystr.h
	$(CC) $< -o $@ -I. -L. -lmystr -Wl,-rpath=.

clean:
	rm -f program libmystr.so


.PHONY: clean
