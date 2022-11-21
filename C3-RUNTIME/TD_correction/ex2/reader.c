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
	int fd = open("./file.txt", O_RDONLY | O_CREAT, 0600);
	if(fd < 0)
	{
		perror("open");
		abort();
	}

	void* projection = mmap(NULL, 64, PROT_READ, MAP_FILE | MAP_SHARED, fd, 0);
	if(projection == MAP_FAILED)
	{
		perror("mmap");
		abort();
	}
	close(fd);

	pid_t pid = -1;

	memcpy(&pid, projection, sizeof(pid_t));
	printf("Got PID %d\n", (int) pid);
	kill(pid, SIGUSR1);
	munmap(projection, 64);
	return 0;
}
