CC=gcc -Wall
B=./build/

all: build/lib1/libcustom.so build/lib2/libcustom.so build/bin/program

etape_debut:
	mkdir -p $(B)/lib1
	mkdir -p $(B)/lib2
	mkdir -p $(B)/bin

build/lib1/libcustom.so: src/lib1.c etape_debut include/header.h
	$(CC) -fPIC -shared -o $@ $<

build/lib2/libcustom.so: src/lib2.c etape_debut include/header.h
	$(CC) -fPIC -shared -o $@ $<

build/bin/program: main.c build/lib1/libcustom.so build/lib2/libcustom.so
	$(CC) -o $@ $< -Iinclude/ -L./build/lib1 -lcustom

clean:
	rm -rf $(B)/*

.PHONY: etape_debut clean

