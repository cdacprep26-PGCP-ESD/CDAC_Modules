//Q7. Creating a nested structure, access members of inner structure from outer one
#include<stdio.h>
struct inner
{
	int c;
	int d;
};

//Nested structure	
struct outer
{
	int a;
	int b;
	struct inner q;
};

int main()
{
	struct outer p = {1,2};
	
	//Accessing members of inner structer
	p.q.c = 3;
	p.q.d = 4;
	
	printf("Outer Structure Elements\na = %d\nb = %d\n", p.a,p.b);
	printf("Outer Structure Elements\nc = %d\nc = %d\n", p.q.c,p.q.d);
	
	return 0;
}//main
	
	
