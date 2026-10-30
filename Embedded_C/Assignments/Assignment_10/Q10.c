#include<stdio.h>
#include<stdlib.h>
int main()
{
	FILE* fp;
	
	fp = fopen("hello.txt", "r");
	
	
	if(fp == NULL)
	{
		perror("File Does not Exists");
		exit(1);
	}
	
	char str[20]; 
	int ch;
	int i = 0;
	for(i = 0; i<7; i++)
	{
		ch = fgetc(fp);
		str[i] = ch;
	}
	
	str[i] = '\0';
	printf("%s\n", str);
	fseek(fp, -5L, 1);
	
	 for(i = 0; i<5; i++)
	{
		ch = fgetc(fp);
		str[i] = ch;
	}
	
	str[i] = '\0';
	printf("%s\n", str);
	
	fclose(fp);
	
	return 0;
}
