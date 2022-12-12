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
	int file = msgget(1337, IPC_CREAT | 0600);

	if( file < 0 )
	{
		perror("msgget");
		return 1;
	}


	int i;

	struct msg_t m;
	m.type = 1;

	double total_time = 0.0;

	for( i = 1 ; i <= NUM_MSG ; i++)
	{
		m.type = 2;
		m.data[0] = i;

		double start = get_time();
		int ret = msgsnd(file, &m, SIZE*sizeof(int), 0);

		if( ret < 0 )
		{
			perror("msgsend");
			return 1;
		}

		double end = get_time();
		total_time += end - start;

		msgrcv(file, &m, SIZE*sizeof(int), 1, 0);
	}	

	m.type = 2;
	m.data[0] = 0;
	msgsnd(file, &m, SIZE*sizeof(int), 0);


	fprintf(stderr, "Pingpong takes %g usec Bandwidth is %g MB/s",
			total_time/NUM_MSG*1e6, (double)(SIZE*NUM_MSG*sizeof(int))/(total_time*1024.0*1024.0));


	msgctl(file, IPC_RMID, NULL);

	return 0;
}
