//Q4 Addition Subtraction and Multiplication of 2D array
#include<stdio.h>

int main()
{
	int a[3][3]={{1,2,3},
		     {4,5,6},
		     {7,8,9}};
		     
	int b[3][3]={{9,8,7},
		     {6,5,4},
		     {3,2,1}};
		     
	int sum[3][3], sub[3][3], mul[3][3];
	int i,j;
	
	printf("Addition\n");
	
	for(i=0; i<3; i++)
	{
		for(j=0; j<3; j++)
		{
			sum[i][j]=a[i][j]+b[i][j];//Addition of 2D array
		}
	}
	
	printf("Addition of Two matrices\n");
	
	for(i=0; i<3; i++)
	{
		for(j=0; j<3; j++)
		{
			printf("%d ",sum[i][j]);//Prints the result of Addition
		}
	
		printf("\n");
	}
	printf("\n");
	
	printf("Subtraction\n");
	
	for(i=0; i<3; i++)
	{
		for(j=0; j<3; j++)
		{
			sub[i][j]=a[i][j] - b[i][j];//Subtraction of 2D array
		}
	}	
		
	printf("Subtraction of Two matrices\n");
	
	for(i=0; i<3; i++)
	{
		for(j=0; j<3; j++)
		{
			printf("%d ",sub[i][j]);//Prints the result of Subtraction
		}
	
		printf("\n");
	}
	
	printf("\n");
	
	printf("Multiplication\n");
	
	for(i=0; i<3; i++)
	{
		for(j=0; j<3; j++)
		{
			mul[i][j] = 0;
			for(int k =0; k<3; k++)
			{
				mul[i][j] = mul[i][j] + a[i][k] * b[k][j];//Multiplication of 2D array
			}
		}
	}
		
	printf("Multiplication of Two matrices\n");
	
	for(i=0; i<3; i++)
	{
		for(j=0; j<3; j++)
		{
			printf("%d ",mul[i][j]);//Prints Result of multiplication
		}
		printf("\n");
	}
	
	return 0;
}//main
	
	
