/*Q10. Trying nesting of structures, unions
Union inside a structure
Structure/Structures inside an union(c) Union inside another union etc.
*/

#include<stdio.h>

//Union inside the structure 
struct Ares
{
	int a;
	int b;
	union Aqua
	{
		int c;
		int d;
	}aq;
}ar;

//Structure inside an Union
union Ares1
{
	int a;
	int b;
	struct Aqua1
	{
		double c;
	}aq1;
}ar1;

//Union inside another Union
union Ares2
{
	int a;
	int b;
	union Aqua2
	{
		double d;
	}aq2;
}ar2;

int main()
{
	printf("size of union inside structure %d and size of structure %d\n", sizeof(ar.aq), sizeof(ar));
	printf("size of structure inside union %d and size of union %d\n", sizeof(ar1.aq1), sizeof(ar1));
	printf("size of union inside union %d and size of union %d\n", sizeof(ar2.aq2), sizeof(ar2));
	
	return 0;
}//main
