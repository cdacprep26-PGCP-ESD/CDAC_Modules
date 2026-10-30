//Q8 linear Equations
//a1x+b1y=c1 , a2x+b2y=c2
//a1x+b1y+c1z=d1 , a2x+b2y+c2z=d2 , a3x+b3y+c3z=d3

#include<stdio.h>

void l_v_2()//Function defination of First equation
{
      int a[2][2], b[2][1]; 
       
       int D, Dx, Dy;
       int x, y, z;
       printf("Enter Coefficients of x & y  in Matrix Form\n"); 
       for(int i = 0; i<2; i++)
       {
       	for(int j=0; j<2; j++)
       	{
       		scanf("%d",&a[i][j]);//Coefficients of x and y in matrix
       	}
       }
       
       printf("Enter Equivalents of Linear Equations in Matrix Form\n");
       for(int i = 0; i<2; i++)
       {
       	int j=0;
       	scanf("%d",&b[i][j]);//Equivalents of linear Equations 1
       }
       
       D = a[0][0]*a[1][1]-a[0][1]*a[1][0];//Finds value of D (Determinants)
       
       Dx = b[0][0]*a[1][1]-a[0][1]*b[1][0];//Finds value of Dx
       
       Dy = a[0][0]*b[1][0]-b[0][0]*a[1][0];//Finds value of Dy
       
       if(D!=0)
       {
       	x=Dx/D;//Finds x value
       	
       	y=Dy/D;//Finds y value
       	
       	printf("x=%d\n", x);
       	printf("y=%d\n",y);
       }
       else
       {
       	printf("Infinite Solutions");	
       }
}

int det(int a[3][3])//Determinants function defination
{
	int d=( a[0][0]*((a[1][1]*a[2][2])-(a[2][1]*a[1][2])) ) - ( a[0][1]*((a[1][0]*a[2][2])-(a[1][2]*a[2][0])) ) + ( a[0][2]*((a[1][0]*a[2][1])-(a[1][1]*a[2][0])) );
	return d;
}

int Dx(int a[3][3],int b[3][1])//Function defination of Dx
{
	a[0][0] = b[0][0];
	a[1][0] = b[1][0];
	a[2][0] = b[2][0];
	
	return det(a);
}

int Dy(int a[3][3],int b[3][1])//Function defination of Dy
{
	a[0][1] = b[0][0];
	a[1][1] = b[1][0];
	a[2][1] = b[2][0];
	
	return det(a);
}

int Dz(int a[3][3],int b[3][1])//function defination of Dz
{
	a[0][2] = b[0][0];
	a[1][2] = b[1][0];
	a[2][2] = b[2][0];
	
	return det(a);
}

	

void l_v_3()//Function defination of second equation
{
       int a[3][3], b[3][1]; 
       
       int D, dx, dy, dz;
       int x, y, z;
       printf("Enter Coefficients of x, y &z in Matrix Form\n"); 
       for(int i = 0; i<3; i++)
       {
       	for(int j=0; j<3; j++)
       	{
       		scanf("%d",&a[i][j]);//Coefficient of x y and z in matrix
       	}
       }
       
       printf("Enter Equivalents of Linear Equations in Matrix Form\n");
       for(int i = 0; i<3; i++)
       {
       	int j=0;
       	scanf("%d",&b[i][j]);//Equivalents of linear Equations 2
       }
       D = det(a);
       
       
       dx = Dx(a,b);//Function call for dx
       dy = Dy(a,b);//Function call for dy
       dz = Dz(a,b);//function call for dz
       
       
       if(D!=0)
       {
       	x = dx/D;//finds value of x
       	y = dy/D;//finds value of y
       	z = dz/D;//finds value of z
       	
       	printf("x = %d\n", x);
       	printf("y = %d\n",y);
       	printf("z = %d\n",z);
       }
       else
       {
       	printf("Infinite Solutions");	
       }	
}       

int main()
{
	int choice;
	printf("1.Finding Two Varibles x & y\n2.Finding Three Varibles x, y & z\nEnter Choice: ");
       scanf("%d",&choice);
       switch(choice)
       {
       	case 1:
       		l_v_2();//function call of first equation
      			break;
      			
       	case 2:
       		l_v_3();//function call of second equation
       		break;
       
       	default :
       		printf("Invalid choice!!\n");
       }
       
       return 0;
}//main
