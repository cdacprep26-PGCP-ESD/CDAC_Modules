//Sume and Average
#include<stdio.h>
int main()
{
	int n, sum=0, avg=0, i=0;
	printf("Enter value of n\n");
	scanf("%d",&n);
	while(i<=n)
	{
		sum = sum + i;
		i++;
	}
	avg = sum/n;
	printf("Sum is %d\n", sum);
	printf("Avg is %d\n", avg);
	return 0;
}//main
