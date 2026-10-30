//Q10.Iterative Calculation of Number of Digits 
#include<stdio.h>

int cnt(int n)//Iterative Digit count function
{
	int count = 0;
	while(n>0)//Iterative Counting of Digits
	{
		count ++;
		n = n/10;
	}
	return count;
}//cnt
int main()
{
	int n;
	
	printf("Enter Number\n");
	scanf("%d",&n);//Number Input
	
	printf("Number of Digits in the number is %d\n", cnt(n));//Count digit Function call and print
	return 0;
}//main
