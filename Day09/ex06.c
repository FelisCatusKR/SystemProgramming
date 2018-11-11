#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>

void func1(void);
void func2(void);

int main() {
	printf("Start\n");
	atexit(func1);
	atexit(func2);
	printf("End\n");
	_exit(0);
	return 0;
}

void func1(void) {
	printf("func1\n");
}

void func2(void) {
	printf("func2\n");
}
