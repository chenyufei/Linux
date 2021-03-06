/*
 *File: fifo_send.c
 */

#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <sys/stat.h>
#include <sys/ipc.h>
#include <fcntl.h>


#define FIFO "/tmp/my_fifo"

int main()
{
	char buf[] = "hello,world";

	//`. check the fifo file existed or not
	int ret;
	ret = access(FIFO, F_OK);
	if(ret == 0)    //file /tmp/my_fifo existed
	{
		system("rm -rf /tmp/my_fifo");
	}

	//2. creat a fifo file
	if(-1 == mkfifo(FIFO, 0766))
	{
		perror("mkfifo");
		exit(EXIT_FAILURE);
	}

	//3.Open the fifo file
	int fifo_fd;
	fifo_fd = open(FIFO, O_WRONLY);
	/*
	没有使用O_NONBLOCK 标记时，打开FIFO 来读取的操作会等到其他进程打开FIFO文件来写入才正常返回。
	同样地，打开FIFO文件来写入的操作会等到其他进程打开FIFO 文件来读取后才正常返回。
	*/
	if(-1 == fifo_fd)
	{
		perror("open");
		exit(EXIT_FAILURE);

	}

	//4. write the fifo file
	int num = 0;
	num = write(fifo_fd, buf, sizeof(buf));
	if(num < sizeof(buf))
	{
		perror("write");
		exit(EXIT_FAILURE);
	}

	printf("write the message ok!\n");

	close(fifo_fd);

	return 0;
}
