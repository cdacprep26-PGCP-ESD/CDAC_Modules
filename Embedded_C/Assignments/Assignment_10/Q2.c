#include<stdio.h>
#include<stdlib.h>

int main()
{
	FILE* fp = fopen("Terra.txt", "r+");
	char ch;
	
	if(fp == NULL)
	{
		perror("File Does not Exists");
		exit(1);
	}
	
	int count = 0 ;
	
	while( ( ch = fgetc(fp) ) != EOF)
	{ 
		count ++;
	}
	
	rewind(fp);
	char str[count+1];
	
	
	while ((fgets(str, sizeof(str), fp)) != NULL)
	{
		printf("%s",str);
	} 
	
	
	fclose(fp);
	return 0;
}
