
#ifndef STRING_H
#define STRING_H

#include <stdio.h>


uint32_t isdigit(char c);
uint32_t isnumber(char * c);
uint32_t isspace(char c);

bool startswith(char * s1, char * s2);
// bool endswith(char * s1, char * s2); not implemented yet
char * strcpy(char * dst, char * src);
char * strcat(char * dst, char * src);
char * strcatc(char * dst, char src);
uint32_t strcmp(char * s1, char * s2);
uint32_t strlen(char * str);
uint32_t strncmp(char * s1, char * s2, size_t n);

//USES MALLOC
// char * strdup(char * src);
char * strsep(char ** stringp, char * delim);
char * strstr(char * in, char * str);

void * memcpy(void * dest, void *src, size_t count);
void * memset(void * dest, uint32_t val, size_t count);
uint16_t * memsetw(uint16_t * dest, uint16_t val, size_t count);
//USES MALLOC
// char * strtok(char * s, char * delim);
char * strtok_r(char * s, char * delim, char ** lasts);

#endif