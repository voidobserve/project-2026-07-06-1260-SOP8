#ifndef __PWM_H__
#define __PWM_H__

#include "include.h"

#define MAX_PWM_DUTY_VAL (SYSCLK / 1 / 20000)
#define STMR0_PEROID_VAL (SYSCLK / 1 / 20000 - 1) // 周期值=系统时钟/分频/频率 - 1
#define STMR1_PEROID_VAL (SYSCLK / 1 / 20000 - 1) // 周期值=系统时钟/分频/频率 - 1

enum
{
    PWM_DUTY_VAL_0_PERCENT = 0,
    PWM_DUTY_VAL_85_PERCENT = (u16)((u32)MAX_PWM_DUTY_VAL * 85 / 100),
    PWM_DUTY_VAL_100_PERCENT = MAX_PWM_DUTY_VAL,
};
#define PWM_DUTY_VAL_X_PERCENT(x) ((u16)((u32)MAX_PWM_DUTY_VAL * (x) / 100))

void pwm_init(void);
void pwm_channel_0_set_duty(u16 channel_duty);
void pwm_channel_1_set_duty(u16 channel_duty); 

#endif
