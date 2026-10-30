#include<stdio.h>
#include"Q2_B.h"

void emp_N();
int choice();
void menu();
void get_data();
void min_sal();
void max_sal();
void avg_sal();
void total_sal();
void min_serv();
void max_serv();
void display_data();

int main()
{
    int ch;
    menu();
    
    while(1)
    {
        ch = choice();
        switch(ch)
        {
            case 0:
                    return 0;
            case 1:
                    emp_N();
                    break;
            case 2:
                    get_data();
                    break;
            case 3:
                    min_sal();
                    break;
            case 4:
                    max_sal();
                    break;
            case 5:
                    avg_sal();
                    break;
            case 6:
                    total_sal();
                    break;
            case  7:
                    min_serv();
                    break;
            case 8:
                    max_sal();
                    break;
            case 9:
                    display_data();
                    break;
            default : 
                    printf("Invalid Input\n");
                    break;


        }
    }
}