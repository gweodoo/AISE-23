#include <stdio.h>
#include <sys/types.h>
#include <sys/ipc.h>
#include <sys/msg.h>
#include <unistd.h>
#include <sys/wait.h>

#include <sys/time.h>

double get_time()
{
	struct timeval val;
	gettimeofday(&val, NULL);
	return (double)val.tv_sec + 1e-6 * val.tv_usec;
}


#define SIZE 32

struct msg_t
{
	long type;
	int data[SIZE];
};

#define NUM_MSG 65536

int main( int argc, char ** argv )
{
	int file = msgget(1337, 0);

	if( file < 0 )
	{
		perror("msgget");
		return 1;
	}


	int i;

	struct msg_t m;
	m.type = 1;

	int stop = 0;

	while(!stop)
	{
		msgrcv(file, &m, SIZE*sizeof(int), 2, 0);
		/* Notify end */
		if( m.data[0] == 0 )
			stop = 1;
		m.type = 1;
		msgsnd(file, &m, SIZE*sizeof(int), 0);
	}



	return 0;
}
