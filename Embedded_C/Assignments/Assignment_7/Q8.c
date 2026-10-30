//Q8.1D and 2D array operations
#include<stdio.h>

//Function Declarations

//1D array functions
void display(int*, int );
void sum(int*, int );	
void min(int* , int );	
void max(int* , int );

//2D array functions
void display_2D(int [3][3], int [3][3]);
void add(int [3][3], int [3][3]);
void sub(int [3][3], int [3][3]);	
void mul(int [3][3], int [3][3]);

int main()
{
	//1D array operations
	int arr[5] = {10,20, 34,578,1000};
	
	//function calls	
	display(arr,5);
	sum(arr,5);
	min(arr,5);
	max(arr,5);
	
	//2D array operations
	int a[3][3] = { {1,4,544},
			    {54,67,24},
			    {545,76,34} };
			    
	int b[3][3] = { {1,2,3},{4,5,6},{7,8,9} };
	//2D array function calls
	display_2D(a,b);
	add(a,b);
	sub(a,b);
	mul(a,b);
	
	return 0;
}//main

//1D array functions definitions

void display(int arr[], int n)
{
	printf("Array Elements : ");
	for(int i = 0; i<n; i++)
	{
		printf("%d ",arr[i]);
	}
	printf("\n");
}//display

void sum(int arr[], int n)
{
	int sum = 0;
	for(int i = 0; i<n; i++)
	{
		sum = sum +arr[i];
	}
	printf("Sum of Array Elements is %d\n",sum);
}//sum

void min(int arr[], int n)
{
	int min = arr[0];
		for(int j = 1; j<n; j++)
		{
			if(arr[j]<min)
			{
				min = arr[j];
			}
		}
	printf("Minimum Element in an array is %d\n", min);
}//min

void max(int arr[], int n)
{
	int max = arr[0];
		for(int j = 1; j<n; j++)
		{
			if(arr[j]>max)
			{
				max = arr[j];
			}
		}
	printf("Maxiimum Element in an array is %d\n", max);
}//max

//2D array function calls

void display_2D(int a[3][3], int b[3][3])
{
	int i,j;
	for(i=0; i<3; i++)
	{
		for(j=0; j<3; j++)
		{
			printf("%d ",a[i][j]);
		}
	
		printf("\n");
	}
	
	for(i=0; i<3; i++)
	{
		for(j=0; j<3; j++)
		{
			printf("%d ",b[i][j]);
		}
	
		printf("\n");
	}
	printf("\n");
}//display_2D

void add(int a[3][3], int b[3][3])
{     
	int sum[3][3];
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
}//add


void sub(int a[3][3], int b[3][3])
{	
	int sub[3][3];
	printf("Subtraction\n");
	int i,j;
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
}//sub


void mul(int a[3][3], int b[3][3])
{	
	int mul[3][3];
	int i,j;
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
}//mul
