//Q7 Understanding and Dereferencing pointer to an array of 5 integers
#include<stdio.h>

int main()
{
	int arr[5] = {10,20,30,40,50};
	int (*parr)[5] ;
	parr=&arr;
	printf("Size of parr : %ld\n", sizeof(parr)); //prints 8 (Size of pointer depends on the machine)
	printf("Size of *parr: %ld\n", sizeof(*parr));//prints 20 (It prints size of array it is pointing to )
	printf("Size of **parr: %ld\n", sizeof(**parr));//prints 4 (It prints size of first element of an array )
	
	//Accessing array elements by dereferencing of parr pointer
	
	for(int i = 0 ; i<5; i++)
	{
		printf("Element at %d position in an array is %d\n",i+1, (*parr)[i]);
	}
	
	return 0;
}
