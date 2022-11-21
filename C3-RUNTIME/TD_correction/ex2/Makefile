CC=gcc

all: reader.exe writer.exe

%.exe: %.c
	$(CC) $< -o $@

clean: 
	rm -f reader.exe writer.exe file.txt

.PHONY: clean

