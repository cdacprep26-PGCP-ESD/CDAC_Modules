//Q12 Checking given matrix is null or not
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
			if(arr[i][j] == 0)//checking given matrix is null or not 
			{
				count++;
			}
			else
			{
				count = 0;
				break;
			}
		}
	}
			
	if(count == 9)
		printf("Given Matrix is NUll Matrix\n");
	else
		printf("Not Null Matrix\n");
	
	return 0;
}//main
