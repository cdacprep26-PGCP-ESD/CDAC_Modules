//Q4
#include<stdio.h>
int service_t(unsigned short int);
struct employee 
{
	unsigned short int emp_id;
	unsigned short int emp_salary;
	unsigned short int emp_year;
	char emp_name [20];
};
int main()
{
	
	unsigned short int max, min, avg =0;
	unsigned short int sum =0;
	int n;
	printf("Enter Number of records you want to enter: ");
	scanf("%d", &n);
	printf("\n");
	struct employee em[n];
	
	for(int i = 0; i<5; i++)
	{
		printf("Enter Employee ID : ");
		scanf("%hu", &em[i].emp_id);
		printf("\n");
		
		printf("Enter Employee Name: ");
		scanf("%s", em[i].emp_name);
		printf("\n");
		
		printf("Enter Employee Salary : ");
		scanf("%hu", &em[i].emp_salary);
		printf("\n");
		
		printf("Enter Joining Year : ");
		scanf("%hu", &em[i].emp_year);
		printf("\n");
		
		printf("Service time of employee %hu : %d\n", em[i].emp_id, service_t(em[i].emp_year));
		
		sum = sum + em[i].emp_salary;
		if( em[i].emp_salary > em[i-1].emp_salary)
		{
			max = i;
		}
		
		if( em[i].emp_salary < em[i-1].emp_salary)
		{
			min = i;
		}		
		
	}
	
	avg = sum / n;
	
	printf("Employee with maximum salary is %hu\n", em[max].emp_id);
	printf("Employee with mini,u, salary is %hu\n", em[min].emp_id);
	printf("Employee average salary %d\n", avg);
	
	return 0;
}

int service_t(unsigned short int year)
{
	return (2026 - year);
}
	
	
	
	
		
