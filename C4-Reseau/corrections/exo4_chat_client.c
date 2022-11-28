#include <sys/types.h>
#include <sys/socket.h>
#include <netdb.h>
#include <arpa/inet.h>
#include <stdio.h>
#include <string.h>
#include <unistd.h>
#include <errno.h>
#include <pthread.h>

#include "exo4_chat.h"

ssize_t safe_write(int fd, void *buff,  size_t size)
{
	size_t written = 0;
	while( (size - written) != 0 )
	{
		errno = 0;
		ssize_t ret = write(fd, buff + written, size-written);

		if( ret < 0 )
		{
			if(errno == EINTR)
			{
				continue;
			}

			perror("write");
			return ret;
		}

		written += ret;
	}

	return 0;
}

ssize_t safe_read(int fd, void * buff, size_t size)
{
	int off = 0;

	while(size)
	{
		int ret = read(fd, buff + off, size);

		if( ret < 0)
		{
			return -1;
		}

		if( ret == 0 )
		{
			return -1;
		}

		off+=ret;
		size -= ret;
	}

	return 0;
}


void * chat_read_loop(void * data)
{
	int fd = *(int *)data;
	struct chat_command cmd;

	/* Read from sock */
	while ( 1 )
	{
		ssize_t ret = safe_read( fd, &cmd, sizeof(struct chat_command));

		if(ret < 0)
		{
			break;
		}

		if(cmd.cmd == SPEAK)
		{
			printf("%s > %s", cmd.login, cmd.text);
			fflush(stdout);
		}
	}
}

int parse_chat_cmd(struct chat_command * cmd, char * login, char * text)
{
	text[strlen(text)] = '\0';

	if(strstr(text, "/mp"))
	{
		/* MP */
	} else if(strstr(text, "/kick"))
	{
		if(strlen(text) < 7)
			return -1;

		char * k = strstr(text, "/kick");
		char * login = k + 6;

		chat_command_kick(cmd, login);

	} else {
		chat_command_speak(cmd, login, text);
	}

	return 0;
}



int main( int argc, char **argv )
{
	/* Liste des alternatives
	 * pour se connecter */
	struct addrinfo *res = NULL;
	/* Configuration souhaitée */
	struct addrinfo hints;
	/* Toujours mettre hints a 0 */
	memset( &hints, 0, sizeof( hints ) );
	/* On veut IPV4 ou IPV6, respectivement AF_INET ou AF_INET6 */
	hints.ai_family = AF_UNSPEC;
	/* On veut faire du TCP */
	hints.ai_socktype = SOCK_STREAM;


	if( argc < 4 )
	{
		fprintf(stderr, "USAGE %s [HOST] [PORT] [LOGIN]\n");
		return 1;
	}

	/* On lance la résolution DNS
	 * argv[1] : nom de l'hôte
	 * argv[2] : port (exemple 80 pour http)
	 * */
	int ret = getaddrinfo( argv[1], argv[2],
	                       &hints,
	                       &res );

	if ( ret < 0 )
	{
		herror( "getaddrinfo" );
		return 1;
	}

	struct addrinfo *tmp;

	int sock = -1;

	int connected = 0;

	/* On parcours les alternative recues */
	for ( tmp = res; tmp != NULL; tmp = tmp->ai_next )
	{
		/* On crée un socket en suivant la configuration
		 * renvoyéee par getaddrinfo */
		sock = socket( tmp->ai_family, tmp->ai_socktype, tmp->ai_protocol );

		if ( sock < 0 )
		{
			perror( "sock" );
			continue;
		}

		/* On tente de le connecter à l'adresse renvoyée par
		 * getaddrinfo et configurée par argv[1] et argv[2] */
		int ret = connect( sock, tmp->ai_addr, tmp->ai_addrlen );

		if ( ret < 0 )
		{
			close( sock );
			perror( "connect" );
			continue;
		}

		connected = 1;
		break;
	}

	if ( !connected )
	{
		fprintf( stderr, "Failed to connect to %s:%s\n", argv[1], argv[2] );
		return 1;
	}


	/* Si nous sommes là le socket est connecté

	/* Send HELLO COMMAND */
	struct chat_command cmd;
	chat_command_hello(&cmd, argv[3]);
	safe_write(sock, &cmd, sizeof(struct chat_command));

	pthread_t th;
	pthread_create(&th, NULL, chat_read_loop, &sock);


	/* avec succes on peut lire et ecrire dedans */
	/* Use the socket */
	char buff[512];

	/* Read from sock */
	while ( 1 )
	{
		printf("# ");
		fflush(stdout);
		char * ret = fgets(buff, 512, stdin);

		if(!ret)
		{
			break;
		}

		if(parse_chat_cmd(&cmd, argv[3], buff) < 0)
		{
			continue;
		}

		safe_write(sock, &cmd, sizeof(struct chat_command));
		sleep(1);
	}

	/* On se déconnecte du serveur */
	close( sock );
	return 0;
}