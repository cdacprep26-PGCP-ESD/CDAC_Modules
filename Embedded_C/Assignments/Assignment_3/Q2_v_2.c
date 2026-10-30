//Recursive Display of String 
#include<stdio.h>

void display(char[],int,int); //Function Declaration
void rev_display(char[],int,int);

int main()
{
	int n; 
	char arr[] = "Hello"; // Sting Definiton and Initiallisation 
	
	n = (sizeof(arr) / sizeof(arr[0])) -1 ;


	display(arr,n,0); // Display Function call
	printf("\n");
	rev_display(arr, n, 0);//Reverse Display Function call

	return 0;
} //main

void display(char arr[],int n,int index) //Display Function Definition 
{
	if( index == n )
	{ 
	 	return ;
	}
	printf("%c ", arr[index]);
	
	display(arr,n,index+1);
} //display

void rev_display(char arr[],int n,int index) // Reverse Display Function Definition
{
        if( index == n )
        {
                return ;
        }
	rev_display(arr,n,index+1);
        printf("%c ", arr[index]);

        
}//rev_display
