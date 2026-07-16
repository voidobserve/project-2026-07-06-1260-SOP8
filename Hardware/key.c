#include "key.h"
#include "include.h"
#include "user_config.h"
#include "motor_handle.h"
#include "pwm.h"
#include "led.h"

#if USER_DEBUG_ENABLE
#include <stdio.h>
#endif

// 检测按键的引脚，配置为输入上拉
void key_init(void)
{
    // SW1
    // P3_PU |= GPIO_P31_PULL_UP(0x01);
    P3_MD1 &= ~GPIO_P31_MODE_SEL(0x03); // 输入
}

u8 key0_get_key_val(void)
{
    key_val_t key_val = KEY_VAL_INVALID;
    // 将按键检测脚配置为下拉，检测有没有高电平
    P3_PU &= ~GPIO_P31_PULL_UP(0x01); // 关闭上拉
    P3_PD |= GPIO_P31_PULL_PD(0x01);  // 打开下拉
    delay_ms(1);                      // 配置完引脚后，必须要等电平稳定再检测
    if (KEY_0_PIN == 1)
    {
#if USER_DEBUG_ENABLE
        // printf("key sacn pin == 1\n");
#endif
        key_val = KEY_VAL_VALID_HIGH;
    }

    // 将按键检测脚配置为上拉，检测有没有低电平
    P3_PD &= ~GPIO_P31_PULL_PD(0x01); // 关闭下拉
    P3_PU |= GPIO_P31_PULL_UP(0x01);  // 打开上拉
    delay_ms(1);                      // 配置完引脚后，必须要等电平稳定再检测
    if (KEY_0_PIN == 0)
    {
#if USER_DEBUG_ENABLE
        // printf("key sacn pin == 0\n");
#endif
        key_val = KEY_VAL_VALID_LOW;
    }

    return key_val;
}

