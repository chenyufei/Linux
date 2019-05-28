/*
 * File: Signal.c
 *Description: Two process
 1. Father:  copy a file, when receive the SIGUSR1 signal, print the progress
 2. Child: timing trigger the parent process
 *Autor: Jimmy Nie
 */
#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <signal.h>
#include <unistd.h>
#include <errno.h>
#include <sys/fcntl.h>

void handler(int sig);
void SigAlarm(int sig);

int count = 0;      //has read bytes
int fileSize = 0;   //the source file size

int main(int argc, char *argv[])
{
	//variable define
	int fd_src, fd_dst;
	int tmp = 0;        //how many bytes read every time
	char buf[128] ;      //tempature storage buffer

	//0. check the argument
	if(argc != 3)
	{
		printf("%s(%d): Check the arguments, argument=%d(3 is need)\n", argv[0],argc,argc);
		exit(EXIT_FAILURE);
	}

	//1. open the file
	if(-1 == (fd_src=open(argv[1], O_RDONLY)))
	{
		perror("open");
		exit(EXIT_FAILURE);
	}

	//open the destination file, if it does not exist, creat it first
	if(-1 == (fd_dst=open(argv[2], O_RDWR|O_CREAT, 0644)))
	{
		perror("open");
		exit(EXIT_FAILURE);
	}

	//2. Obtain the source file size
	fileSize = lseek(fd_src, 0, SEEK_END);
	if(fileSize < 0)
	{
		perror("lseek");
		exit(EXIT_FAILURE);
	}

	lseek(fd_src, 0, SEEK_SET);

	//3. Father process install SIGUSR1 signal
	if(signal(SIGUSR1, handler) == SIG_ERR)
	{
		perror("signal");
		exit(EXIT_FAILURE);
	}

	//4. Creat a new process(child)
	pid_t  pid;
	if(-1 == (pid=fork()))
	{
		perror("fork");
		exit(EXIT_FAILURE);
	}

	//In child process
	else if(pid == 0)
	{
		//Install the signal SIGALRM
		if(signal(SIGALRM, SigAlarm) == SIG_ERR)
		{
			perror("signal");
			exit(EXIT_FAILURE);
		}

		ualarm(200,10000);  //after 20ms start trigger, and every 50ms trigger once
		//alarm(1);

		while(1)    //execute continues
			;
	}

	//In parent process
	else
	{
		//3. copy source file to destination file
		while(1)
		{
			//read the source file to buf
			if(-1 == (tmp=(read(fd_src, buf, 128))))
			{
				perror("read");
				exit(EXIT_FAILURE);
			}

			//check the end of file
			if(0 == tmp)
			{
				printf("Finished copy the file, and file size:%d\n", fileSize);
				kill(pid, SIGINT);      //finished copy, trigger a signal to child, and terminate child process
				break;
			}

			//write the buffer to the destination file
			if(-1 == write(fd_dst, buf, tmp))
			{
				perror("Write");
				exit(EXIT_FAILURE);
			}

			count += tmp;
		}

		wait(NULL);     //wait child process exit
		close(fd_src);
		close(fd_dst);
	}


	return 0;
}

//function used to print the progree of copy file
void handler(int sig)
{
	int i = 0;
	i = (int)(((float)count / (float)fileSize) * 100);
	printf("\nHas copyed %d%% \n",i);

	int j = 0;
	for(j=0; j<i; j++)
	{
		if(j%2)
			printf("*");
	}

	printf("\n");
}

//used to send SIGUSR1 signal to parent process
void SigAlarm(int sig)
{
	kill(getppid(), SIGUSR1);
	//alarm(1);
}
