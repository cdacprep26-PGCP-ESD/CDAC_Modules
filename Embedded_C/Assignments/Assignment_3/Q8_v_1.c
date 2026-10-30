//Q8.Iterative Counting of 1's and 0's in the Binary Code 
#include<stdio.h>

int cnt_0(int n)//Iterative Counting 0's Function definition
{
	int count = 0;
	int i = 0;
	while(n>0)//iterative counting of 0's
	{
		if(n%10 == 0)
		{
			count ++;
		}
		n = n/10;
	}
	return count;
}//cnt_0

int cnt_1(int n)//Iterative Counting 1's Function definition
{
	int count = 0;
	int i = 0;
	while(n>0)//iterative counting of 1's
	{
		if(n%10 == 1)
		{
			count ++;
		}
		n = n/10;
	}
	return count;
}//cnt_1

int main()
{
	int n,ch;
	
	printf("Enter Binary code\n");
	scanf("%d",&n);
	printf("1.Count Zero's\n 2.Count ONe's\n Enter Choice\n");
	scanf("%d",&ch);
	switch(ch)//Choosing to 1's or 0's
	{
		case 1: 
			printf("Count of Zero's is %d\n", cnt_0(n));//Count 0's Function call
			break;
		case 2:
			printf("Count of One's is %d\n",cnt_1(n));//Count 1's Function call
			break;
		default:
			printf("Invalid Input !!!\n");
			
	}
	return 0;
}//main
