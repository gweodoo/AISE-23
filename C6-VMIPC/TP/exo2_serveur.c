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
    struct mq_attr attr;

    attr.mq_maxmsg = 10;
    attr.mq_msgsize = sizeof(struct msg_t);
    attr.mq_flags = 0;
    attr.mq_curmsgs = 0;

    mqd_t mq1 = mq_open("/toto11", O_RDWR | O_CREAT, 0600, &attr);

	if( mq1 == (mqd_t)-1 )
	{
		perror("mq_opena");
		return 1;
	}


    mqd_t mq2 = mq_open("/toto22",  O_RDWR | O_CREAT, 0600, &attr);

	if( mq2 == (mqd_t)-1 )
	{
		perror("mq_openb");
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
        int ret = mq_send(mq1, (const char *)&m, sizeof(struct msg_t ), 1);

		if( ret < 0 )
		{
			perror("msgsend");
			return 1;
		}

		double end = get_time();
		total_time += end - start;

		mq_receive(mq2, (char *)&m, sizeof(struct msg_t), NULL);
	}	

	m.type = 2;
	m.data[0] = 0;
	mq_send(mq1, (const char *)&m, sizeof(struct msg_t ), 1);


	fprintf(stderr, "Pingpong takes %g usec Bandwidth is %g MB/s",
			total_time/NUM_MSG*1e6, (double)(SIZE*NUM_MSG*sizeof(int))/(total_time*1024.0*1024.0));


	mq_unlink("/toto1");
	mq_unlink("/toto2");


	return 0;
}
