#include <stdio.h>
#include <unistd.h>
#include <sys/types.h>
#include <unistd.h>
#include <sys/wait.h>

int main(int argc, char **argv)
{
	pid_t p = fork();

	if(p == 0)
	{
		char * arg[] = {"ls", "-la", NULL};

		execvp(arg[0], arg);

	}
	else
	{
		wait(NULL);
		printf("AFTER WAIT");
	}


	return 0;
}
