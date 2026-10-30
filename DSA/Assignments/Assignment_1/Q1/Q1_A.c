#include<stdio.h>
#include"Q1_B.h"

void createArr();
void insertAt();
void del_at();
void min();
void max();
void display();
void revdis();
void search();
void count();
void avg();
void duplicate();
void rev_mem();

int main()
{
    int choice;
    createArr();


    while(1)
    {
        printf("1.To insert the element\n");
        printf("2.To delete the element\n");
        printf("3.To find min element\n");
        printf("4.To find max element\n");
        printf("5.To display the element of array\n");
        printf("6.To display the reverse element of array\n");
        printf("7.To search element in array\n");
        printf("8.To count the elemnts of array\n");
        printf("9.To find the the average of all elements of array\n");
        printf("10.To check if there is any duplicate element\n");
        printf("11.To reverse the elements in memory\n");
        printf("12.Exit\n");

        printf("Enter your choice\n");
        scanf("%d",&choice);


        
        switch(choice)
        {
            case 1:
                    insertAt();
                    break;

            case 2:
                    del_at();
                    break;
                
            case 3:
                    min();
                    break;
                    
            case 4:
                    max();
                    break;

            case 5:
                    display();
                    break;

            case 6:
                    revdis();
                    break;

            case 7:
                    search();
                    break;

            case 8:
                    count();
                    break;

            case 9:
                    avg();
                    break;

            case 10:
                    duplicate();
                    break;
            
            case 11:
                    rev_mem();
                    break;
            case 12:
                    return 0;
                    break;

            default:
                    printf("Invalide Input\n");
                    break;
        }
    }

    return 0;
}