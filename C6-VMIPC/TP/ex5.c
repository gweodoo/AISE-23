#include <sys/ipc.h>
#include <sys/shm.h>
#include <string.h>
#include <sys/wait.h>
#include <sys/time.h>

#include <unistd.h>
#include <stdio.h>
#include <sys/mman.h>
#include <sys/stat.h>        /* For  mode
				constants */
#include  <fcntl.h>            /* For O_*
				  constants */
#include <pthread.h>
#include <stdlib.h>

typedef enum{
	SENDER_RDY=1,
	RECV_RDY=2
}is_rdy_t;


double get_time()
{
	struct timeval val;
	gettimeofday(&val, NULL);
	return (double)val.tv_sec + 1e-6 * val.tv_usec;
}



struct shm_segment
{
	pthread_spinlock_t lock;
	is_rdy_t rdy;
	char payload[0];	
};

#define BUFF_SIZE 1024*1024*1024


#define SHM_SIZE 1024*1024
#define PL_SIZE ((1024*1024) - sizeof(struct shm_segment))
static inline void wait_for_state(struct shm_segment *base, is_rdy_t type)
{
	int ok = 0;

	while(!ok)
	{
		pthread_spin_lock(&base->lock);

		if(base->rdy == type)
		{
			ok = 1;
		}

		pthread_spin_unlock(&base->lock);

	}

}



static inline size_t send_at(struct shm_segment * base, void * src,
		size_t size)
{

	wait_for_state(base, RECV_RDY);

	pthread_spin_lock(&base->lock);

	size_t to_send = (size < PL_SIZE) ? size : PL_SIZE;

	memcpy(base->payload, src, to_send);
	base->rdy = SENDER_RDY;

	pthread_spin_unlock(&base->lock);


	return to_send;	
}



static inline size_t recv_at(struct shm_segment * base, void * dest,
		size_t size)
{


	size_t to_recv = (size < PL_SIZE) ? size : PL_SIZE;

	wait_for_state(base, SENDER_RDY);

	pthread_spin_lock(&base->lock);

	memcpy(dest, base->payload, to_recv);
	base->rdy = RECV_RDY;

	pthread_spin_unlock(&base->lock);

	return to_recv;	
}

int main(int argc, char **argv)
{
	int shm = shm_open("/toto", O_RDWR | O_CREAT | O_TRUNC, 0600);

	if( shm < 0)
	{
		perror("shm_open");
		return 1;
	}

	if( ftruncate(shm, SHM_SIZE) < 0)
	{
		perror("ftruncate");
		return 1;
	}


	int pid = fork();

	struct shm_segment *val = (struct shm_segment*) mmap(NULL, SHM_SIZE, PROT_READ | PROT_WRITE, MAP_SHARED, shm, 0);

	if( val == MAP_FAILED )
	{
		perror("mmap");
		return 1;
	}

	pthread_spin_init(&val->lock, PTHREAD_PROCESS_SHARED);


	unsigned char * hundredmeg = malloc(BUFF_SIZE*sizeof(unsigned char));

	if(hundredmeg < 0)
	{
		perror("malloc");
		return 1;
	}

	size_t iter;

	if( pid == 0 )
	{
		/* Set bad values */
		for(iter = 0 ; iter < (BUFF_SIZE); iter++)
		{
			hundredmeg[iter] = 1;
		}

		size_t total = BUFF_SIZE;
		size_t received = 0;

		while((total - received) != 0)
		{
			received += recv_at(val, hundredmeg + received, total - received);
		}

		for(iter = 0 ; iter < (BUFF_SIZE); iter++)
		{
			if( hundredmeg[iter] != (iter % 256) )
			{
				printf("Pas bon @ %ld (%d != %d)\n", iter, hundredmeg[iter], iter % 256);
			}
		}


	}
	else
	{
		/* Set values */

		for(iter = 0 ; iter < (BUFF_SIZE); iter++)
		{
			hundredmeg[iter] = iter%256;
		}

		val->rdy = RECV_RDY;

		size_t total = BUFF_SIZE;
		size_t sent = 0;


		double begin = get_time();
		while((total - sent) != 0)
		{
			sent += send_at(val, hundredmeg + sent, total - sent);
		}
		double end = get_time();

		wait(NULL);

		printf("Sent %f MB in %f seconds == %f MB/s\n", BUFF_SIZE/(1024.0*1024.0), end-begin, (BUFF_SIZE / (1024.0*1024.0)) / (end - begin) );

	}

	munmap(val, SHM_SIZE);


	return 0;
}
