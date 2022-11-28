#include <stdio.h>
#include <sys/types.h>
#include <sys/socket.h>
#include <netdb.h>
#include <string.h>
#include <unistd.h>

int safe_write(int fd, void * buff, size_t size)
{
	size_t written = 0;

	while(size - written)
	{
		ssize_t ret = write(fd, buff + written, (size - written));

		if( ret < 0)
		{
			perror("write");
			return ret;
		}
	
		written += ret;
	}

	return size;
}



int main(int argc, char ** argv)
{
	/* ARG */
	if( argc < 2)
	{
		printf("%s [ADDR]\n", argv[0]);
		return 1;
	}


	struct addrinfo * res = NULL;
	struct addrinfo hints;

	memset(&hints, 0, sizeof(struct addrinfo));

	hints.ai_family = AF_UNSPEC;
	hints.ai_socktype = SOCK_STREAM;


	int ret = getaddrinfo(argv[1], "80", &hints, &res);

	if(ret < 0 )
	{
		perror("getaddrinfo");
	}


	struct addrinfo * tmp;
	int sock = -1;

	for(tmp = res; tmp != NULL; tmp = tmp->ai_next)
	{
		sock = socket(tmp->ai_family, tmp->ai_socktype, tmp->ai_protocol);

		if( sock < 0)
		{
			perror("socket");
			continue;
		}

		if( connect( sock, tmp->ai_addr, tmp->ai_addrlen) < 0)
		{
			perror("connect");
			continue;
		}


		break;
	}

	if(sock < 0)
	{
		fprintf(stderr, "Failed to connect to %s\n", argv[1]);
		return 1;
	}



	char GET[] = "GET / HTTP/1.1\n\n\n";

	ssize_t rret = safe_write(sock, GET, strlen(GET));



	char buff[512];

	while( (rret = read(sock, buff, 512)) != 0)
	{
		if(rret < 0)
		{
			break;
		}

		write(STDOUT_FILENO, buff, rret);
	}



	close(sock);

	return 0;
}
