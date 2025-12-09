#include <stdio.h>
#include "str_reverse.h"

int main(){
	char str[] = "System Programming is the best";
	printf("Original : %s\n", str);
	str_reverse(str);
	printf("Reversed : %s\n", str);
	return 0;
}
