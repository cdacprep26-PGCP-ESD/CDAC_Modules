//Q9.Recurcive Calculation of Power of n 
#include<stdio.h>

int power(int a, int n)//Recurcive Power function difinition
{
	if(n==0)
	{
		return 1;
	}
	else
	{
		return a*power(a, n-1);
	}
}//power

int main()
{
	int a, n, result;//Variable Declaration

	printf("Enter base:");//Number Input
	scanf("%d",&a);

	printf("Enter power:");//Power Input
	scanf("%d",&n);

	result=power(a,n);

	printf("The power is %d\n", result);
	
	return 0;
}//main
