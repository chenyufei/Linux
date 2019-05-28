#include "./task.h"

#define TAR_TASK "server"

int main(int argc, char* argv[])
{
	int pid=-1;
	int ret;


	//get the server task pid
	ret = getPidByName(&pid, TAR_TASK);
	if((-1 == ret) || (-1 == pid))
	{
		printf("getPidByName was failed!\n");
		exit(EXIT_FAILURE);
	}

	printf("The task %s: pid=%d\n", TAR_TASK, pid);


	//Send a signal to server every 1 second
	int i=0;
	while(i < 10)
	{
		kill(pid,SIGUSR1);
		i++;
		printf("send %d times signal!\n", i);
		sleep(1);
	}

	printf("\nEnd the server and myself!\n");
	kill(pid, SIGUSR2);
	kill(getpid(), SIGSTOP);

	return 0;
}
