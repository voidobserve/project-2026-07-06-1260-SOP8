#include "led.h"
#include "include.h"

void led_init(void)
{
    P1_MD1 &= ~GPIO_P17_MODE_SEL(0x03);
    P1_MD1 |= GPIO_P17_MODE_SEL(0x01); // 输出
    
    LED_POWER_OFF();
}