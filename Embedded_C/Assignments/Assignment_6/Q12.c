//Q12 Checking the size of pointers using an array
#include<stdio.h>

int main()
{
	int arr[3][4]={{1,2,3,4},
			{5,6,7,8},
			{9,1,2,3}};
			
	int (*q)[3][4] = arr;
	
	printf("Printing Pointer Values \n");
	printf("\n");
	
	printf("Size of pointer variavle q : %d\n", sizeof(q));// prints size of pointer variable
	
	printf("Size of *q : %d\n", sizeof(*q) );// prints size of 2D array 
	
	printf("Size of **q : %d\n", sizeof(**q));//prints size of first row in an array
	
	printf("Size of ***q : %d\n", sizeof(***q));//prints size of first element of an array
	
	
	printf("Accessing Array Elements\n");
	printf("\n");
	for(int i = 0; i<3; i++)
	{
		for(int j =0; j<4; j++)
		{
			printf("arr[%d[[%d] = %d \t",i,j, (*q)[i][j] );
		}
		printf("\n");
	}
	printf("\n");
	printf("\n");
	
	return 0;
}//main
	
