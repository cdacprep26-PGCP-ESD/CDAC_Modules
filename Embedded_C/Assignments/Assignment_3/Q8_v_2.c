//Q8.Recurcive Counting of 1's and 0's in the Binary Code 
#include<stdio.h>

//Function Declarations
int count_z(int,int);
int count_o(int,int);

int main()
{
	int b;

	printf("Enter binary code:\n");
	scanf("%d", &b);

	printf("Number of Zero's is %d\n",count_z(b,0));//Count 0's Function call
	printf("Number of one's is %d\n",count_o(b,0));//Count 1's Function call

	return 0;
}//msin

int count_z(int b, int count)//Recurcive Counting 0's Function definition
{
	if(b == 0)
	{
		return count;
	}
	if(b%10 == 0)
	{
		return count_z(b/10, count+1);
	}

	else
	{
		return count_z(b/10, count);
	}
}//count_z

int count_o(int b, int count)//Recurcive Counting 1's Function definition
{
        if(b == 0)
        {
                return count;
        }
        if(b%10 == 1)
        {
                return count_o(b/10, count+1);
        }

        else
        {
                return count_o(b/10, count);
        }
}//count_o
