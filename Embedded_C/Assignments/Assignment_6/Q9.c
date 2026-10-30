/* Q9 Given int x; Differentiate between int const *p ,const int*p;
       int const * p = &x; const int * const p = &x;
*/

#include<stdio.h>

int main(){
 
  int x = 10;
  int y = 20;

  // CASE 1
  int const *p = &x;  // pointer to constant integer
  
  // *p = 20;  Error : we are trying to change value which is declared as constant
  p++; // valid : it will increment the address of pointer variable
  // (*p)++; Error : Again we are trying to update value which is constant
  p=&y; // valid : Now pointer is pointing to another variable

  printf("*p : %d\n",*p);


  // CASE 2
  const int *p1;
  p1 = &x;
  
  // *p1 = 20;  Error : we are trying to change value which is declared as constant
  p1++; // valid : it will increment the address of pointer variable
  // (*p1)++; Error : Again we are trying to update value which is constant
  p1=&y; // valid : Now pointer is pointing to another variable

  printf("*p1 : %d\n",*p1);


  // CASE 3
  int *const p2 = &x ;  //constant pointer to integer
  
  *p2 = 20 ; //valid : we updated value of variable pointed by const pointer
  // p2++;  Error : constant value cannot be updated
  (*p2)++; //valid : update the value of variable
  // p2 =&y ; Error : We are making pointer point to another variable
  
  printf("p2 : %d\n",*p2);  

  // CASE 4
  const int * const p3 = &x; // constant integer to constant pointer
  
  /*
  *p3 = 20 ; Error : Constnt integer cannot be change
  p3++;  Error : constant value cannot be updated
  (*p3)++; Error : constant int cannot be change
  p3 =&y ; Error : We are making pointer point to another variable
  */

  printf("*p3 : %d\n",*p3);


    return 0;

}//main
     
