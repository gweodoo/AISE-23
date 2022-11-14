#include <stdio.h>
#include <sys/types.h>
#include <unistd.h>
#include <sys/wait.h>
#include <unistd.h>
#include <sys/stat.h>
#include <fcntl.h>


int main(int argc, char ** argv)
{

	int pipefd[2];

	/* R[0] & W[1] */
	if( pipe(pipefd) < 0 )
	{
		perror("pipe");
		return 1;
	}



	pid_t pid = fork();


	if(pid == 0)
	{
		if( dup2(pipefd[0], STDIN_FILENO) < 0 )
		{
			perror("dup2");
			return 1;
		}

		close(pipefd[0]);
		close(pipefd[1]);

		char * new_argv[] = { "tac", "-s", " ", NULL};

		if( execvp( new_argv[0], new_argv ) < 0 )
		{
			perror("execvp");
			return 1;
		}

	}
	else
	{


		pid_t ff = fork();

		if( ff == 0 )
		{

			if( dup2(pipefd[1], STDOUT_FILENO) < 0 )
			{
				perror("dup2");
				return 1;
			}


			close(pipefd[0]);
			close(pipefd[1]);


			printf("Coucou tout le monde\n");
		}
		else
		{	
			close(pipefd[1]);
			close(pipefd[0]);
			wait(NULL);
			wait(NULL);
		}
	}


	return 0;
}
