//Q10.Understanding function pointers
#include<stdio.h>

typedef void (*f_ptr)(int,int); // Defining function pointer as a type
void test(int, int); // function declarations
void test2(int, int);

int main()
{
	f_ptr fp1; //pointer declarations
	fp1 = test;
	fp1(20,10); //test function call using pointer
	
	//function pointer inside main function
	void (*fp2) (int , int );
	fp2 = test2;
	fp2 (10,10);
	return 0;
}//main

//Function Definitons 
void test(int a, int b)
{
	printf("Just Testing Function Pointer so this is sum %d\n", a+b);
}//test

void test2(int a, int b)
{
	printf("Just Testing Function Pointer so this is sum %d\n", a+b);
}//test1