void key0_scan_handle(void)
{
#if 0
    // 使用边沿检测（原本是5ms调用一次）
    volatile u8 curr = KEY_0_PIN;         // 存放当前检测到的电平
    static volatile u8 last = U8_MAX_VAL; // 存放上一次检测到的电平

    if (curr == last)
    {
        return;
    }

    if (last != U8_MAX_VAL)
    {
        if (curr)
        {
            // 上升沿
            motor_set_dir(&motor_handle_0, MOTOR_DIR_NONE);
            // printf("key0 level == 1\n");
            // printf("toggle\n");
        }
        else
        {
            // 下降沿
            // printf("key0 level == 0\n");
        }
    }
    else // last == U8_MAX_VAL
    {
        // 第一次上电，忽略
        // printf("key0 last == U8_MAX_VAL\n");
    }

    last = curr;
#endif

    // 使用电平检测
#if 0

    static volatile u8 filter_cnt = 0; // 滤波计数器
    static volatile u8 filter_key_lev = KEY_LEV_UNKNOWN;
    volatile key_lev_t cur_key_lev = KEY_0_PIN; // 获取当前引脚对应的电平
    static volatile key_lev_t last_key_lev = KEY_LEV_UNKNOWN;

    // TODO 第一次上电时，电机不转动
    static volatile u8 is_initialized = 0;

    // TODO 有信号指的是有高电平或者低电平信号，引脚悬空表示没有信号

    if (cur_key_lev != filter_key_lev)
    {
        filter_cnt = 0;
        filter_key_lev = cur_key_lev;
    }

    if (filter_cnt < 3)
    {
        filter_cnt++;
        return;
    }

    if (0 == is_initialized && cur_key_lev == KEY_LEV_HIGH)
    {
        // 没有初始化，说明是刚上电，并且当前引脚是高电平
        return;
    }
    else if (0 == is_initialized && cur_key_lev == KEY_LEV_LOW)
    {
        // 没有初始化，并且当前引脚是低电平，说明当前是上电后，第一次有按键按下
        is_initialized = 1;
    }

    if (last_key_lev != cur_key_lev &&
        cur_key_lev == KEY_LEV_LOW)
    {
        /*
            检测到按键按下，有下降沿时，
            切换方向
        */
        motor_set_dir(&motor_handle_0, MOTOR_DIR_NONE);
    }
    else if (cur_key_lev == KEY_LEV_HIGH)
    {
        // 检测到按键抬起，让电机停止
        motor_handle_0.stop();
        motor_handle_0.status = MOTOR_STATUS_STOP;
        if (MOTOR_STATUS_FORWARD == motor_handle_0.dest_dir)
        {
            // 如果电机是正转后停下来的，点亮灯光
            LED_POWER_ON();
        }
    }

    last_key_lev = cur_key_lev;

#endif

    //
    static volatile u8 filter_cnt = 0; // 滤波计数器
    static volatile key_val_t filter_key_lev = KEY_VAL_INVALID;
    static volatile u8 is_initialized = 0;

    volatile key_val_t cur_key_lev = KEY_VAL_INVALID;

    cur_key_lev = key0_get_key_val();
    if (cur_key_lev != filter_key_lev)
    {
        filter_cnt = 0;
        filter_key_lev = cur_key_lev;
    }

    if (filter_cnt < 3)
    {
        filter_cnt++;
        return;
    }

    if (0 == is_initialized && cur_key_lev == KEY_VAL_INVALID)
    {
        // 没有初始化，并且当前键值无效，说明是上电后，一直没有给有效信号
        return;
    }
    else if (0 == is_initialized && cur_key_lev != KEY_VAL_INVALID)
    {
        // 没有初始化，但是检测到了有效键值，说明是上电后，第一次收到有效信号
        is_initialized = 1;
    }

#if (PRODUCT_TYPE == PRODUCT_TYPE_1)
    if (KEY_VAL_INVALID != cur_key_lev)
    {
        // 键值有效，可能是高电平也可能是低电平
#if USER_DEBUG_ENABLE
        // printf("key val valid\n");
        // printf("key val == %u\n", (u16)cur_key_lev);
#endif

        if (KEY_VAL_VALID_LOW == cur_key_lev)
        {
            motor_set_dir(&motor_handle_0, MOTOR_DIR_FORWARD);
#if USER_DEBUG_ENABLE
            printf("MOTOR_DIR_FORWARD\n");
#endif
        }
        else
        {
            motor_set_dir(&motor_handle_0, MOTOR_DIR_REVERSE);
#if USER_DEBUG_ENABLE
            printf("MOTOR_DIR_REVERSE\n");
#endif
        }
    }
    else
    {
        // 键值无效，说明当前引脚悬空
#if USER_DEBUG_ENABLE
        printf("key val invalid\n");
#endif
        motor_set_dir(&motor_handle_0, MOTOR_DIR_REVERSE);
#if USER_DEBUG_ENABLE
        printf("MOTOR_DIR_REVERSE\n");
#endif
    }
#elif (PRODUCT_TYPE == PRODUCT_TYPE_2)
    if (KEY_VAL_INVALID != cur_key_lev)
    {
        // 键值有效，可能是高电平也可能是低电平
#if USER_DEBUG_ENABLE
        // printf("key val valid\n");
        // printf("key val == %u\n", (u16)cur_key_lev);
#endif

        if (KEY_VAL_VALID_LOW == cur_key_lev)
        {
            motor_set_dir(&motor_handle_0, MOTOR_DIR_REVERSE);
#if USER_DEBUG_ENABLE
            printf("MOTOR_DIR_REVERSE\n");
#endif
        }
        else
        {
            motor_set_dir(&motor_handle_0, MOTOR_DIR_FORWARD);
#if USER_DEBUG_ENABLE
            printf("MOTOR_DIR_FORWARD\n");
#endif
        }
    }
    else
    {
        // 键值无效，说明当前引脚悬空
#if USER_DEBUG_ENABLE
        printf("key val invalid\n");
#endif
        motor_set_dir(&motor_handle_0, MOTOR_DIR_FORWARD);
#if USER_DEBUG_ENABLE
        printf("MOTOR_DIR_FORWARD\n");
#endif
    }
#else
#error "product type invalid"
#endif
}
