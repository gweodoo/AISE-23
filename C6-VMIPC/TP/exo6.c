#include <unistd.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <sys/wait.h>


int main(int argc, char ** argv)
{

	char buff[1024];

	char * params[512] = {0};


	while( fgets(buff, 1024, stdin) != NULL)
	{
		printf("%s", getenv("PS1"));

		buff[strlen(buff) - 1] = '\0';

		memset(params, 0, sizeof(params));


		int cnt = 0;
		char * l = buff;
		char * p = NULL;

		while( (p = strsep(&l, " ") ) != NULL)
		{
			params[cnt] = p;
			cnt++;
		}

		fflush(stdout);



		pid_t f = fork();

		if(!f)
		{
			execvp(params[0], params);
		}
		else
		{
			wait(NULL);
		}
	}

	return 0;
}
