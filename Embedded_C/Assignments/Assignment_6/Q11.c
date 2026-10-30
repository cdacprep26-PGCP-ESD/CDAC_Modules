//Q11 Printing last element without cosidering lenght or size 
#include<stdio.h>

int main()
{
	int arr[] = {10,29,30,40,50};
	int n, elmnt;
	n = sizeof(arr) /  sizeof(int);
	
	printf("Elements in Array\n");
	
	for(int i = 0; i<n; i++)
	{
		printf("%d ", arr[i] );
	}
	printf("\n");
	
	printf("Last element of an array without considering its length is %d\n",*(*(&arr + 1) -1) );
	
	return 0;
}//main
