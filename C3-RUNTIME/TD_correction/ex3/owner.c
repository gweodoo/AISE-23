#include <stdio.h>
#include <unistd.h>
#include <sys/mman.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <stdlib.h>
#include <string.h>
#include <signal.h>
#include <fcntl.h>

size_t square(int n)
{
	return n * n;
}
void dummy_square() {}

int main(int argc, char *argv[])
{
	int fd = open("libsquare.o", O_RDWR | O_CREAT, 0600);
	size_t size = (char*)dummy_square - (char*)square;
	if(fd < 0)
	{
		perror("open");
		abort();
	}

	if(ftruncate(fd, size) < 0)
	{
		perror("ftruncate");
		abort();
	}



	void* projection = mmap(NULL, size, PROT_WRITE, MAP_SHARED | MAP_FILE, fd, 0);
	if(projection == MAP_FAILED)
	{
		perror("mmap");
		abort();
	}

	memcpy(projection, (void*)square, size );

	munmap(projection, size);
	close(fd);
	return 0;
}
