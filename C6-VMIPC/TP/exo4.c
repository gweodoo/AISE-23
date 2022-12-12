#include <stdio.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <fcntl.h>
#include <sys/mman.h>
#include <unistd.h>
#include <stdlib.h>

int compar(const void *pa, const void *pb)
{
	int *a = (int*)pa;
	int *b = (int*)pb;

	return *a - *b;
}


int main(int argc, char ** argv)
{
	int fd = open("./int.dat", O_CREAT | O_RDWR, 0600);

	if( fd < 0 )
	{
		perror("open");
		return 1;
	}

	size_t total_size = 1024*1024*5;
	size_t num_int = total_size / sizeof(int);

	if( ftruncate(fd, total_size) < 0 )
	{
		perror("ftruncate");
		return 1;
	}


	int * array = (int *) mmap(NULL, total_size, PROT_READ | PROT_WRITE, MAP_SHARED, fd, 0);

	if( array == MAP_FAILED)
	{
		perror("mmap");
		return 1;
	}


	int i;
	for(i = 0 ; i < num_int; i++)
	{
		array[i] = random();
	}


	qsort(array, num_int, sizeof(int), compar);


	if(munmap(array, total_size) < 0)
	{
		perror("munmap");
		return 1;
	}

	close(fd);

	return 0;
}
