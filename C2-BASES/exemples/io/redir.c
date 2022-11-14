#include <stdio.h>
#include <sys/types.h>
#include <unistd.h>
#include <sys/wait.h>
#include <unistd.h>
#include <sys/stat.h>
#include <fcntl.h>


int main(int argc, char ** argv)
{
	pid_t pid = fork();


	if(pid == 0)
	{
		/* REPLACE STDOUT with fd for ./lol */
		int fd = open("./lol" ,  O_WRONLY | O_CREAT, 0700);

		if( fd < 0)
		{
			perror("open");
			return 1;
		}

		if( dup2(fd, STDOUT_FILENO) < 0 )
		{
			perror("dup2");
			return 1;
		}

		close(fd);

		char * new_argv[] = { "ls", "-l", NULL};

		if( execvp( new_argv[0], new_argv ) < 0 )
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
