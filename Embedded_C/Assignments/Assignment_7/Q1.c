//Q1. Counting how many times a function is called
#include<stdio.h>

int fun(int z);

int main()
{	//Function calls
	fun(0);
	fun(0);
	fun(0);
	fun(0);
	//prints how many times a function is called
	printf("Function is called %d times\n",fun(1));
	
	return 0;
}//main

int fun(int z) //function definition
{
	static int count = 0;
	if(!z)
	{
		count ++;
	}
	return count;
}//fun

