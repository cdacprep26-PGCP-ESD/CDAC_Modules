#include<stdio.h>
#include<stdlib.h>

int main()
{
	FILE* fp;
	
	fp = fopen("hello.txt", "r");
	
	char str[20];
	int ch;
	int i = 0;
	
	if(fp == NULL)
	{
		perror("File Does not Exists");
		exit(1);
	}
	while( ( ch = fgetc(fp) ) != EOF)
	{ 
		str[i] = ch;
		i++;
	}
	
	str[i] = '\0';
	printf("%s\n", str);
	fseek(fp, 0L, 0);
	i = 0;
	 
	while( ( ch = fgetc(fp) ) != EOF)
	{ 
		str[i] = ch;
		i++;
	}
	
	str[i] = '\0';
	printf("%s\n", str);
	fseek(fp, 0L, 0);
	i = 0;
	
	while( ( ch = fgetc(fp) ) != EOF)
	{ 
		str[i] = ch;
		i++;
	}
	
	str[i] = '\0';
	printf("%s\n", str);
	fseek(fp, 0L, 0);
	
	
	fclose(fp);
	
	return 0;
}
