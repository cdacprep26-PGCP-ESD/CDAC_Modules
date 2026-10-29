#include <vega/hal.h>

#include <stdio.h>
#include <string.h>

static void simple_calci(char*, char*);

int main()
{
    char buf[2];
    char str[50];
    int flag = 0;

    UART_Init(&huart0);


    while (1) 
    {
        if(flag == 0)
        {
            strcpy(str, "Do you Want to Calculate y or n\r\n");
            UART_Transmit(&huart0,"Do you Want to Calculate y or n\r\n", strlen(str));
            UART_Receive(&huart0, buf, 1);
            sprintf(str, "Entered Choice %c\r\n", buf[0] );
            UART_Transmit(&huart0,str, strlen(str) );
            buf[1] = buf[0];

            
            if(buf[1] == 'y')
            {
                simple_calci(str, buf);
            }
            else if(buf[1] == 'n')
            {
                sprintf(str, "---Calculation Ended\r\n" );
                UART_Transmit(&huart0,str, strlen(str));
                flag = 1;
            }
            else{}
        }
        
        
    }
    return 0;
}

static void simple_calci(char str[], char buf[])
{
    int n1, n2, flag = 0;
    char rxdata;

    strcpy(str, "Enter Number\r\n");
    UART_Transmit(&huart0,str, strlen(str));
    UART_Receive(&huart0, buf, 1);
    sprintf(str, "Entered Number %c\r\n", buf[0] );
    UART_Transmit(&huart0,str, strlen(str) );
    n1 = buf[0] - '0';


    strcpy(str, "Enter Operator\r\n");
    UART_Transmit(&huart0,str, strlen(str));
    UART_Receive(&huart0, buf, 1);
    sprintf(str, "Entered Operator %c\r\n", buf[0] );
    UART_Transmit(&huart0,str, strlen(str) );
    rxdata = buf[0];

    strcpy(str, "Enter Number\r\n");
    UART_Transmit(&huart0,str, strlen(str));
    UART_Receive(&huart0, buf, 1);
    sprintf(str, "Entered Number %c\r\n", buf[0] );
    UART_Transmit(&huart0,str, strlen(str) );
    n2 = buf[0] -'0';


    

    int result = 0;

            switch(rxdata)
            {
                case'+':
                        result = n1 + n2;
                        break;
                case'-':
                        result = n1 - n2;
                        break;
                case'*':
                        result = n1 * n2;
                        break;
                
                case'/':
                        if(n2 == 0)
                        {
                            sprintf(str, "Not Defined\r\n" );
                            UART_Transmit(&huart0,str, strlen(str) );
                            flag = 1;
                        }
                        else
                        {
                            result = n1 / n2;
                        }
                        break;
                default:
                        sprintf(str, "Invalid Input\r\n");
                        UART_Transmit(&huart0,str, strlen(str) );
                        break;

            }

            if(flag == 0)
            {
                sprintf(str, "Result %d\r\n", result );
                UART_Transmit(&huart0,str, strlen(str) );
            }

        
}
