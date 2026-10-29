#include<vega/hal.h>
#include<stdio.h>
#include<string.h>

void UART(char);
char buf[20];

uint8_t count = 0;
int main()
{
    char ch;
    UART_Init(&huart0);

    while(1)
    {
        UART_Receive(&huart0, (char *)&ch, 1);
        sprintf(buf,"Received  :%c\r\n", ch);
        UART_Transmit(&huart0, buf, strlen(buf));
        UART(ch);   
        
    }
    return 0;
}

void UART(char ch)
{
    switch(ch)
        {
            case '+':
                    count += 1;
                    sprintf(buf,"Count  :%d \r\n", count);
                    UART_Transmit(&huart0, buf, strlen(buf));
                    break;
            case '-':
                    if(count == 0)
                    {
                        sprintf(buf,"Count is Zero\r\n");
                        UART_Transmit(&huart0, buf, strlen(buf));
                    }
                    else
                    {
                        count -= 1;
                        sprintf(buf,"Count  :%d \r\n", count);
                        UART_Transmit(&huart0, buf, strlen(buf));
                    }
                    break;
            case 'r':
                    count = 0;
                    sprintf(buf,"Count  :%d\r\n", count);
                    UART_Transmit(&huart0, buf, strlen(buf));
            default:
                    sprintf(buf,"Count is Zero\r\n");
                    UART_Transmit(&huart0, buf, strlen(buf));
                    break;

        }
}