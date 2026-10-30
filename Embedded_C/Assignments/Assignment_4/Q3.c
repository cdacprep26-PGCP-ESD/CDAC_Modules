#include <stdio.h>

int main()
{
	int i, k;

	printf("Enter the value :\n");
	scanf("%d", &i);
	
	printf("%d\n", k=i++);
	printf("%d\n", k=++i);
	
	//
	
	int y, x;

	printf("Enter the value :\n");
	scanf("%d", &x);

	printf("%d\n", y=(x++)*10);
        printf("%d\n", k=(++x)*10);

	//
	
	int q, p;

	printf("Enter the value :\n");
        scanf("%d", &p);

        printf("%d\n", q=(p--)/3);
        printf("%d\n", q=(--p)/3);


	return 0;
}//main
