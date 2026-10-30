//Q12.Passing function name as a argument
#include<stdio.h>

int test(int x, int y, int (*fp)(int,int))
{
	 int z = fp(x,y);
	 return z;
}

int sum(int x, int y)
{
	return x+y;
}

int main()
{
	int x,y;
	printf("Enter x&y\n");
	scanf("%d%d",&x, &y);
	printf("Sum is %d\n",test(x,y,sum));//sum function is passed as a argument
	
	return 0;
}//main
