#include <stdio.h>
#include <stdlib.h>

#include "ppm.h"

int ppm_image_create(struct ppm_image *i, int w, int h)
{
	i->width = w;
	i->height = h;

	i->table = malloc(sizeof(struct ppm_pixel) * w * h);
	if(! i->table)
	{
		perror("malloc");
		return 1;
	}
	return 0;
}

int ppm_image_delete(struct ppm_image *i)
{
	if(!i)
	{
		return 1;
	}
	free(i->table);
	i->width = 0;
	i->height = 0;
	i->table = NULL;
}

int ppm_image_save(struct ppm_image *i, char *filepath)
{
	FILE * f = fopen(filepath, "w");
	if (!f)
	{
		perror("fopen");
		return 1;
	}

	fprintf(f, "P6\n");
	fprintf(f, "%d\n", i->width);
	fprintf(f, "%d\n", i->height);
	fprintf(f, "%d\n", 255);

	fwrite(i->table, sizeof(struct ppm_pixel), i->width * i->height, f);

	fclose(f);
	return 0;
}
