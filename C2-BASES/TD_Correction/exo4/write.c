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

	for(i = 0; i < NUM_PART; i++)
	{
		part[i].x = i;
		part[i].y = i + 1;
		part[i].z = 0;
	}


	FILE * out = fopen("./data", "w");

	fwrite(part, sizeof(struct particle), NUM_PART, out);

	return 0;
}
