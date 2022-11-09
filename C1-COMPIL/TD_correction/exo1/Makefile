CC=gcc -Wall
AR=ar
B=./build/

all: build/lib/libcustom.so build/lib/libcustom.a build/bin/program

etape_debut:
	mkdir -p $(B)/lib
	mkdir -p $(B)/bin

build/lib/libcustom.so: code_lib.c etape_debut header.h
	$(CC) -fPIC -shared -o $@ $<

build/lib/libcustom.a: code_lib.c etape_debut header.h
	$(CC) -c $< -o $(B)/code_lib.o
	$(AR) rcs -o $@ $(B)/code_lib.o

build/bin/program: main.c build/lib/libcustom.so
	$(CC) -o $@ $< -I. -L./build/lib -lcustom -Wl,-rpath=./build/lib
	#$(CC) -o $@_static -static $< -I. -L./build/lib -lcustom

clean:
	rm -rf $(B)/*

.PHONY: etape_debut etape_fin

