#include <sys/types.h>
#include <fcntl.h>
#include <unistd.h>
#include <string.h>

int main() {
	int filedes;
	char tmpstr[] = "Hello, world!!";
	filedes = open("ex3_text.txt", O_RDWR|O_CREAT);
	write(filedes, tmpstr, strlen(tmpstr));
	close(filedes);
	return 0;
}
