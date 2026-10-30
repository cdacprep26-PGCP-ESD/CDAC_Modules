//Q6 Trace of matrix
#include<stdio.h>

int main()
{
	int arr[3][3] = { {1,2,3},
			  {4,5,6},
			  {7,8,9} };
	int trace = 0;
	for(int i = 0; i<3; i++)
	{
		for(int j = 0 ; j<3; j++)
		{
			if( i=j )
			{
				trace = trace + arr[i][j];//Trace of matrix (Addition of diagonal)
			}
		}
	
	}
	printf("Trace of a matrix is %d\n", trace);
	return 0;
}//main	
