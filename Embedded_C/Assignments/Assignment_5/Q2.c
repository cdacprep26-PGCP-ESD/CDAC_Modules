//Q2 Addition of two array
#include<stdio.h>

int main()
{
	int arr[5] = {1,2,3,4,5};//First array
	int arr1[5] = {6,7,8,9,10};//Second array
	int add_arr[5];
	
	for(int i = 0 ; i<5; i++)
	{
		add_arr [i] = arr[i] + arr1[i];//Addition of two arrays
	}
	
	for(int i =0; i < 5; i++)
	{
		printf("%d ",add_arr[i]);
	} 
	
	return 0;
	
}//main
