#include "stdio.h"
#include "print.h"
#include "input.h"

char read_key(){
	char c;
	x86_Read_Keypress(c);
	return c;
}

void clear_screen(){
	x86_clear_screen();
}

void putc(char c){
	x86_Write_Character(c,0);
}

void puts(const char* s){
	while(*s)
	{
		putc(*s);
		s++;
	}
}

void puts_f(const char far* s){
	while(*s)
	{
		putc(*s);
		s++;
	}
}
uint16_t strlen(const char* str){
	char* s = str;
	uint16_t count = 0;
	while(*s)
	{
		count++;
		s++;
	}
	printf("String length: %i%n", count);
	return count;
}

char* strspl(const char* str, const char* del){
	char* s = str;
	while(*s)
	{
		printf("char: %c%n", *s);
		if(*s == *del)
		{
			printf("Splitting string now%n");
			*s = 0;
			s++;
			return s;
		}
		s++;
	}
}
// if flag is 1 - no errors
// if flag is 0 - strings are not the same
bool strcmp(const char str1[], const char str2[]){
	uint16_t i = 0;
	bool flag = 1;
	if(strlen(str1) != strlen(str2)){
		flag = 0;
		return flag;
	}
	for(i = 0; i<=strlen(str1); i++){
		printf("char: %c char2: %c count: %i%n", str1[i], str2[i], i);
		if(i == strlen(str1)){
			flag = 1;
			break;
		}

		if(!(str1[i] == str2[i])){
			flag = 0;
			break;
		}
		i++;
	}
	return flag;
}

void _cdecl printf(const char* fmt, ...){
	int* argp = (int*) &fmt;
	int state = PRINTF_STATE_START;
	int length = PRINTF_LENGTH_START;
	int radix = 10;
	bool sign = false;

	argp++;
	while (*fmt){
	switch(state){
		case PRINTF_STATE_START:
			if (*fmt == '%'){
				state = PRINTF_STATE_LENGTH;
			}else{
				putc(*fmt);
			}

			break;
		case PRINTF_STATE_LENGTH:
			if (*fmt == 'h'){
				length = PRINTF_LENGTH_SHORT;
				state = PRINTF_STATE_SHORT;
			}else if(*fmt == 'l'){
				length = PRINTF_LENGTH_LONG;
				state = PRINTF_STATE_LONG;
			}else{
				goto PRINTF_STATE_SPEC_;
			}
			break;
		case PRINTF_STATE_SHORT:
			if (*fmt == 'h'){
				length = PRINTF_LENGTH_SHORT_SHORT;
				state = PRINTF_STATE_SPEC;
			}else{
				goto PRINTF_STATE_SPEC_;
			}
			break;
		case PRINTF_STATE_LONG:
			if (*fmt == 'l'){
						length = PRINTF_LENGTH_LONG_LONG;
						state = PRINTF_STATE_SPEC;
					}else{
						goto PRINTF_STATE_SPEC_;
					}
			break;

		case PRINTF_STATE_SPEC:
			PRINTF_STATE_SPEC_:
			switch(*fmt){
				case 'c':
					putc((char)*argp);
					argp++;
					break;
				case 's':
					if (length == PRINTF_LENGTH_LONG || length == PRINTF_LENGTH_LONG_LONG){
						puts_f(*(const char far**)argp);
						argp += 2;
					}else{
						puts(*(const char **)argp);
						argp++;
					}
					break;
				case '%':
					putc('%');
					break;
				case 'd':
				case 'i':
					radix = 10;
					sign = true;
					argp = printf_number(argp, length, sign, radix);
					break;
				case 'u':
					radix = 10;
					sign = false;
					argp = printf_number(argp, length, sign, radix);
					break;
				case 'X':
				case 'x':
				case 'p':
					radix = 16;
					sign = false;
					argp = printf_number(argp, length, sign, radix);
					break;
				case 'o':
					radix = 8;
					sign = false;
					argp = printf_number(argp, length, sign, radix);
					break;
				case 'n':
					putc(0x0d);
					putc(0x0a);
					break;
				default:
					break;
			}
		state = PRINTF_STATE_START;
		length = PRINTF_LENGTH_START;
		radix = 10;
		sign = false;
		break;
		}
		fmt++;
	}
}

const char possibleChars[] = "0123456789abcdef";

int* printf_number(int* argp, int length, bool sign, int radix){
	char buffer[32];
	unsigned long long number;
	int number_sign = 1;
	int pos = 0;

	switch(length){
		case PRINTF_LENGTH_SHORT_SHORT:
		case PRINTF_LENGTH_SHORT:
		case PRINTF_LENGTH_START:
			if(sign){
				int n = *argp;
				if(n < 0){
					n = -n;
					number_sign = -1;
				}
				number = (unsigned long long) n;
			}else{
				number = *(unsigned int*) argp;
			}
			argp++;
			break;
		case PRINTF_LENGTH_LONG:
			if(sign){
					long int n = *(long int*)argp;
					if(n < 0){
						n = -n;
						number_sign = -1;
					}
					number = (unsigned long long) n;
			}else{
				number = *(unsigned long int*) argp;
			}
			argp += 2;
			break;
		case PRINTF_LENGTH_LONG_LONG:
			if(sign){
				long long int n = *(long long int*)argp;
				if (n < 0){
					n = -n;
					number_sign = -1;
				}
				number = (unsigned long long) n;
			}else{
				number = *(unsigned long long int*)argp;
			}
			argp += 4;
			break;
	}

	do{
		uint32_t rem;
		x86_div64_32(number, radix, &number, &rem);
		buffer[pos++] = possibleChars[rem];
	}while(number > 0);

	if (sign && number_sign < 0){
		buffer[pos++] = '-';
	}

	while (--pos >= 0){
		putc(buffer[pos]);
	}

	return argp;
}
