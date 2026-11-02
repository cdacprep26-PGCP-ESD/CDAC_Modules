#include <vega/aries.h>
#include <vega/hal.h>
#include <string.h>
#include <stdio.h>

int cnt = 0;

int main()
{
    GPIO_Init(GPIOB, BTN0, GPIO_IN);
    UART_Init(&huart0);
    GPIO_Init(GPIOB, RGB_GREEN, GPIO_OUT);
    RGB_GREEN_RST;

    while (1)
    {
        if (GPIO_ReadPin(GPIOB, BTN0) == 0)
        {
            delayms(50);
            if (GPIO_ReadPin(GPIOB, BTN0) == 0)
            {
                cnt++;
                char str[35];
                sprintf(str, "Button Pressed : %d\n\r", cnt);
                UART_Transmit(&huart0, str, strlen(str));

                while (GPIO_ReadPin(GPIOB, BTN0) == 0)
                {
                    delayms(10);
                }
            }
        }

        if (cnt % 5 == 0 && cnt != 0)
        {
            RGB_GREEN_SET;
            delayms(500);
            RGB_GREEN_RST;

            delayms(500);

            RGB_GREEN_SET;
            delayms(500);
            RGB_GREEN_RST;
        }

        if (cnt >= 20)
        {
            cnt = 0;
        }
    }

    return 0;
}