#include <sys/types.h>
#include <sys/stat.h>
#include <fcntl.h>
#include <stdio.h>
#include <unistd.h>
#include <string.h>

int main(int argc, char ** argv)
{
	umask(0777);


	int fd = open("./testNONE", O_WRONLY | O_CREAT, 0777);

	if( fd < 0 )
	{
		perror("open");
		return 1;
	}


	char buff[] = "CA MARCHE ??? LOLILOL\n";

	write(fd, buff, strlen(buff));


	close(fd);

	return 0;
}
