#include <stdio.h>
#include <sys/fcntl.h>
#include <string.h>
#include <unistd.h>
#include <stdlib.h>

int main(int argc, char *argv[])
{
	int fd;
	int count = 0;
	char buf[] = "Hello,world\n";

	printf("The sizeof(buf)=%d\n",sizeof(buf));
	fd = open(argv[1], O_RDWR|O_CREAT, 0644);

	for(count=0; count < 1024*1024; count++)
		write(fd, buf, strlen(buf));        //file size = 16*256*1024*1024 = 4Mbyte

	//printf("The sizeof(buf)=%d\n",sizeof(buf));
	close(fd);


	FILE *fp = fopen("test.txt", "wb");
	if (! fp)
	{
		return -1;
	}
	fseek(fp, 1024 * 1024 * 10 -1, SEEK_SET);
	fwrite("a", 1, 1, fp);
	fclose(fp);
	return 0;
}
