//finding factorial
#include<stdio.h>
int main()
{
	int n,fact=1;
	printf("Enter Number: \n");
	scanf("%d",&n);
	int i;
	//Finding Factorial
	for( i=1; i<=n;i++)
	{
		fact=fact*i;
	}
		printf("factorial is :%d \n",fact);
	
       return 0;
}//main

