//Iterative Display and Reverse Display of String 
#include<stdio.h>

int main()
{

	char arr[] = "helllo";
	int i=0;
	
	
	printf("Normal Display\n");	
	while(arr[i] !='\0') //Normal Display
	{
		printf("%c",arr[i]);
		i++;
	}
	printf("\n");
	
	
	printf("Reverse Display\n");
	
	i--;
	while(i>=0) // Reverse Display
	{
		printf("%c",arr[i]);
		i--;
	}
	printf("\n");

	return 0;
}
