//printing list of prime numbers
#include<stdio.h>

int main()
{
	int n, isPrime; 
	printf("Enter Number :\n");
	scanf("%d",&n);
	for(int i = 2; i<=n; i++)
	{
		isPrime = 1;
		for(int j = 2; j<i; j++)
		{
			if(i%j == 0)
			{
				isPrime = 0;
				break;
			}
		}

		if(isPrime == 1)
		{
			printf("%d\n",i);
		}
	}
return 0;
}//main
