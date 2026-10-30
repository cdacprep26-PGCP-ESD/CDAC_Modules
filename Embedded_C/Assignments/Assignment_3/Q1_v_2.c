//Q1.Recurcive C program to implement the display and reverse display function for integer array element 

#include<stdio.h>

//Function Declaration
void display(int[],int,int);
void rev_display(int[],int,int);

int main()
{
	int n, arr[] = {1, 2, 3, 4, 5};
	
	n = sizeof(arr) / sizeof(arr[0]) ;


	display(arr,n,0);//Display Function call
	printf("\n");
	rev_display(arr, n, 0);//Reverse Display function call

	return 0;
}//main

void display(int arr[],int n,int index) //Recurcive Normal display fuction defination
{
	if( index == n )
	{ 
	 	return ;
	}
	printf("%d ", arr[index]);
	
	display(arr,n,index+1); 
}//display

void rev_display(int arr[],int n,int index) //Recurcive Reverse display function defination
{
        if( index == n )
        {
                return ;
        }
	rev_display(arr,n,index+1); 
        printf("%d ", arr[index]);

        
}//rev_display


