#include <stdio.h>
#include <unistd.h>

int main() {
	printf("Before exec\n");
	execl("/bin/ls", "ls", "-l", (char*)0);
	printf("After exec\n");
	return 0;
}
