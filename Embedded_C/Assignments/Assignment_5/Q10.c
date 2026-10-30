//Q10 Checking given matrix is identity or not
#include<stdio.h>

int main()
{
	int arr[3][3];
	int count = 0;
	printf("Enter Matrix Elements\n");
	for(int i = 0; i<3; i++)
	{
		for(int j = 0; j<3; j++)
		{
			scanf("%d",&arr[i][j]);//Input matrix elements
		}
	}
			
	for(int i = 0; i<3; i++)
	{
		for(int j = 0; j<3; j++)
		{
			if( (i==j) && (arr[i][j] == 1) ) //checking matrix is identity or not
			{
				count++;
			}
			else if( (i!=j) && (arr[i][j] != 0) )
			{
				count = 0;
				break;
			}
		}
	}
			
	if(count == 3)
		printf("Given Matrix is Identity Matrix\n");
	else
		printf("Not Identity Matrix\n");
	
	return 0;
}//main
