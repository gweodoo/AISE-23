#include <sys/types.h>
#include <sys/socket.h>
#include <netdb.h>
#include <string.h>
#include <stdio.h>
#include <unistd.h>
#include <sys/types.h>
#include <unistd.h>
#include <pthread.h>
#include <stdlib.h>

struct client_desc
{
    int client_fd;
    int i;
};


void * client_func(void *pcli)
{
    struct client_desc * cli = (struct client_desc *)pcli;

    char toto[128];

    snprintf(toto, 128, "COUCOU TOI %d\n", cli->i);

	int i;

	for(i = 0; i < 1024; i++)
    		write(cli->client_fd, toto, strlen(toto));
    close(cli->client_fd);

    free(cli);

    return NULL;
}



int main(int argc, char const *argv[])
{
    struct addrinfo hints;
    memset(&hints, 0, sizeof(struct addrinfo));

    hints.ai_family = AF_UNSPEC;
    hints.ai_socktype = SOCK_STREAM;
    hints.ai_flags = AI_PASSIVE;
    
    struct addrinfo * result = NULL;

    int ret = getaddrinfo(NULL, argv[1], &hints, &result);


    if(ret < 0)
    {
        herror("getaddrinfo");
        return 1;
    }

    int sock = 0;
    int server_ready = 0;

    struct addrinfo *tmp;
    for(tmp = result; tmp != NULL; tmp=tmp->ai_next)
    {
        sock = socket(tmp->ai_family, tmp->ai_socktype, tmp->ai_protocol);

        if(sock < 0)
        {
            perror("socket");
            continue;
        }

        if( bind(sock, tmp->ai_addr, tmp->ai_addrlen) < 0)
        {
            perror("bind");
            continue;
        }

        if( listen(sock, 5) < 0)
        {
            perror("listen");
            continue;
        }

        server_ready = 1;
        break;
    }

    if(server_ready == 0)
    {
        fprintf(stderr, "PAS SERVER READY :-'(");
        return 1;
    }


    int client_fd = -1;

    struct sockaddr client_addr;
    socklen_t client_addr_len;

    int i = 0;

    while(1)
    {
        client_fd = accept(sock, &client_addr, &client_addr_len);

        if(client_fd < 0)
        {
            perror("accept");
            break;
        }

        i++;

        struct client_desc * new_desc = malloc(sizeof(struct client_desc));

        new_desc->client_fd = client_fd;
        new_desc->i = i;

        pthread_t th;
        pthread_attr_t attr;
	pthread_attr_init(&attr);
	pthread_attr_setdetachstate(&attr, PTHREAD_CREATE_DETACHED);
	pthread_create(&th, &attr, client_func, (void *)new_desc);
	
    }

	close(sock);

    return 0;
}
