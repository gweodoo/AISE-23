#ifndef MYSTR_H_
#define MYSTR_H_

#include <stddef.h>
#include <stdbool.h>



typedef struct {
	char *data;
	size_t len;
} string;

string * str_create(const char * str);
void str_print(const string *s);
void str_destroy(string *s);
void str_copy(string *s1, const string *s2);
int str_compare(const string *s1, const string *s2);
void str_append(string* s1, const string* s2);
string* str_slice(const string *s1, size_t start, size_t end);
bool str_regex(const string* s1, const string* pattenrn);
#endif /* ifndef MYSTR_H_ */
