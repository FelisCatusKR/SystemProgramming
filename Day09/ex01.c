#include <stdio.h>
#include <unistd.h>
#include <sys/types.h>

int main() {
	pid_t pid;
	int i = 0;
	i++;
	printf("Before fork (%d)\n", i);
	pid = fork();
	if (pid == 0) {
		printf("Child (%d)\n", ++i);
	} else if (pid > 0)  {
		sleep(2);
		printf("Parent (%d)\n", --i);
	} else
		printf("Fork failed\n");
	return 0;
}
