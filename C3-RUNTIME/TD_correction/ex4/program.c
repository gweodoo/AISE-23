#include <elf.h>
#include <stdio.h>
#include <stdlib.h>
#include <sys/mman.h>
#include <sys/types.h>
#include <unistd.h>
#include <fcntl.h>
#include <sys/stat.h>


int main(int argc, char *argv[])
{
	int fd = open("./program", O_RDONLY, 0600);
	if (fd < 0)
	{
		perror("open");
		abort();
	}

	struct stat stat;
	if(fstat(fd, &stat) < 0)
	{
		perror("fstat");
		abort();
	}

	void * start = mmap(NULL, stat.st_size, PROT_READ, MAP_SHARED | MAP_FILE, fd, 0);
	if(start == MAP_FAILED)
	{
		perror("mmap");
		abort();
	}

	Elf64_Ehdr* hdr = (Elf64_Ehdr*) start;
	Elf64_Sym* symtab;
	size_t count = 0;
	int i;
	char*strtab;

	printf("'%x' '%c' '%c' '%c'\n", *((char*)start), *((char*)start+1), *((char*)start+2), *((char*)start+3));

	Elf64_Shdr *sections = (Elf64_Shdr*)((char*)start + hdr->e_shoff);
	for (i = 0; i < hdr->e_shnum; ++i) {
		if(sections[i].sh_type == SHT_SYMTAB)
		{
			symtab = (Elf64_Sym*)((char*)start+sections[i].sh_offset); 
			count = sections[i].sh_size / sections[i].sh_entsize;
			strtab = (char*)start + sections[sections[i].sh_link].sh_offset;
		}
	}

	for (i = 0; i < count; ++i) {
		printf("%d: %s\n", i, (char*)strtab + symtab[i].st_name);
	}

	return 0;
}
