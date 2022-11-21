#include <mystr.h>
#include <stdio.h>


int main(int argc, char *argv[])
{
	string* a = str_create("Bonjour");
	string* b = str_create("les gens\n");

	str_append(a, b);
	str_print(a);

	string* s = str_slice(a, 0, 2);
	str_print(s);

	if(str_compare(a, b) == 0)
	{
		printf("both strings are identical\n");
	}
	else
	{
		printf("strings differ\n");
	}

	str_copy(s, a);
	str_print(s);

	string *re = str_create("^[Bb]on.*$");
	if(str_regex(a, re))
		printf("The regex found a match !\n");
	else
		printf("Not found !\n");

	return 0;
}
