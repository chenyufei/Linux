#include <unistd.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <fcntl.h>
#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include <sys/sem.h>
#include <sys/ipc.h>
#include <sys/wait.h>

//union semun
//{
//  int val;
//  struct semid_ds *buf;
//  unsigned short *arry;
//};
union semun {
		int              val;    /* Value for SETVAL */
		struct semid_ds *buf;    /* Buffer for IPC_STAT, IPC_SET */
		unsigned short  *array;  /* Array for GETALL, SETALL */
		struct seminfo  *__buf;  /* Buffer for IPC_INFO (Linux-specific) */
};
static int sem_id = 0;
static int set_semvalue();
static void del_semvalue();
static int semaphore_p();
static int semaphore_v();

int main(int argc, char *argv[])
{
		char message = 'X';
		int i = 0;

		// 创建信号量
		sem_id = semget((key_t) 1234, 1, 0666 | IPC_CREAT);

		set_semvalue();
		//semaphore_v();
		pid_t id=fork();
		if(id==0){
				while(1){
						semaphore_p();
						printf("A");
						fflush(stdout);
						usleep(101000);
						printf("A ");
						fflush(stdout);
						usleep(100000);
						semaphore_v();
				}
		}else{
				while(1)
				{
						semaphore_p();
						printf("B");
						fflush(stdout);
						usleep(100100);
						printf("B ");
						fflush(stdout);
						usleep(120000);
						semaphore_v();                                                                                                 
				}
				wait(NULL);
		}
		exit(EXIT_SUCCESS);
}

static int set_semvalue()
{
		// 用于初始化信号量，在使用信号量前必须这样做
		union semun sem_union;
		sem_union.val = 1;
		if (semctl(sem_id, 0, SETVAL, sem_union) == -1)
		{
				return 0;
		}
		return 1;
}

static void del_semvalue()
{
		// 删除信号量
		union semun sem_union;

		if (semctl(sem_id, 0, IPC_RMID, sem_union) == -1)
		{
				fprintf(stderr, "Failed to delete semaphore\n");
		}
}

static int semaphore_p()
{
		// 对信号量做减1操作，即等待P（sv）
		struct sembuf sem_b;
		sem_b.sem_num = 0;
		sem_b.sem_op = -1;//P()
		sem_b.sem_flg = SEM_UNDO;
		if (semop(sem_id, &sem_b, 1) == -1)
		{
				fprintf(stderr, "semaphore_p failed\n");
				return 0;
		}

		return 1;
}

static int semaphore_v()
{
		// 这是一个释放操作，它使信号量变为可用，即发送信号V（sv）
		struct sembuf sem_b;
		sem_b.sem_num = 0;
		sem_b.sem_op = 1; // V()
		sem_b.sem_flg = SEM_UNDO;
		if (semop(sem_id, &sem_b, 1) == -1)
		{
				fprintf(stderr, "semaphore_v failed\n");
				return 0;
		}

		return 1;
}
