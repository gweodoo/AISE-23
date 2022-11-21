#include <stdio.h>
#include <unistd.h>
#include <sys/mman.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <stdlib.h>
#include <string.h>
#include <signal.h>
#include <fcntl.h>

void func(int signal)
{
	printf("Caught %d\n", signal);
}

int main(int argc, char *argv[])
{
	pid_t pid = getpid();
	int fd = open("./file.txt", O_RDWR | O_CREAT, 0600);
	if(fd < 0)
	{
		perror("open");
		abort();
	}

	if(ftruncate(fd, 64) < 0)
	{
		perror("ftruncate");
		abort();
	}

	void* projection = mmap(NULL, 64, PROT_WRITE, MAP_FILE | MAP_SHARED, fd, 0);
	if(projection == MAP_FAILED)
	{
		perror("mmap");
		abort();
	}
	close(fd);

	memcpy(projection, &pid, sizeof(pid_t));

	if(signal(SIGUSR1, func) == SIG_ERR)
	{
		perror("signal");
		abort();
	}
	
	printf("Now, pause ! (PID = %d)\n", (int)pid);
	pause();

	munmap(projection, 64);
	return 0;
}
