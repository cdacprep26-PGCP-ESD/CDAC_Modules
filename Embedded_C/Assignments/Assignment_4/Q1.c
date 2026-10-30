//printing pascal triangle
#include<stdio.h>

int main()

{
	int n, a;
	printf("Enter Number of rows ");
	scanf("%d",&n);

	for( int i = 0; i<=n; i++) // rows
	{
		for(int j = 1; j<=n-i; j++) // spaces
		{
			printf(" ");
		}

		for( int k = 0; k<=i; k++) // values
		{
			
			if( k==0)
			{
				a=1;
			}
			else
			{
				a = a*(i-k+1)/k;
			}
			
			printf("%d ",a);
			
		}
		printf("\n");
		
	}
	return 0;
}//main

		
