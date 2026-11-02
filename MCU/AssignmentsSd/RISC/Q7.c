#include <vega/aries.h>
#include <vega/hal.h>
#include <string.h>
#include <stdio.h>

#define MILLIS(time) (time * 100000)

volatile int cnt;
int led;
char c;
char str[30];
int main()
{
    GPIO_Init(GPIOB, RGB_BLUE, GPIO_OUT);
    RGB_BLUE_RST;
    UART_Init(&huart0);
    TIMER_Init(&htimer0);
    htimer0.LoadCount = MILLIS(1000);
    TIMER_Start_IT(&htimer0);

    while (1)
    {
        UART_Receive(&huart0, (char *)&c, 1);
        sprintf(str, "You Entered : %c\n\r", c);
        UART_Transmit(&huart0, str, strlen(str));

        if (c == 'o')
        {
            RGB_BLUE_SET;
            cnt = 0;
            led = 1;
        }

        if (c == 's')
        {
            if (led)
            {
                sprintf(str, "LED IS ON\n\r");
                UART_Transmit(&huart0, str, strlen(str));
            }
            else
            {
                sprintf(str, "LED IS OFF\n\r");
                UART_Transmit(&huart0, str, strlen(str));
            }
        }
    }
}

void TIMER_ElapsedCallback(TIMER_Handle_t *htimer)
{
    
    cnt++;

    if (cnt > 5)
    {
        RGB_BLUE_RST;
        led = 0;
    }
}
