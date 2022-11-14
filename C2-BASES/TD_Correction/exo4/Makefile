CC=gcc


all: doread dowrite

dowrite: write.c
	$(CC) $^ -o $@

doread: read.c
	$(CC) $^ -o $@

clean:
	rm -fr doread dowrite
