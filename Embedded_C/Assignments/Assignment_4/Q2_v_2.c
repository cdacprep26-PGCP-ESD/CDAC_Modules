#include<stdio.h>
void display(char[],int,int);
void rev_display(char[],int,int);
int main()
{
	char n, arr[] = "Hello";
	
	n = (sizeof(arr) / sizeof(arr[0])) -1 ;


	display(arr,n,0);
	printf("\n");
	rev_display(arr, n, 0);

	return 0;
}

void display(char arr[],int n,int index)
{
	if( index == n )
	{ 
	 	return ;
	}
	printf("%c ", arr[index]);
	
	display(arr,n,index+1);
}

void rev_display(char arr[],int n,int index)
{
        if( index == n )
        {
                return ;
        }
	rev_display(arr,n,index+1);
        printf("%c ", arr[index]);

        
}
