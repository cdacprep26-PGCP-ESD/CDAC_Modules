//Q5 Transpose of a matrix
#include<stdio.h>

int main()
{
	int arr[3][3] = { {1,2,3}, {4,5,6,}, {7,8,9} };
	int trnp[3][3];

	for( int i =0; i<3; i++)
	{
		for(int j = 0; j<3; j++)
		{ 
			trnp[j][i] = arr[i][j];//Transpose of matrix
		}
	}

	for(int i = 0; i<3; i++)
	{
		for(int j=0; j<3; j++)
		{
			trnp[i][j] = arr[i][j];//Modifying original matrix
		}

	}

	for(int i = 0; i<3; i++)
	{
		for(int j = 0; j<3; j++)
		{
			printf("%d ", arr[i][j]);//prints transpose matrix
		}
		printf("\n");
	}

	return 0;
}//main



