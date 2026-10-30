//Q10 Understanding pointer to an array
#include<stdio.h>

int main()
{
	int arr[3][4] = { {1,2,3,4},
			   {5,6,7,8},
			   {9,0,1,2} };
	int (*p)[4] = arr;
	// Accessing Elements using array using pointer to an array
	
	printf("Accessing Array Elements\n");
	printf("\n");
	for(int i = 0; i<3; i++)
	{
		for(int j =0; j<4; j++)
		{
			printf("arr[%d[[%d] = %d \t",i,j, (*(p+i))[j] );
		}
		printf("\n");
	}
	printf("\n");
	printf("\n");
	
	// Pointer Values
	
	printf("Printing Pointer Values \n");
	printf("\n");
	
	printf("Size of pointer variavle p : %d\n", sizeof(p));// prints size of array variable in memeory
	
	printf("Size of *p : %d\n",sizeof(*p) );// prints size of first row of 2D matrix
	
	printf("Size of **p : %d\n", sizeof(**p) );// prints size of first element in an array 
	
	printf("value of p : %p\n", p);//prints address stored in pointer p which address of 1st of row 2D matrix
	
	printf("Value of p+1 : %p\n", p+1);//prints address stored in pointer p which address of 2nd roe of 2D matrix
	
	printf("\n");
	printf("\n");
	// Checking Equivalance with array 

	printf("Checking Equivalance With Array\n");
	printf("(*(p+i))[j] & *(*(p+i)+j \n");
	printf("\n");
	for(int i = 0; i<3; i++)
	{
		for(int j =0; j<4; j++)
		{
			printf(
				"arr[%d][%d] = %d, "
				"(*(p+%d))[%d] = %d, "
				"*(*(p+%d)+%d) = %d\n",

				i, j, arr[i][j],

				i, j, (*(p + i))[j],

				i, j, *(*(p + i) + j)
			    );
		}
		printf("\n");
}//main
	
	
return 0;
}
