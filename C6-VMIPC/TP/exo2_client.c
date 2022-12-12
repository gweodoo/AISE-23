#include <stdio.h>
#include <sys/types.h>

#include <unistd.h>
#include <sys/wait.h>

#include <sys/time.h>


#include <fcntl.h>           /* For O_* constants */
#include <sys/stat.h>        /* For mode constants */
#include <mqueue.h>

double get_time()
{
	struct timeval val;
	gettimeofday(&val, NULL);
	return (double)val.tv_sec + 1e-6 * val.tv_usec;
}


#define SIZE 16

struct msg_t
{
	long type;
	int data[SIZE];
};

#define NUM_MSG 65536

int main( int argc, char ** argv )
{
    mqd_t mq1 = mq_open("/toto11",  O_RDWR);

	if( mq1 == (mqd_t)-1 )
	{
		perror("mq_open");
		return 1;
	}


    mqd_t mq2 = mq_open("/toto22",  O_RDWR);

	if( mq2 == (mqd_t)-1 )
	{
		perror("mq_open");
		return 1;
	}


	int i;

	struct msg_t m;
	m.type = 1;

	int stop = 0;

	while(!stop)
	{

		mq_receive(mq1, (char *)&m, sizeof(struct msg_t), NULL);
		/* Notify end */
		if( m.data[0] == 0 )
			stop = 1;
		m.type = 1;
		mq_send(mq2, (const char *)&m, sizeof(struct msg_t ), 1);
	}



	return 0;
}
