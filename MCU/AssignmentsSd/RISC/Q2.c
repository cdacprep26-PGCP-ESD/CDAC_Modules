#include <vega/hal.h>
#include <vega/aries.h>



int main()
{
    GPIO_Init(GPIOB, RGB_RED, GPIO_OUT);
    GPIO_SetPin(GPIOB, RGB_RED);
    GPIO_Init(GPIOB, BTN1, GPIO_IN);

    while(1)
    {

        
        if(GPIO_ReadPin(GPIOB, BTN1) == 0)
        {
            delayms(20);
             if(GPIO_ReadPin(GPIOB, BTN1) == 0)
            {
                GPIO_TogglePin(GPIOB, RGB_RED);
                delayms(250);
            }

        }
        else
        {
            GPIO_TogglePin(GPIOB, RGB_RED);
            delayms(1000);
        }

    }

    return 0;

}