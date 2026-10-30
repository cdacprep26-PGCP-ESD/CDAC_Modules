// Chaeck character is vowel or consonent
#include<stdio.h>

int main()
{
	char ch;
	printf("Enter Character: ");
	scanf("%c",&ch);
	if(ch=='A' || ch=='a' || ch=='e' || ch=='E' || ch=='O' || ch=='o' || ch=='I' || ch=='i' || ch=='U' || ch=='u')
	{
		printf("Given Charater is Vowel\n");
	}
	else
	{
		printf("It is a Consonent\n");
	}
	return 0;
}//main

