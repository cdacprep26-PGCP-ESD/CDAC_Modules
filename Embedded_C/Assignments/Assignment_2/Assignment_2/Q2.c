//Income tax calculator
#include <stdio.h>

//declared variables

int age salary, tax, investment, taxable_income;
char gender;
int IncomeTax(int taxable_income , int age, char gender); //function declaration

int main(){
	printf("Enter your Salary : \n");
	scanf("%d",&salary);

	printf("Enter your age :\n");
	scanf("%d",&age);

	printf("Enter your Gender (M-Male & F-Female) : \n");
	scanf(" %c",&gender);

	printf("Enter your Invesed amount :");
	scanf("%d",&investment);
	
	taxable_income = salary - investment;

	int TotalTax = IncomeTax(taxable_income,age,gender);//stored result of SeniorTax function
	printf("Total Tax is %d", TotalTax);
	return 0;
}//main

//function defination
int IncomeTax(int taxable_income , int age , char gender){
   if(age <=60 && gender=='M'){
         if(taxable_income <= 500000){
		 tax = taxable_income * 0.10; //Tax calculation 10%
         }//if
	 else{
		 tax = taxable_income * 0.15; //Tax calculation 15%
	 }
   }//if
   
   else if(age <=60 && gender=='F'){
	   if(taxable_income <= 500000){
                 tax = taxable_income * 0.10; //Tax calculation 10%
         }//else if
         else{
                 tax = taxable_income * 0.12; //Tax calculation 12%
	 }
   }

   else{
        tax = 0; //No Tax
   }
   
return tax;
}//IncomeTax
