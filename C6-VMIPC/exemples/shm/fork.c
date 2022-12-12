#include <sys/ipc.h>
#include <sys/shm.h>
#include <sys/wait.h>
#include <unistd.h>
#include <stdio.h>

int main(int argc, char **argv)
{
	int shm = shmget(IPC_PRIVATE, 2 * sizeof(int), IPC_CREAT | 0600 );

	if( shm < 0)
	{
		perror("shmget");
		return 1;
	}

	int pid = fork();

	int *val = (int*) shmat(shm, NULL, 0);

	if( !val )
	{
		perror("shmat");
		return 1;
	}


	if( pid == 0 )
	{


		/* valeur de départ */
		int last_val = -1;
		while(1)
		{
			if( val[1] != last_val ){
				printf("Val is %d max is 5\n", val[1]);
				last_val = val[1];

				/* Stop condition */
				if( 5 <= val[1] )
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

	}
	else
	{

		/* valeur de départ */
		val[0] = 1;
		val[1] = 0;

		while(val[0])
		{
			sleep(1);
			val[1]++;
		}

		/* Unmap segment */
		shmdt(val);

		wait(NULL);
		/* Server marks the segment for deletion */
		shmctl(shm, IPC_RMID, NULL);
	}


	return 0;
}
