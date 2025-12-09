#include <stddef.h>
#include "str_reverse.h"

char* str_reverse(char* str){
	if(str == NULL) return NULL;

	int left = 0;
	int right = 0;

	while(str[right] != '\0'){
	   right++;
	}
	right--;

	while(left < right){
	   char temp = str[left];
	   str[left] = str[right];
	   str[right] = temp;

	   left++;
	   right--;
	}
	return str;
}

