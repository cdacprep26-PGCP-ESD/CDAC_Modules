//Q3 Conversion of integer from little endian to big endian 
                                                                                                                                                                                                                                                                   #include<stdio.h>
int main()
{
	int n;
	int result;
	
	char *p;
	char *q;
	
	printf("Enter the hexadecimal number:\n");
	scanf("%x",&n);
	
	p = (char *)&n;
	q = (char *)&result;
	
	q[0]=p[3];//converting little endian to big endian
	q[1]=p[2];
	q[2]=p[1];
	q[3]=p[0];
	
	printf("Little Endian number 0x%08X\n",n);
	printf("Big Endian number 0x%08X\n",result);
	return 0;
}//main
	
