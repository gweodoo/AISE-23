#include <stdio.h>
#include <unistd.h>


int main(int argc, char ** argv)
{
	if( argc == 1 )
	{
		return 1;
	}	
	
	int i;
	for(i = 1; i < argc; i++)
	{
		int rc = unlink(argv[i]);

		if(rc < 0 )
		{
			perror("rm");
		}
	}

	return 0;
}
