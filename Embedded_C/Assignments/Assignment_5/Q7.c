//Q7 Determinant for 2x2, 3x3 matrices
#include<stdio.h>

int main()
{
	int choice;
	int b[2][2], a[3][3];
	int d, d1;
	
	printf("1.Press for 2*2 determinants\n2.Press for 3*3 determinants\nEnetr choice\n");
	scanf("%d",&choice);
	
	switch(choice)
	{
	case 1:
		printf("Enter 2*2 matrix\n");
		
		for(int i=0; i<2; i++)
		{
			for(int j=0; j<2; j++)
			{
				scanf("%d",&b[i][j]);//Input the 2x2 matrix
			}
		}
		
		d=b[0][0]*b[1][1]-b[0][1]*b[1][0];//Finds the value of Determinants
		
		printf("The determinant of 2*2 matrix is:%d\n", d);
		
		break;
		
	case 2:
		printf("Enter 3*3 matrix\n");
		
		for(int i=0; i<3; i++)
		{
			for(int j=0; j<3; j++)
			{
				scanf("%d",&a[i][j]);//Input 3x3 matrix
			}
		}
		
		d1=( a[0][0]*((a[1][1]*a[2][2])-(a[2][1]*a[1][2])) ) - ( a[0][1]*((a[1][0]*a[2][2])-(a[1][2]*a[2][0])) ) + ( a[0][2]*((a[1][0]*a[2][1])-(a[1][1]*a[2][0])) );//Finds the value of Determinants
		
		printf("The determinant of 3*3 matrix is:%d\n", d1);
		
		break;
		
	default:
		printf("Inavlid inpit!!\n");
		
	}
		
		return 0;
}//main
		
		
		
