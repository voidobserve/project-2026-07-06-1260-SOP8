#include "include.h"
#include "user_config.h"

#include "timer.h"
#include "task.h"
#include "pwm.h"
#include "adc.h"
#include "key.h"
#include "uart.h"
#include "led.h"

#include "motor_handle.h"

void main(void)
{
    system_init();

    // 关闭HCK和HDA的调试功能
    WDT_KEY = 0x55;  // 解除写保护
    IO_MAP &= ~0x01; // 清除这个寄存器的值，实现关闭HCK和HDA引脚的调试功能（解除映射）
    WDT_KEY = 0xBB;

#if USER_DEBUG_ENABLE
    uart0_init(); // 串口初始化
    printf("uart0_init\n");
#endif

    pwm_init();
    adc_init();
    key_init();
    led_init();

    motor_handle_init();
    timer0_init(); // 系统定时器初始化，需要放在初始化的最后

    delay_ms(10); // 等待系统稳定（至少要等adc值初始化完成）
#if USER_DEBUG_ENABLE
    printf("main while begin\n");
#endif

    while (1)
    {
        WDT_KEY = WDT_KEY_VAL(0xAA); // 喂狗

        task_handle();

        // printf("main\n");
    }
}