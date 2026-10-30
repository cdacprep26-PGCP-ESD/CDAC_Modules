//Iterative sum of N numbers
#include<stdio.h>

int main()
{
	int n,i, sum=0;

	printf("Enter the number:");
	scanf("%d", &n);

	for(i=1; i<=n; i++) //Iteratively taking sum
	{
		sum=sum+i;
	}

	printf("Sum is:%d", sum);

	return 0;
}//main
