//C program to print fibonacci series
#include <stdio.h>

int main(){

	int i=0 , j = 1 , k;
	int n;

	printf("how many numbers you want in fibonacci series :");
	scanf("%d", &n);

	while(n != 0){
	       
	       printf("%d", i );
       	       k = i + j;
	       i = j;
	       j = k;
	       n--;
	}
	return 0;
}
