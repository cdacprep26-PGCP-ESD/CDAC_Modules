/* Perform the following operations using sprintf  
concatenation of two strings 
copying one string to another 
length of a string 
*/

#include<stdio.h>
#include<stdlib.h>

int main()
{
	char str[20] = "Ad Astra";
	char str2[] = "Per Aspera";
	
	printf("string 1 : %s\n",str);
	printf("string 2 : %s\n\n",str2);
	
	//String Concatenation 
	sprintf(str + 8, " %s", str2);
	printf("Concatenation of two string: %s\n\n",str);
	
	//Count of String
	int cnt;
	char *temp;
	temp = (char*)malloc(100*sizeof(char));
	cnt = sprintf(temp, "%s", str);
	printf("Lenght of string: %d\n\n",cnt);
	free(temp);
	
	//Copying one string to another
	char str3[cnt];
	sprintf(str3,"%s",str);
	printf("Copying string from another string: %s\n\n",str3);
	
	return 0;
}//main
