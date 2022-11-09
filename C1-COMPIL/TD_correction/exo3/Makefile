CC=gcc -Wall
B=./build/

all: build/mandel build/lib/libinterp.so

etape_debut:
	mkdir -p $(B)/lib

build/lib/libinterp.so: code_interp.c etape_debut
	$(CC) -fPIC -shared -o $@ $< -ldl

build/mandel: mandel.c etape_debut
	$(CC) -o $@ $< -lm

clean:
	rm -rf $(B)/*

.PHONY: etape_debut clean

