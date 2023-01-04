#ifndef PPM_H_
#define PPM_H_ 

struct color
{
	int r;
	int g;
	int b;
};

struct ppm_pixel
{
	unsigned char r;
	unsigned char g;
	unsigned char b;
};

struct ppm_image
{
	size_t width;
	size_t height;
	struct ppm_pixel * table;
};

static inline void ppm_pixel_setcolor(struct ppm_pixel* s, int r, int g, int b)
{
	s->r = r;
	s->g = g;
	s->b = b;
}

int ppm_image_create(struct ppm_image *i, int w, int h);
int ppm_image_delete(struct ppm_image *i);
int ppm_image_save(struct ppm_image *i, char *filepath);


static inline void ppm_image_setpixel(struct ppm_image *i, int x, int y, unsigned char r, unsigned char g, unsigned char b)
{
	struct ppm_pixel *elt = i->table + i->width * y + x;
	ppm_pixel_setcolor(elt, r, g, b);
}

static inline struct color getcolor(int value, int max)
{
	double ratio = (double)value / (double)max;
	struct color c = {0, 0, 0};

	if (ratio < 0.25)
	{
		c.r = (ratio*4) * 255;
		c.g = 0;
		c.b = 255;
	}
	else if (ratio < 0.5)
	{
		c.r = 255;
		c.g = 255;
		c.b = (ratio-0.32)*4 * 255;
	}
	else if (ratio < 0.75)
	{
		c.r = 0;
		c.g = 255;
		c.b = (ratio - 0.56)*4*255;
	}
	else
	{
		c.r = 255;
		c.g = 255 - (ratio-0.70)*4*455;
		c.b = 255;
	}
}


#endif /* ifndef PPM_H_ */
