#define	_GNU_SOURCE 1
#include <dlfcn.h>
#include <stdio.h>


#include <stdlib.h>

#include <unistd.h>

#include <stddef.h>

size_t total_allocated = 0;
size_t total_calls = 0;
void*(*mallocptr)(size_t) = NULL;
void *malloc(size_t size)
{
	//printf("size requested: %lu\n", size);
	total_allocated += size;
	total_calls ++;
	// addr = ....appeler malloc()

	if(!mallocptr)
	{
		write(1, "Hello", 5);
		mallocptr = dlsym(RTLD_NEXT, "malloc");
		if(mallocptr == NULL)
		{
			abort();
		}
	}
	void* addr = mallocptr(size);
	
	//printf("addr: %p\n", addr);
	return addr;
}
__attribute__((destructor))
void fini()
{
	write(1, "Bye", 3);
	printf("Malloc called %lu times\n", total_calls);
	printf("Total allocated: %lu\n", total_allocated);
}

