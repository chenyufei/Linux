#include "./task.h"

void handler(int num)
{
	static i=0;
	i++;
	printf("The %d times receive signal!\n", i);
}

int main(int argc, char* argv[])
{
	int pid=-1;
	int ret;

	signal(SIGUSR1, handler);
	signal(SIGUSR2, NULL);

	while(1);
	return 0;
}
