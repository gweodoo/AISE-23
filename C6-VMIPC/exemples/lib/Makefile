CC=gcc

all: rpath.out

liba.so: a.c
	$(CC) -shared -fPIC -o $@ $^

rpath.out: liba.so t.c
	$(CC) t.c -o $@ -L. -la -Wl,-rpath=$(PWD)

clean:
	rm -f liba.so rpath.out
