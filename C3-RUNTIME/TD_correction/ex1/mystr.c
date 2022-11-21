#include <mystr.h>
#include <string.h>
#include <stdio.h>
#include <stdlib.h>




string * str_create(const char * str){
	string *ret = malloc(sizeof(string));

	if(str == NULL)
	{
		ret->data = NULL;
		ret->len = 0;
	}

	ret->len = strlen(str);
	ret->data = malloc(ret->len);
	memcpy(ret->data, str, ret->len);

	return ret;
}
void str_destroy(string *s)
{
	if(s->data)
		free(s->data);
	free(s);
}
void str_print(const string *s)
{
	if(s->data == NULL) return;
	char* buf = malloc(s->len + 1);
	memcpy(buf, s->data, s->len);
	buf[s->len] = '\0';
	printf("%s", buf);
	free(buf);
}

void str_copy(string *s1, const string *s2)
{
	if(s2->len <= 0)
	{
		if(s1->data)
			free(s1->data);
		s1->data = NULL;
		s1->len = 0;
		return;
	}
	if(s1->data)
		free(s1->data);
	s1->data = malloc(s2->len);
	s1->len = s2->len;

	memcpy(s1->data, s2->data, s1->len);
}

int str_compare(const string *s1, const string *s2)
{
	if(s1->data == NULL || s2->data == NULL) return -1;

	size_t i, j;
	for(i=0, j=0; i < s1->len && j < s2->len; i++, j++)
	{
		if(s1->data[i] == s2->data[i])
			continue;
		
		else if (s1->data[i] < s2->data[i])
			return -1;
		else return 1;
	}

	return 0;

}
void str_append(string* s1, const string* s2)
{
	 if(s2->data == NULL)
	 	 return;

	 char* buf = realloc(s1->data, s1->len + s2->len);
	 memcpy(buf + s1->len ,s2->data, s2->len);

	 s1->data = buf;
	 s1->len += s2->len;
}
string* str_slice(const string *s1, size_t start, size_t end)
{
	if(s1->data == NULL || start < 0 || end > s1->len )
		return str_create(NULL);

	string *ret = malloc(sizeof(string));
	ret->data = malloc(end-start);
	ret->len = end-start;

	memcpy(ret->data, s1->data + start, ret->len);
	return ret;
}

#include <regex.h>

bool str_regex(const string* s1, const string* pattern)
{
	int status;
	regex_t r;
	if(regcomp(&r, pattern->data, REG_EXTENDED|REG_NOSUB) != 0)
		return 0;

	status = regexec(&r, s1->data, 0, NULL, 0);
	regfree(&r);
	if(status == 0)
		return 1;
	else
		return 0;
}

