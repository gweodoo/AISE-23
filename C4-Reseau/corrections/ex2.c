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
	hints.ai_flags = AI_PASSIVE;

	int ret = getaddrinfo(NULL, argv[1], &hints, &res);

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
			sock = -1;
			continue;
		}

		if( bind( sock, tmp->ai_addr, tmp->ai_addrlen) < 0)
		{
			perror("bind");
			sock = -1;
			continue;
		}

		if( listen( sock, 25) < 0)
		{
			perror("listen");
			sock = -1;
			continue;
		}


		break;
	}

	if(sock < 0)
	{
		fprintf(stderr, "Failed to connect to %s\n", argv[1]);
		return 1;
	}


	while(1)
	{
		printf("HERE WAIT\n");

		int client_fd = accept(sock, NULL, 0);

		if(client_fd < 0)
		{
			break;
		}

		printf("CLIENT ARRIVED\n");
	
		char buff[512];
		ssize_t rret;


		while( (rret = read(client_fd, buff, 512)) != 0)
		{
			int done = 0;
			if(rret < 0)
			{
				break;
			}

			int i;

			for(i=0 ;i < rret; i++)
			{
				if(buff[i] == 'A')
				{
					done = 1;
				}
			}

			safe_write(client_fd, buff, rret);

			if(done)
			{
				break;
			}
		}

		close(client_fd);

	}


	close(sock);

	return 0;
}
