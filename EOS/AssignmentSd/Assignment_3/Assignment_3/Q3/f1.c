#include<stdio.h>

int add(int a, int b);
int sub(int a, int b);
int mul(int a, int b);
int div(int a, int b);

int main()
{
	int a, b;
	
	printf("Enter two number:\n");
	scanf("%d%d",&a,&b);
	
	printf("Addition : %d\n",add(a,b));
	printf("Subtraction : %d\n",sub(a,b));
	printf("Multiplication : %d\n",mul(a,b));
	
	if(b != 0)
	{
		printf("Division : %d\n",div(a,b));
	}
	else
	{
		printf("division is not possible\n");
	}
	
	return 0;
	
}
