//Header Files
#include<stdio.h>
#include<stdlib.h>
#include<stdio.h>
#include<stdlib.h>

int main()
{
	 int **arr;
	 int row = 5,col;
  
   	printf("\nEnter Column-size: ");
        scanf("%d",&col);
        
        //allocate memory for array of pointer
  	arr = (int**)malloc(row * sizeof(int*));
  	
  	//allocate memory for each row
	  for(int i =0;i<row;i++)
	  {
	     arr[i] = (int*)malloc(col * sizeof(int));
	  }
	  
	  
	  //Input Elements
	  printf("\nEnter %d elements: \n",row*col);
	  for(int i = 0;i < row;i++)
	  {
	    for(int j = 0;j < col; j++)
	    { 
	      scanf("%d",&arr[i][j]);
	    }   
	  }
	  
	  //Print 2D Array
	 printf("\nElements:\n");
        for(int i = 0;i < row;i++)
        {
           for(int j=0; j <col;j++)
           {
             printf("%d ",arr[i][j]);
           }
           printf("\n");
           
        }  
        
        //free memory
        for(int i=0; i < row;i++)
        free(arr[i]);
        
        
        free(arr);
           
  
  
  	return 0;
  
 }
 
  
	 

