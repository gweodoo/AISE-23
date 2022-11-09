#include <stdio.h>
#include <string.h>

size_t nbchars = 0;

void mylib_puts(char* s)
{
	nbchars += strlen(s);
	puts(s);
}

size_t mylib_nbchars()
{
	return nbchars;
}

