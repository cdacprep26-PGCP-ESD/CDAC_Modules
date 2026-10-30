#include<stdio.h>
#include<stdlib.h>

struct employee 
{
	int emp_id;
	int emp_salary;
	int emp_year;
	char emp_name [20];
};

int max, min; 
float avg =0;
int sum =0, N, live_cnt = 0;

struct employee* em = NULL;

void emp_N()
{
    int N;
    printf("Enter Number of Employee : ");
    scanf("%d",&N);
    em = (struct employee*)calloc(N, sizeof(struct employee));
}

int choice()
{
    int ch;
    printf("Enter Choice : ");
    scanf("%d", &ch);
    return ch;
}
void menu()
{
    printf("1.Create Record\n");
    printf("2.Insert Employee Data\n");
    printf("3.Minimum Salary\n");
    printf("4.Maximum Salary\n");
    printf("5.Average Salary");
    printf("6.Total Salary\n");
    printf("7.Minimum Service\n");
    printf("8.Maximum Salary\n");
    printf("9.Display all Data\n");
    printf("0.Stop\n");
}

void get_data()
{
	printf("Enter Employee ID : ");
	scanf("%d", &em[live_cnt].emp_id);
		
		
	printf("Enter Employee Name: ");
	scanf("%s", em[live_cnt].emp_name);
    
		
		
	printf("Enter Employee Salary : ");
	scanf("%d", &em[live_cnt].emp_salary);
		
		
	printf("Enter Joining Year : ");
	scanf("%d", &em[live_cnt].emp_year);

	live_cnt = live_cnt + 1;
    
		
}

void min_sal()
{
    int min = em[0].emp_salary;
     for(int i = 1; i<live_cnt; i++)
     {
      if( min > em[i].emp_salary)
		{
			min = em[i].emp_salary;
		}      
     }
     printf("Minimum Salary : %d", min);
}


void max_sal()
{
    int max = em[0].emp_salary;
     for(int i = 1; i<live_cnt; i++)
     {
      if( max < em[i].emp_salary)
		{
			max = em[i].emp_salary;
		}      
     }
     printf("Maximum Salary : %d", max);
}

void avg_sal()
{
    for(int i = 0; i<live_cnt; i++)
    {
        sum = sum + em[i].emp_salary;
    }
    avg = sum/live_cnt;
    printf("Average of Salary : %f\n", avg);
}


void total_sal()
{
    int total = 0;
    for(int i = 0; i< live_cnt; i++)
    {
        total = em[i].emp_salary;
    }
}


void min_serv()
{
    int serv, year = 2026;
    serv = 2026 - em[0].emp_year;
    for(int i = 0; i<live_cnt; i++)
    {
        if( (2026 - em[i].emp_year) < serv )
        {
            serv = i;
        }
    }

    printf("Employee ID %d has Minimum service \n", em[serv].emp_id);
    
    
}


void max_serv()
{
    int serv, year = 2026;
    serv = 2026 - em[0].emp_year;
    for(int i = 0; i<live_cnt; i++)
    {
        if( (2026 - em[i].emp_year) > serv )
        {
            serv = i;
        }
    }

    printf("Employee ID %d has Maximum service \n", em[serv].emp_id);
    
}


void display_data()
{
    for(int i = 0; i<live_cnt; i++)
    {
        printf("Enter Employee ID : %d\n",em[i].emp_id);
		
		
		printf("Enter Employee Name: %s\n", em[i].emp_name);
		
		
		printf("Enter Employee Salary : %d\n",em[i].emp_salary);
		
		
		printf("Enter Joining Year : %d\n",em[i].emp_year);
    }
}
