/*Define your own functions for the following (using array notation as well as exclusively using pointers)  
concatenation 
comparison 
reversing in memory (Use recursion) 
finding the last occurrence of a given character (Use recursion) 
counting the number of occurrences of a given character */

#include <stdio.h>

//Functions Declarations
char *strcat(char *, char *);
int strcmp(char *, char *);
char *rev_str(char [], int, int);
int l_o(char*, int,int,char);
int n_o(char*, char);

int main()
{
    char str1[20] = "hello";
    char str2[] = "World";
    char ch;
    printf("%s\n", strcat(str1, str2)); //String Concatenation
    printf("%s\n", rev_str(str1, 0, 10)); //Reverse the string
    printf("Enter Alphabate \n");
    scanf("%c",&ch);
    printf("%c is last appered at %d position \n", 'l', l_o(str1, 0, -1,ch)+1);//Gives the last occurance of given character
    printf("Given Alphabate has appeared %d times \n",n_o(str1,ch)); //Gives the count of number of character occurance
    return 0;
}//main

char *strcat(char *str1, char *str2)//Defination of string concatenation
    int i = 0, j = 0;

    while (str1[i] != '\0')
        i++;

    str1[i] = ' ';
    i++;

    while (str2[j] != '\0')
    {
        str1[i] = str2[j];
        i++;
        j++;
    }

    str1[i] = '\0';

    return str1;
}

int strcmp(char *str1, char *str2)//Defination of string comparison
{
    int i = 0;

    while (str1[i] != '\0' && str2[i] != '\0')
    {
        if (str1[i] > str2[i])
            return 1;

        if (str1[i] < str2[i])
            return -1;

        i++;
    }

    if (str1[i] == '\0' && str2[i] == '\0')
        return 0;

    if (str1[i] != '\0')
        return 1;

    return -1;
}

char *rev_str(char str[], int i, int j) //Defination of string Reverse
{
    if (i >= j)
        return str;

    char temp = str[i];
    str[i] = str[j];
    str[j] = temp;

    return rev_str(str, i + 1, j - 1);
}

int l_o(char str[], int i,int j,char ch)//Defination of loc	
	if( str[i] == '\0')
	{
		return j;
	}
	if(str[i] ==ch)
	{
		j = i;
	}
	return l_o(str, i+1,j,ch);
}

int n_o(char str[], char ch)//Defination of noc
{ 
	int cnt =0, i = 0;
	while(str[i] != '\0')
	{
		if(str[i] == ch)
		{
			cnt ++;
		}
		i++;
	}
	
	return cnt;
}
