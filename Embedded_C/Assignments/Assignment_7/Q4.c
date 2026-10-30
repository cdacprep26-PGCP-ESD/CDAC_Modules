//Q4.Swapped values using call by value and call by reference
#include<stdio.h>

void swap_value(int x,int y) //call by value
{
	int temp;
	
	temp=x;
	x=y;
	y=temp;
	
	printf("The values after swap a=%d, b=%d\n",x,y);
}//swap_value

void swap_reference(int *x,int *y) // call by reference
{
	int temp;
	
	temp=*x;
	*x=*y;
	*y=temp;
	
	
}//swap_reference

int main()
{	
	int a,b;
	printf("Enter two values:\n");
	scanf("%d %d",&a,&b);
	
	printf("The values before swap a=%d, b=%d\n",a,b);
	swap_value(a,b);//swapped using call by value
	
	printf("The values before swap a=%d, b=%d\n",a,b);
	swap_reference(&a,&b);//swapped using call by value
	
	printf("The values After swap a=%d, b=%d\n",a,b);
	
	return 0;
}//main
	
	
	
	
	
	
