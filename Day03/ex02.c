#include <stdio.h>
#include <string.h>
#include <sys/types.h>
#include <fcntl.h>
#include <unistd.h>

int main() {
	int filedes;
	ssize_t nread;
	char tmpstr[1024];

	filedes = open("./ex3_text.txt", O_RDWR);
	memset(tmpstr, 0, sizeof(tmpstr));
	nread = read(filedes, tmpstr, 1024);
	printf("%s", tmpstr);

	close(filedes);
	return 0;
}
