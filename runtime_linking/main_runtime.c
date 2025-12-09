#include <stdio.h>
#include <stdlib.h>
#include <dlfcn.h>

int main(){
	void *handle;
	char* (*str_reverse)(char*);
	char *error;

	handle = dlopen("./libstr_reverse.so", RTLD_LAZY);
	if(!handle){
	   fprintf(stderr, "%s\n", dlerror());
	   exit(1);
	}
	
	str_reverse = dlsym(handle, "str_reverse");
	if((error = dlerror()) != NULL){
	   fprintf(stderr, "%s\n", error);
	   exit(1);
	}
	
	char str[] = "Professor thank you for your hard work";
	printf("Orginal : %s\n", str);
	str_reverse(str);
	printf("Reversed : %s\n", str);
	
	if(dlclose(handle) < 0){
	   fprintf(stderr, "%s\n", dlerror());
	   exit(1);
	}
	return 0;
}
