#include <vega/hal.h>
#include <vega/aries.h>
#include<string.h>
#include<stdio.h>
#define MILLIS(time)        (time * 100000)          

volatile int count = 0;
int flag = 0;
int main()
{
   char str[20];

    htimer0.LoadCount = MILLIS(1000);
   
    TIMER_Init(&htimer0);
    UART_Init(&huart0); 

    TIMER_Start_IT(&htimer0);
    
    while (1) 
    {
        if(flag == 1)
        {
            sprintf(str,"Count %d\r\n", count);
            UART_Transmit(&huart0, str, strlen(str));
            flag = 0;
        }
        
    }
    
    return 0;
}



void TIMER_ElapsedCallback(TIMER_Handle_t *htimer)
{
    if(count >= 10)
    {
        count = 0;
    }
   count = count + 1;
   flag = 1;
   
}