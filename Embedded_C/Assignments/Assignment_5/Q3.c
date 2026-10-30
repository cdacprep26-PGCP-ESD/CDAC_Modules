//Q3 Polynomial evaluation whre corfficient are tored in array
#include<stdio.h>

int main()
{
	int d,x,result = 0;
	printf("Enter Degree : ");
	scanf("%d",&d);
	printf("\n");
	int a[d];
	printf("Enter Coefficients \n");
	
	for(int i = 0; i<=d; i++)
	{
		scanf("%d", &a[i]);//Coefficient Input
	}
	
	printf("Enter x Variable : ");
	scanf("%d", &x);
	
	for(int i=0; i<=d; i++)
	{ 
		result = result*x + a[i];//Result Evaluation
	}
	
	printf("Result is %d\n", result);
	
	return 0;
	
}//main
	
	
	
