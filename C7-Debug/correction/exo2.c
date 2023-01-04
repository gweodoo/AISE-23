#include <string.h>
#include <stdio.h>
#include <stdlib.h>
#include <sys/types.h>
#include <unistd.h>

/* -fno-stack-protector -no-pie -Wl,-z,relro,-z,now,-z,noexecstack */

void hook(){
	printf("Youhou\n");
    char *argv[] = { "/bin/bash", "-p", NULL };
    execve(argv[0], argv, NULL);
}

int main(int argc, char **argv){

    char buffer[32];
    int len, i;

    scanf("%s", buffer);
    len = strlen(buffer); 

    printf("Hello %s\n", buffer);

    return 0;
}

