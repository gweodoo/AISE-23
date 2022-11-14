#include <stdio.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <fcntl.h>
#include <unistd.h>


int main(int argc, char ** argv)
{
	if(argc < 2)
	{
		fprintf(stderr, "%s [FILE]\n", argv[0]);
		return 1;
	}

	int fd = open(argv[1], O_RDONLY);

	if(fd < 0)
	{
		perror("open");
		return 1;
	}
	

	
	ssize_t cnt = 0;

	char buff[1024];

	while(( cnt = read(fd, buff, 50) ))
	{


		if(cnt < 0)
		{
			perror("read");
			break;
		}

		write(STDOUT_FILENO, buff, cnt);

	}




	return 0;
}
