//Q9 Generation of identity and null matrices
#include<stdio.h>

int main()
{
	int ch, arr[3][3];
	printf("1.Generate Null Matrix\n2.Generate Identity Matrix\nEnter Choice ");
	scanf("%d",&ch);
	printf("\n");
	
	switch(ch)
	{
		case 1:
			for(int i = 0; i<3; i++)
			{
				for(int j = 0; j<3; j++)
				{
					arr[i][j] = 0;
					printf("%d ",arr[i][j]);//Generate Null matrix
				}
				printf("\n");
			}
			break;
		case 2:
			for(int i = 0; i<3; i++)
			{
				for(int j = 0; j<3; j++)
				{
					if( i == j)
					{
						arr[i][j] = 1;//Generate identity matrix
					}
					else
					{
						arr[i][j] = 0;
					}
					printf("%d ",arr[i][j]);
				}
				printf("\n");
			}
			break;
		default :
			printf("Invalid Input !!\n");
		}
	return 0;
}//main	
			
			
			
			
			
			
			
