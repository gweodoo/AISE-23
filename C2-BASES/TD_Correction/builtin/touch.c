#include <stdio.h>
#include <time.h>
#include <unistd.h>
#include <stdlib.h>
#include <fcntl.h>

int main(int argc, char ** argv){


	if( argc == 2 )
	{
		int fd = open(argv[1], O_CREAT | O_RDWR, 0666);

		if( fd < 0 )
		{
			perror("open");
			return 1;
		}


		close(fd);
	}

	return 0;
}



