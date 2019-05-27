#include <sys/sem.h>
#include <iostream>
#include <unistd.h>

using namespace std;

void P(int semid)
{
	struct sembuf buf = {0,-1,0};

	semop(semid,&buf,1);
}

void V(int semid)
{
	struct sembuf buf = {0,1,0};

	semop(semid,&buf,1);
}

void use_toilet(int pid,int semid)
{
	cout << "I'm process: " << pid << " read for use toilet" << endl;
	P(semid);
	cout << "I'm process: " << pid << " using toilet" << endl;
	sleep(1);
	cout << "I'm process: " << pid << " i'm done" << endl;
	V(semid);
}


int main(int argc,char** argv)
{
	key_t keyid = ftok("testforsem",101);

	if (keyid < 0)
	{
		cerr << " get key failed" << endl;
		return -1;
	}

	int semid = semget(keyid,1,IPC_CREAT|0660);
	if (semid < 0)
	{
		cerr << "get semphore failed" << endl;
		return -1;
	}

	//设置信号量最大资源数为 1，只允许一个进程访问
	if (semctl(semid,0,SETVAL,1) < 0)
	{
		cerr << "Inital sem val failed" << endl;
		return -1;
	}

	pid_t pid = fork();

	switch (pid)
	{
		case -1:
			cerr << "Fork failed" << endl;
			break;

		case 0:
			use_toilet(getpid(),semid);
			break;

		default:
			use_toilet(getpid(),semid);
			sleep(3);
			break;
	}

	return 0;
}
