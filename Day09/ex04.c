#include <stdio.h>
#include <unistd.h>
#include <sys/types.h>

int main() {
	pid_t pid;
	printf("Task started\n");
	if ((pid = fork()) > 0) {
		printf("Parent\n");
		sleep(1);
	} else if (pid == 0) {
		printf("Child\n");
		execl("/bin/ls", "ls", "-l", (char*)0);
		printf("exec failed\n");
	} else
		printf("Fork failed\n");
	printf("Task finished\n");
	return 0;
}
