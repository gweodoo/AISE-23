#include <stdio.h>
#include <signal.h>
#include <sys/types.h>
#include <unistd.h>


int cnt = 0;

void handler(int signum)
{
	printf("Signal %d\n", ++cnt);
}


int main(int argc, char ** argv)
{

	pid_t c = fork();


	if( c == 0)
	{

		signal(SIGUSR1, handler);

		while(1){}

	}
	else
	{
		while(1)
		{
			printf("Send sig\n");
			kill(c, SIGUSR1);
			sleep(3);
		}


	}


	return 0;
}
