#include <stdlib.h>
#include <stdio.h>
int main() {
	int var, check = 0x04030201;
	char buf[40];

	fgets(buf,45,stdin);

	if(check != 0x04030201)
	{
		printf("Not bad\n");
	}
	printf("%p\n", check);
	if (check == 0xdeadbeef) {
		printf("You Win !\n");
		system("/bin/sh");
	}
	return 0;
}

