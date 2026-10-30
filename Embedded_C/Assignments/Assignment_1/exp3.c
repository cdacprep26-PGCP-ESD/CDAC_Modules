// biggest of Three numbers
#include<stdio.h>

int main()
{
	int a, b, c;
	printf("Enter three numbers :A, B, C\n ");
	scanf("%d\n", &a);
	scanf("%d\n", &b);
	scanf("%d",&c);
	if(a>b && a>c)
	{
		printf("A is the biggest\n");
	}
	else if(b>a && b>c)
	{
		printf("B is the biggest\n");
	}
	else
	{
		puts("C is the biggest\n");

	}
	return 0;
}//main
