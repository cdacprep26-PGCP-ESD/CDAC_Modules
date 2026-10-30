//Q4. Iteratively taking Factorial of a Number Input
#include<stdio.h>

int main()
{
	int n,fact=1,i;

	printf("Enter the number:");
	scanf("%d",&n);

	for(i=1; i<=n; i++)//loop for calculation of Factorial
	{
		fact=fact*i;
	}

	printf("The factorial:%d", fact);

	return 0;
}//main
