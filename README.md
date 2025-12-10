# 📘 String Reverse Converter
시스템 프로그래밍(CSE3209) 프로젝트로, C 언어 기반 문자열 뒤집기(string reverse) 함수를 제작하고 **Static Linking, Dynamic Linking, Runtime Linking** 각각을 실습한 내용을 정리한 문서입니다.

문자열을 입력하면 **메모리 내부에서 제자리(in-place)** 로 뒤집은 뒤 반환하는 간단한 라이브러리를 제작하는 것이 목표입니다.

---

## 🖥️ 개발환경
- **OS:** Ubuntu 20.04 LTS
- **Compiler:** GCC 9.4.0

---

## 📁 리포지토리 구조
```bash
├── static_linking
│   ├── main.c
│   ├── str_reverse.c
│   ├── str_reverse.h
│   ├── str_reverse.o
│   ├── libreverse.a
│   └── exe_static
├── dynamic_linking
│   ├── main.c
│   ├── str_reverse.c
│   ├── str_reverse.h
│   ├── str_reverse.o
│   ├── libreverse.so
│   └── exe_dynamic
├── runtime_linking
│   ├── main_runtime.c
│   ├── str_reverse.c
│   ├── str_reverse.h
│   ├── str_reverse.o
│   ├── libstr_reverse.so
│   └── exe_runtime
└── README.md
```

---

## 🔧 코드 설명

### str_reverse.h
다음의 헤더 파일은 str_reverse() 함수 프로토타입을 선언합니다.
```c
#ifndef STR_REVERSE_H
#define STR_REVERSE_H

char* str_reverse(char* str);

#endif
```

### str_reverse.c
다음의 c파일은 헤더 파일을 포함하고, 함수 정의를 구현합니다.
기존 문자열을 메모리 안에서 제자리로 뒤집은 뒤 뒤집은 문자열을 반환하는 함수를 제작하였습니다.
```c
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
```

### main.c
라이브러리를 사용하는 메인 프로그램입니다.   
기존 문자열과 str_reverse() 함수를 사용하여 뒤집은 문자열을 출력합니다.
```c
#include <stdio.h>
#include "str_reverse.h"

int main(){
	char str[] = "System Programming is the best";
	printf("Original : %s\n", str);
	str_reverse(str);
	printf("Reversed : %s\n", str);
	return 0;
}
```

### main_runtime.c
Runtime Linking에서 사용되는 main 코드입니다.
dl라이브러리를 사용해 런타임에 dynamic linking을 명시적으로 요청합니다. 
dlopen과 dlsym을 이용해 동적으로 라이브러리를 로드하고 str_reverse() 함수를 호출합니다.
```c
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
```

---

## 🏗️Static Linking
해당 과정의 실행은 static_linking 디렉토리에서 이루어집니다.
1. object 파일 생성

   str_reverse.c 파일을 gcc를 통해 컴파일하여 str_reverse.o 파일을 생성합니다.

   `$ gcc -Og -c str_reverse.c -o str_reverse.o`
2. 아카이브 파일(정적 라이브러리) 생성

   str_reverse.o 파일을 ar -rcs 명령어를 통해 reverse.a를 생성합니다.

   `$ ar -rcs libreverse.a str_reverse.o`
3. 실행파일 생성

   현재 디렉토리의 static library를 링크하고, exe_static 실행 파일을 생성합니다.

   `$ gcc -static main.c -L. -lreverse -o exe_static`
4. exe_static 파일을 실행합니다.

   <img width="832" height="182" alt="Image" src="https://github.com/user-attachments/assets/cb748614-be5f-4b09-bfc7-46fde5224cde" />

---

## 🔗Dynamic Linking
해당 과정의 실행은 dynamic_linking 디렉토리에서 이루어집니다.
1. object 파일 생성

   str_reverse.c 파일을 gcc를 통해 -fPIC로 컴파일하여 str_reverse.o 파일을 생성합니다.

   `$ gcc -Og -c -fPIC str_reverse.c -o str_reverse.o`
2. 공유 라이브러리 생성

   str_reverse.o 파일을 -shared 명령어를 통해 reverse.so를 생성합니다.

   `$ gcc -shared -o libreverse.so str_reverse.o`
3. 현재 디렉토리의 shared library를 링크하고, exe_dynamic 실행 파일을 생성합니다.

   `$ gcc main.c -L. -lreverse -o exe_dynamic`
4. 현재 디렉토리를 공유 라이브러리 검색 경로로 만들어 환경변수를 설정합니다.

   `$ export LD_LIBRARY_PATH=.:$LD_LIBRARY_PATH`
5. exe_dynamic 파일을 실행합니다.

   <img width="833" height="233" alt="Image" src="https://github.com/user-attachments/assets/acb82809-a393-4b8f-b4a1-7b552f46116c" />

---

## ⚙️Runtime Linking
해당 과정은 runtime_linking 디렉토리에서 이루어집니다.
1. object 파일 생성

   str_reverse.c 파일을 gcc를 통해 -fPIC로 컴파일하여 str_reverse.o 파일을 생성합니다.

   `$ gcc -fPIC -c str_reverse.c -o str_reverse.o`
3. 공유 라이브러리 생성

   str_reverse.o 파일을 -shared 명령어를 통해 str_reverse.so를 생성합니다.

   `$ gcc -shared -o libstr_reverse.so str_reverse.o`
5. 런타임 링크를 사용하도록 수정된 main_runtime.c 파일을 컴파일하여 exe_runtime 파일을 생성합니다.

   `$ gcc -rdynamic -o exe_runtime main_runtime.c -ldl`
7. 현재 디렉토리를 공유 라이브러리 검색 경로로 만들어 환경변수를 설정합니다.

   `$ export LD_LIBRARY_PATH=.:$LD_LIBRARY_PATH`
9. exe_runtime 파일을 실행합니다.

   <img width="837" height="233" alt="Image" src="https://github.com/user-attachments/assets/fcd334e2-a7f8-4d73-9381-c5d9f3707dc3" />

---

## 🧾 요약
이 프로젝트는 문자열 뒤집기 함수를 제작하고, 이를 **세 가지 서로 다른 방식으로 링크하여 적용**하는 과정을 모두 담고 있습니다.
- 정적 링크: 실행 파일에 직접 포함
- 동적 링크: 실행 시 필요한 경우 로딩
- 런타임 링크: 코드 실행 중 명시적으로 라이브러리 로드

각 방식의 차이를 실제 컴파일·실행 과정을 통해 명확히 이해할 수 있습니다.

---
