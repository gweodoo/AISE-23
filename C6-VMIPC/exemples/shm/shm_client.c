#include <sys/ipc.h>
#include <sys/shm.h>

#include <unistd.h>
#include <stdio.h>

int main(int argc, char **argv)
{
	int shm = shmget(19999, 2 * sizeof(int), 0 );

	if( shm < 0)
	{
		perror("shmget");
		return 1;
	}

	int *val = (int*) shmat(shm, NULL, 0);

	if( !val )
	{
		perror("shmat");
		return 1;
	}

	/* valeur de départ */
	int last_val = -1;
	while(1)
	{
		if( val[1] != last_val ){
			printf("Val is %d max is 60\n", val[1]);
			last_val = val[1];

			/* Stop condition */
			if( 60 <= val[1] )
			{
				val[0] = 0;
				break;
			}
		}
		else
		{
			usleep(100);
		}

	}

	/* Unmap segment */
	shmdt(val);

	return 0;
}
