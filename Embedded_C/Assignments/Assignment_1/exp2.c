//reversing numbers
#include<stdio.h>

int main()
{
	int n, rev=0, dig=0;
	printf("Enter Number to be reversed :\n ");
	scanf("%d",&n);
	//reversing digits
	while(n>0)
	{
		dig = n%10;
		rev = rev*10 + dig;
		n = n/10;
	}
	printf("Reversed Number : %d ",rev);
	return 0;
}//main	
	
