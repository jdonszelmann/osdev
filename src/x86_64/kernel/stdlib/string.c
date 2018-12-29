
#include <stdio.h>

char * strcpy(char *dst, char *src){
	uint32_t i;
	for(i=0; src[i] != 0; i++){
		dst[i] = src[i];
	}
	dst[i] = 0;
	return dst;
}

char * strcat(char *dst, char *src){
	uint32_t i = 0;
	uint32_t j = 0;
	for (i = 0; dst[i] != 0; i++);
	for (j = 0; src[j] != 0; j++){
		dst[i+j] = src[j];
	}
	dst[i+j] = 0;
	return dst;
}

char * strcatc(char *dst, char src){
	uint32_t i = 0;
	for (i = 0; dst[i] != 0; i++);
	dst[i+0] = src;
	dst[i+1] = 0;
	return dst;
}

uint32_t strcmp(char *s1, char *s2){
	for (uint32_t i = 0; ; i++){
		if (s1[i] != s2[i]){
			return s1[i] < s2[i] ? -1 : 1;
		}

		if (s1[i] == '\0'){
			return 0;
		}
	}
}


uint32_t strncmp( char * s1, char * s2, size_t n ){
	while (n && *s1 && ( *s1 == *s2 )){
		++s1;
		++s2;
		--n;
	}
	if (n == 0){
		return 0;
	}else{
		return ( *(uint8_t *)s1 - *(uint8_t *)s2 );
	}
}

uint32_t strlen(char *str){
	for (uint32_t i = 0;; i++){
		if (str[i] == '\0'){
			return i;
		}
	}
	return 0;
}


bool starts_with(char *s1, char* s2){
	if(strncmp(s1, s2, strlen(s2)) == 0){
		return true;
	}
	return false;
}


char * strstr(char *in, char *str) {

	char c = *str++;
	if (!c){
		return (char *) in;
	}

	uint32_t len = strlen(str);
	do {
		char sc;

		do {
			sc = *in++;
			if (!sc)
				return (char *) 0;
		} while (sc != c);
	} while (strncmp(in, str, len) != 0);

	return (char *) (in - 1);
}

//see http://man7.org/linux/man-pages/man3/strsep.3.html
char *strsep(char **stringp, char *delim) {
	char *s;
	char *spanp;
	uint32_t c, sc;
	char *tok;
	if ((s = *stringp) == NULL){
		return (NULL);
	}
	for (tok = s;;) {
		c = *s++;
		spanp = delim;
		do {
			if ((sc = *spanp++) == c) {
				if (c == 0){
					s = NULL;
				}else{
					s[-1] = 0;
				}
				*stringp = s;
				return (tok);
			}
		} while (sc != 0);
	}
}

// char * strdup(char * src) {
// 	uint32_t len = strlen(src) + 1;
// 	char * dst = kmalloc(len);        
// 	memcpy(dst, src, len);    
// 	return dst;
// }

//USES MALLOC
// char * strtok(char* s, char* delm){
// 	static uint32_t currIndex = 0;
// 	if(!s || !delm || s[currIndex] == '\0')
// 	return NULL;
// 	char *W = (char *)malloc(sizeof(char)*100);
// 	uint32_t i = currIndex, k = 0, j = 0;

// 	while (s[i] != '\0'){
// 		j = 0;
// 		while (delm[j] != '\0'){
// 			if (s[i] != delm[j]){
// 				W[k] = s[i];
// 			}else{
// 				goto It;
// 			}
// 			j++;
// 		}
// 		//ptr++;
// 		i++;
// 		k++;
// 	}
// It:
// 	W[i] = 0;
// 	currIndex = i+1;
// 	//Iterator = ++ptr;
// 	return W;
// }


char * strtok_r(char *s, char *delim, char **lasts){
	char *spanp;
	uint32_t c, sc;
	char *tok;

	/* s may be NULL */
	/*netsnmp_assert(delim != NULL);*/
	/*netsnmp_assert(lasts != NULL);*/

	if (s == NULL && (s = *lasts) == NULL){
		return (NULL);
	}

	/*
	* Skip (span) leading delimiters (s += strspn(s, delim), sort of).
	*/
	cont:
	c = *s++;
	for (spanp = delim; (sc = *spanp++) != 0;) {
		if (c == sc){
			goto cont;
		}
	}

	if (c == 0) {/* no non-delimiter characters */
		*lasts = NULL;
		return (NULL);
	}
	tok = s - 1;

	/*
	* Scan token (scan for delimiters: s += strcspn(s, delim), sort of).
	* Note that delim must have one NUL; we stop if we see that, too.
	*/
	for (;;) {
		c = *s++;
		spanp = delim;
		do {
			if ((sc = *spanp++) == c) {
				if (c == 0){
					s = NULL;
				}else{
					s[-1] = 0;
				}
				*lasts = s;
				return (tok);
			}
		} while (sc != 0);
	}
}