#include <header.h>
#include <sys/types.h>
#include <stdio.h>
int main(void)
{
	mylib_puts("Bonjour");
	mylib_puts("Bientôt l'heure de manger.");

	size_t count = mylib_nbchars();
	printf("Nb chars = %lu\n", count);
	return 0;
}

