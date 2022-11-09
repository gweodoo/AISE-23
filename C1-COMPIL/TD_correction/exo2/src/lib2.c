#include <stdio.h>
#include <string.h>
#include <stdlib.h>


size_t nbchars = 0;

void mylib_puts(char* s)
{
	size_t len = strlen(s);
	size_t i;
	nbchars += len;
	char* sdup = strdup(s);

	for (i = 0; i < len; ++i) {
		sdup[len-1-i] = s[i];
	}
	puts(sdup);
	free(sdup);
}

size_t mylib_nbchars()
{
	return nbchars;
}

