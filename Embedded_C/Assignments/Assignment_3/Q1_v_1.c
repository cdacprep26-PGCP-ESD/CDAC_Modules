//Q1.Iterative Array Display and Reversse Display
#include<stdio.h>

int main()
{
	int n, arr[10];

	printf("Enter the number of element:");
	scanf("%d",&n);

	for(int i=0; i<n; i++)
	{
		scanf("%d",&arr[i]);
	}// Taking input

	printf("Normal Display\n");
	for(int i=0; i<n; i++)//Normal Display
	{
		printf(" %d ", arr[i]);
	}

	printf("\n");
	printf("Reverse Display\n");
	for(int i=n-1; i>=0; i--) //Reverse Display
	{
		printf("%d ", arr[i]);
	}

	return 0;
}//main
