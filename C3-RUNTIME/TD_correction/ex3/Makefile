CC=gcc -Wall

all: owner.exe user.exe

%.exe: %.c
	$(CC) $< -o $@

clean: 
	rm -f owner.exe user.exe

.PHONY: clean

