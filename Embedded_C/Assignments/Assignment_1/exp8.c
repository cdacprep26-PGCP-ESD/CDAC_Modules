// patern 1
#include<stdio.h>
int main()
{
	int n; 
	printf("Enter N \n");
	scanf("%d",&n);
	
	for(int i = 0 ; i<=n ; i++)
	{
		for(int j = 0; j<i; j++)
		{
			printf("%d ",i);
		}
		printf("\n");
	}
	return 0;
} //main

