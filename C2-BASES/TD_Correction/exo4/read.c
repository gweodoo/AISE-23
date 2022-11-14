#include "part.h"
#include <stdlib.h>
#include <stdio.h>



int main(int argc, char ** argv)
{
	struct particle * part = malloc(NUM_PART * sizeof(struct particle));

	if(!part)
	{
		perror("malloc");
		return 1;
	}

	int i;


	FILE * out = fopen("./data", "r");

	size_t ret = fread(part, sizeof(struct particle), NUM_PART, out);

	if(ret != NUM_PART)
	{
		printf("Truncated input\n");
		return 1;
	}

	for(i = 0; i < NUM_PART; i++)
	{
		if( part[i].x != i || part[i].y != (i + 1) || part[i].z != 0 )
		{
			printf("Erreur part %d\n", i);
		}
	}


	return 0;
}
