//Q6.Iterative fibonacci series
#include <stdio.h>

int main(){

	int i=0 , j = 1 , k; // variable declaration
	int n;

	printf("how many numbers you want in fibonacci series :");
	scanf("%d", &n);

	while(n != 0) //Iterative Printing of fibonacci series till n
	{
	       
	       printf("%d ", i );
       	       k = i + j;
	       i = j;
	       j = k;
	       n--;
	}
	printf("\n");
	return 0;
}//main
