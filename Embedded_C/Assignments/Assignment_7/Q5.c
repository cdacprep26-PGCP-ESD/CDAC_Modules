//Q5.Calculating sum and product using single function 
#include<stdio.h>

void fun(int a,int b, int *sum,int *product)
{
	//sum and product
	*sum = a+b;
	*product = a*b;
}//fun
	
int main()
{
	int a,b;
	int sum, product;
	
	printf("Enter the two numbers:\n");
	scanf("%d %d",&a,&b);
	
	fun(a,b,&sum,&product); //function call
	
	printf("Sum:%d\nProduct:%d\n",sum,product);
	
	return 0;
}//main
	
