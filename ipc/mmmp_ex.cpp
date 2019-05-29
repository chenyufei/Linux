#include<iostream>
#include<sys/mman.h>
#include<pthread.h>
#include<string.h>
#define uint unsigned int
#define MMSIZE (uint)(1<<23)
#define MSIZE (uint)(1<<20)
#define MPRT (uint)(1<<16)

using namespace std;

class tc
{
	private:
		uint a;
	public:
		tc():a(1)
	{
		;
	}
		~tc()
		{
			;
		}
		void add()
		{
			a=a+1;
		}
		void set()
		{
			a=1;
		}
		void prt(int i)
		{
			if(!(i%(MPRT)))
			{
				cout<<":"<<a;
			}
		}

};

struct tt
{
	tc* p1;
	pthread_mutex_t* p2;
};

void* test(void* arg)
{
	int i = 0;
	tt* s = NULL;
	s = (tt*)arg;
	int maxloop = 50;
	while(maxloop--)
	{
		i = MSIZE;
		pthread_mutex_lock(s->p2);//MUTEX保护临界区
		cout<<"Thread:"<<pthread_self()<<" work now!!!\n";
		for(;i--;)
		{
			(s->p1+i)->prt(i);
			(s->p1+i)->add();
		}
		cout<<"\n";
		pthread_mutex_unlock(s->p2);//解锁
	}
}



int main(void)
{
	pthread_t tid[3];
	pthread_mutex_t pmut;
	tt s1;
	tc* p = (tc*)mmap(NULL,MMSIZE,PROT_READ|PROT_WRITE,MAP_ANONYMOUS|MAP_PRIVATE,-1,0);//MMAP分配一个匿名私有虚拟内存用于线程间通信
	pthread_mutex_init(&pmut,NULL);
	s1.p1 = p;
	s1.p2 = &pmut;

	int i = MSIZE+1;

	for(;i--;)
	{
		(p+i)->set();//初始化所有的a=1
	}


	for(i=0;i<3;i++)
	{
		pthread_create(tid+i,NULL,test,(void*)&s1);//建立3个线程
	}

	for(i = 0;i<3;i++)
	{
		pthread_join( *(tid+i) , NULL);//堵塞回收线程
	}
	pthread_mutex_destroy(&pmut);
	munmap(p,MMSIZE);

}
