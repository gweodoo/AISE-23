#include <fcntl.h>           /* For O_* constants */
#include <sys/stat.h>        /* For mode constants */
#include <mqueue.h>
#include <stdio.h>
#include <unistd.h>
#include <sys/wait.h>

#include <sys/time.h>

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
	struct mq_attr attr;

	attr.mq_maxmsg = 5;
	attr.mq_msgsize = sizeof(struct msg_t);
	attr.mq_flags = 0;
	attr.mq_curmsgs = 0;


	mqd_t file1 = mq_open("/pinga", O_RDWR | O_CREAT, 0600, &attr);

	if( file1 < 0 )
	{
		perror("mq_open");
		return 1;
	}

	mqd_t file2 = mq_open("/pingb", O_RDWR | O_CREAT, 0600, &attr);

	if( file2 < 0 )
	{
		perror("mq_open");
		return 1;
	}

	int i;

	struct msg_t m;
	m.type = 1;

	int prio = 1;

	int pid = fork();

	if( pid == 0 )
	{
		int stop = 0;

		while(!stop)
		{
			mq_receive(file1, ( char *)&m, sizeof(struct msg_t), &prio);
			/* Notify end */
			if( m.data[0] == 0 )
				stop = 1;
			mq_send(file2, ( char *)&m, sizeof(struct msg_t), 1);
		}

	}
	else
	{
		double total_time = 0.0;

		for( i = 1 ; i <= NUM_MSG ; i++)
		{
			m.data[0] = i;

			double start = get_time();
			int ret = mq_send(file1, ( char *)&m, sizeof(struct msg_t), 1);

			if( ret < 0 )
			{
				perror("msgsend");
				return 1;
			}

			double end = get_time();
			total_time += end - start;

			mq_receive(file2, ( char *)&m, sizeof(struct msg_t), &prio);
		}	

		m.data[0] = 0;
		mq_send(file1, (char *)&m, sizeof(struct msg_t), 1);

		wait( NULL );

		fprintf(stderr, "Pingpong takes %g usec Bandwidth is %g MB/s",
				total_time/NUM_MSG*1e6, (double)(SIZE*NUM_MSG*sizeof(int))/(total_time*1024.0*1024.0));

		mq_close(file1);
		mq_close(file2);
		
		mq_unlink("/pinga");
		mq_unlink("/pingb");
	}

	return 0;
}
