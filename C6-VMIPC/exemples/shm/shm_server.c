#include <sys/ipc.h>
#include <sys/shm.h>

#include <unistd.h>
#include <stdio.h>

int main(int argc, char **argv)
{
	int shm = shmget(19999, 2 * sizeof(int), IPC_CREAT | IPC_EXCL | 0600 );

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
	val[0] = 1;
	val[1] = 0;

	while(val[0])
	{
		sleep(1);
		val[1]++;
	}

	/* Unmap segment */
	shmdt(val);
	/* Server marks the segment for deletion */
	shmctl(shm, IPC_RMID, NULL);
	
	return 0;
}
