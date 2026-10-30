//Q10.Recurcive Calculation of Number of Digits 
#include<stdio.h>

//Function Declarations
int count(int,int);

int main()
{
	int n;

	printf("Enter Number\n");
	scanf("%d",&n);//Number Input

	printf("Number of Digits is %d \n",count(n,0));//Count digit Function call and print

	return 0;
}


int count(int n, int c)//Recurcive Digit count function
{
	if(n == 0)
	{
		return c;
	}
	return count(n/10, c+1);

}//count
