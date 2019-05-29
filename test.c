#include <stdio.h>
const int a = 10;
int c = 10;
int d = 10;

int f()
{
}

int g()
{
}
int main(void)
{
	const int b = 20;
	int *pa = (int*)&a;
	int *pb = (int*)&b;
	char *pc = "abcd";
	printf("%p\n",pc);
	printf("%s %d\n",__func__,__LINE__);
//	*pa = 30;
	printf("%s %d\n",__func__,__LINE__);
	*pb = 30;
	*(int*)&c = 20;
	printf("%p\n",main);
	printf("%p\n",f);
	printf("%p\n",g);
	printf("%p\n",&a);
	printf("%p\n",&c);
	printf("%p\n",&d);

	return 0;
}
