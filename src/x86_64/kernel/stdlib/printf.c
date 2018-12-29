
#include <stdio.h>

int printf(char *fmt, ...) {

	va_list valist;
	va_start(valist, fmt);

	vprintf(fmt,valist);

	va_end(valist);

	return 0;
}

int vprintf(char * s, va_list valist) {
	char buff[128];
	char *buffer = buff;


	for (; *s != '\0'; s++) {
		if (*s == '\n') {
			kernel_putc('\n');
		} else if (*s != '%') {
			kernel_putc(*s);
		} else {
			s++;
			switch (*s) {
				case '\0':
					break; //Protection
				case 'd': //Signed decimal integer
				case 'i':
					itoa(va_arg(valist, int32_t), buffer, 10);
					kernel_puts(buffer);
					break;
				case 'u':
					uitoa(va_arg(valist, uint32_t), buffer, 10);
					kernel_puts(buffer);
					break;
				case 's':
					kernel_puts(va_arg(valist, char *));
					break;
				case 'c':
					kernel_putc(va_arg(valist, uint32_t));
					break;
				case 'o': //Octal integer
					itoa(va_arg(valist, int32_t), buffer, 8);
					kernel_puts(buffer);
					break;
				case 'b': //Binary integer
					itoa(va_arg(valist, int32_t), buffer, 2);
					kernel_puts(buffer);
					break;
				case 'x': //Hexadecimal integer
					itoa(va_arg(valist, int32_t), buffer, 16);
					kernel_puts(buffer);
					break;
				case 'p':
					itoa((uint32_t)va_arg(valist, void *), buffer, 16);
					kernel_puts(buffer);
					break;
				case 'f':
					dtoa(va_arg(valist, double), buffer, -1);
					kernel_puts(buffer);
					break;
				case '%': //%
					kernel_puts("%%");
					break;
				default: //Anything else
					return 1;
			}
		}
	}

	va_end(valist);

	return 0;
}


int sprintf(char * res, char *fmt, ...) {

	va_list valist;
	va_start(valist, fmt);

	int length = vsprintf(res, fmt,valist);

	va_end(valist);

	return length;
}


//prints format and arguments to string. returns length of this string. if string to print to is NULL no printing will be
//done and only the length is returned so a second print can print on a buffer that is guaranteed to be the right size
//TODO test this! seems to randomly produce pagefaults
int vsprintf(char * res, char *s, va_list valist){
	char buff[128];
	char *buffer = buff;

	//start with 1 for null terminator
	uint32_t length = 1;

	//empty string
	*res = '\0';

	for (; *s != '\0'; s++) {
		if (*s == '\n') {
			length += 1;
			if(res != NULL){
				strcatc(res,'\n');
			}
		} else if (*s != '%') {
			length += 1;
			if(res != NULL){
				strcatc(res,*s);
			}
		} else {
			s++;
			switch (*s) {
				case '\0':
					break; //Protection
				case 'd': //Signed decimal integer
				case 'i':
					itoa(va_arg(valist, int32_t), buffer, 10);
					length += strlen(buffer);
					if(res != NULL){
						strcat(res,buffer);
					}
					break;
				case 'u':
					uitoa(va_arg(valist, uint32_t), buffer, 10);
					length += strlen(buffer);
					if(res != NULL){
						strcat(res,buffer);
					}
					break;
				case 's':
					buffer = va_arg(valist, char *);
					length += strlen(buffer);
					if(res != NULL){
						strcat(res,buffer);
					}
					break;
				case 'c':
					length += 1;
					if(res != NULL){
						strcatc(res,(char)va_arg(valist, uint32_t));
					}
					break;					
				case 'o': //Octal integer
					itoa(va_arg(valist, int32_t), buffer, 8);
					length += strlen(buffer);
					if(res != NULL){
						strcat(res,buffer);
					}
					break;
				case 'b': //Binary integer
					itoa(va_arg(valist, int32_t), buffer, 2);
					length += strlen(buffer);
					if(res != NULL){
						strcat(res,buffer);
					}
					break;
				case 'x': //Hexadecimal integer
					itoa(va_arg(valist, int32_t), buffer, 16);
					length += strlen(buffer);
					if(res != NULL){
						strcat(res,buffer);
					}
					break;
				case 'p':
					itoa((uint32_t)va_arg(valist, void *), buffer, 16);
					length += strlen(buffer);
					if(res != NULL){
						strcat(res,buffer);
					}
					break;
				case 'f':
					dtoa(va_arg(valist, double), buffer, -1);
					length += strlen(buffer);
					if(res != NULL){
						strcat(res,buffer);
					}
					break;
				case '%': //%
					length += 1;
					if(res != NULL){
						strcat(res,"%");
					}
					break;
				default: //Anything else
					return length;
			}
		}
	}

	va_end(valist);

	return length;
}

//not yet inplemented, TODO
// int snprintf(char * res, char *fmt, ...) {

// 	va_list valist;
// 	va_start(valist, fmt);

// 	int length = vsprintf(res, fmt,valist);

// 	va_end(valist);

// 	return length;
// }


// //same as vsprintf but will guarantee stringsize of n
// int vsnprintf(char * res, char *s, va_list valist){
// 	char buff[128];
// 	char *buffer = buff;

// 	//start with 1 for null terminator
// 	uint32_t length = 1;

// 	//empty string
// 	*res = '\0';

// 	for (; *s != '\0'; s++) {
// 		if (*s == '\n') {
// 			length += 1;
// 			if(res != NULL){
// 				strcatc(res,'\n');
// 			}
// 		} else if (*s != '%') {
// 			length += 1;
// 			if(res != NULL){
// 				strcatc(res,*s);
// 			}
// 		} else {
// 			s++;
// 			switch (*s) {
// 				case '\0':
// 					break; //Protection
// 				case 'd': //Signed decimal integer
// 				case 'i':
// 					itoa(va_arg(valist, int), buffer, 10);
// 					length += strlen(buffer);
// 					if(res != NULL){
// 						strcat(res,buffer);
// 					}
// 					break;
// 				case 's':
// 					buffer = va_arg(valist, char *);
// 					length += strlen(buffer);
// 					if(res != NULL){
// 						strcat(res,buffer);
// 					}
// 					break;
// 				case 'o': //Octal integer
// 					itoa(va_arg(valist, int), buffer, 8);
// 					length += strlen(buffer);
// 					if(res != NULL){
// 						strcat(res,buffer);
// 					}
// 					break;
// 				case 'b': //Binary integer
// 					itoa(va_arg(valist, int), buffer, 2);
// 					length += strlen(buffer);
// 					if(res != NULL){
// 						strcat(res,buffer);
// 					}
// 					break;
// 				case 'x': //Hexadecimal integer
// 					itoa(va_arg(valist, int), buffer, 16);
// 					length += strlen(buffer);
// 					if(res != NULL){
// 						strcat(res,buffer);
// 					}
// 					break;
// 				case 'p':
// 					itoa((uint32_t)va_arg(valist, void *), buffer, 16);
// 					length += strlen(buffer);
// 					if(res != NULL){
// 						strcat(res,buffer);
// 					}
// 					break;
// 				case 'f':
// 					dtoa(va_arg(valist, double), buffer, -1);
// 					length += strlen(buffer);
// 					if(res != NULL){
// 						strcat(res,buffer);
// 					}
// 					break;
// 				case '%': //%
// 					length += 1;
// 					if(res != NULL){
// 						strcat(res,"%");
// 					}
// 					break;
// 				default: //Anything else
// 					return length;
// 			}
// 		}
// 	}

// 	va_end(valist);

// 	return length;
// }