#include <stdio.h>
#include <unistd.h>

int main() {
	char *arg[] = { "ls", "-l", (char*)0 };
	printf("Before exec\n");
	execv("/bin/ls", arg);
	printf("After exec\n");
	return 0;
}
