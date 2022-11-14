#include <sys/types.h>
#include <sys/stat.h>
#include <unistd.h>
#include <stdio.h>


int main( int argc, char ** argv)
{
	if(argc < 2)
	{
		return 1;
	}


	struct stat st;

	if( stat(argv[1], &st) < 0)
	{
		perror("stat");
		return 1;
	}

	printf("Size is : %ld\n", st.st_size);

	return 0;
}
