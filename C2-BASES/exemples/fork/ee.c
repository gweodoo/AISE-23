#include <stdio.h>
#include <sys/types.h>
#include <unistd.h>
#include <sys/wait.h>

int main(int argc, char ** argv)
{
	if(argc == 1)
	{
		return 1;
	}

	pid_t c = fork();

	if(c < 0)
	{
		perror("fork");
		return 1;
	}


	if(c==0)
	{
		if( execvp(argv[1], &argv[1]) < 0)
		{
			perror("execvp");
			return 1;
		}
	}
	else
	{
		wait(NULL);
	}

	return 0;
}
