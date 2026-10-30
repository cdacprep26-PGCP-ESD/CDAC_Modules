//Q1 Reverse an array in memory

#include<stdio.h>

int main()
{
	int arr[5] = {1,2,3,4,5};
	int arr1[5];
	for(int i =0 ; i<5; i++)
	{
		arr1[i] = arr[i];//Normal array
	}
	int j;
	j =0;
	for(int i =4  i>=0; i--)
	{
		arr[j] = arr1[i];//Reverse array
		j++;
	}
	
	for(int i = 0; i<5; i++)
	{
	printf("%d ",arr[i]);
	}
	
	printf("\n");
	
	return 0;
}//main
