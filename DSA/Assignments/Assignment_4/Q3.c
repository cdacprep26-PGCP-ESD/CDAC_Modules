/*3. WAP to create a stack of characters in order to store alphabets A-Z.
 And print them in reverse order*/
 
 #include <stdio.h>
 #include <stdlib.h>
 
typedef struct{
    int size;
    int top;
    char *stk;
}stack; 

int isfull(stack *s){
    return (s->top==s->size-1);
}

int isempty(stack *s){
    return (s->top==-1);
}          

void push(stack *s,char ch){
   if(isfull(s)){
      printf("Stack is full,cannot insert alphabet!");
      return;
   }
   s->top++;
   s->stk[s->top]=ch;
}

int pop(stack *s){
   char c;
   if(isempty(s)){
     printf("Stack is empty,cannot delete alphabet!");
     return 1;
   }
   c=s->stk[s->top];
   s->top--;  
   return c;
}
         

int main(){
   stack s;
   s.size=26;
   s.top=-1;
   
   
   s.stk=(char*)malloc(s.size*sizeof(char));
   for(char ch='A';ch<='Z';ch++){
       push(&s,ch);
   }
   
   printf("Original order pushed : A to Z\n");
   printf("Popped order (reverse) : ");
   
   while(!isempty(&s)){
      printf("%c ",pop(&s));
   }
   printf("\n");
   
   return 0;
}          
 
