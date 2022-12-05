#include <stdio.h>
#include <unistd.h>
#include <sys/mman.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <stdlib.h>
#include <string.h>
#include <signal.h>
#include <fcntl.h>


int main(int argc, char *argv[])
{
	int fd = open("libsquare.o", O_RDONLY | O_CREAT, 0600);
	if(fd < 0)
	{
		perror("open");
		abort();
	}

	struct stat stat;
	if(fstat(fd, &stat) < 0)
	{
		perror("fstat");
		abort();
	}

	void* projection = mmap(NULL, stat.st_size, PROT_READ | PROT_EXEC, MAP_SHARED | MAP_FILE, fd, 0);
	if(projection == MAP_FAILED)
	{
		perror("mmap");
		abort();
	}


	size_t (*func)(int);
	func = ( size_t(*)(int) )projection;
	
	printf("square of %d is %lu\n", 10, func(10));

	close(fd);
	return 0;
}
