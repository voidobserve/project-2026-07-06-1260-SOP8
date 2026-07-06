#include "pwm.h"
#include "include.h"
#include "user_config.h"

void pwm_init(void)
{
    // REVIEW 上电之后会有脉冲，需要先设置PWM占空比为0，再使能PWM

    // M+ ON 电机正转开关
    P2_MD0 &= ~GPIO_P21_MODE_SEL(0x03);
    P2_MD0 |= GPIO_P21_MODE_SEL(0x01);
    FOUT_S21 = GPIO_FOUT_STMR0_PWMOUT;

    // M- ON 电机反转开关
    P0_MD1 &= ~GPIO_P06_MODE_SEL(0x03);
    P0_MD1 |= GPIO_P06_MODE_SEL(0x01);
    FOUT_S06 = GPIO_FOUT_STMR1_PWMOUT;

    // ===============================================================================
    // STIMER0 配置
    STMR0_PSC = STMR_PRESCALE_VAL(0x00);                        // 不分频
    STMR0_PRH = STMR_PRD_VAL_H((STMR0_PEROID_VAL >> 8) & 0xFF); // 周期高八位寄存器
    STMR0_PRL = STMR_PRD_VAL_L((STMR0_PEROID_VAL >> 0) & 0xFF); // 周期低八位寄存器
    STMR0_CMPAH = STMR_CMPA_VAL_H(((0) >> 8) & 0xFF);           // 比较值A点高八位寄存器
    STMR0_CMPAL = STMR_CMPA_VAL_L(((0) >> 0) & 0xFF);           // 比较值A点低八位寄存器
    STMR_PWMVALA &= ~STMR_0_PWMVALA(0x1);                       // 计数CNT小于比较值A,PWM输出1,大于等于输出0
    STMR_PWMEN |= STMR_0_PWM_EN(0x1);                           // PWM输出使能
    STMR_CNTMD |= STMR_0_CNT_MODE(0x1);                         // 选择连续计数模式
    STMR_LOADEN |= STMR_0_LOAD_EN(0x1);                         // 自动装载使能
    STMR_CNTCLR |= STMR_0_CNT_CLR(0x1);                         // 计数清零
    STMR_CNTEN |= STMR_0_CNT_EN(0x1);                           // 计数使能
    // ===============================================================================
    // STIMER1 配置
    STMR1_PSC = STMR_PRESCALE_VAL(0x00);                        // 不分频
    STMR1_PRH = STMR_PRD_VAL_H((STMR1_PEROID_VAL >> 8) & 0xFF); // 周期高八位寄存器
    STMR1_PRL = STMR_PRD_VAL_L((STMR1_PEROID_VAL >> 0) & 0xFF); // 周期低八位寄存器
    STMR1_CMPAH = STMR_CMPA_VAL_H(((0) >> 8) & 0xFF);           // 比较值A点高八位寄存器
    STMR1_CMPAL = STMR_CMPA_VAL_L(((0) >> 0) & 0xFF);           // 比较值A点低八位寄存器
    STMR_PWMVALA &= ~STMR_1_PWMVALA(0x1);                       // 计数CNT小于比较值A,PWM输出1,大于等于输出0
    STMR_PWMEN |= STMR_1_PWM_EN(0x1);                           // PWM输出使能
    STMR_CNTMD |= STMR_1_CNT_MODE(0x1);                         // 选择连续计数模式
    STMR_LOADEN |= STMR_1_LOAD_EN(0x1);                         // 自动装载使能
    STMR_CNTCLR |= STMR_1_CNT_CLR(0x1);                         // 计数清零
    STMR_CNTEN |= STMR_1_CNT_EN(0x1);                           // 计数使能
}

// 设置通道0的占空比
void pwm_channel_0_set_duty(u16 channel_duty)
{
    STMR0_CMPAH = STMR_CMPA_VAL_H(((channel_duty) >> 8) & 0xFF); // 比较值
    STMR0_CMPAL = STMR_CMPA_VAL_L(((channel_duty) >> 0) & 0xFF); // 比较值
    STMR_LOADEN |= STMR_0_LOAD_EN(0x1);                          // 自动装载使能
}

// 设置通道1的占空比
void pwm_channel_1_set_duty(u16 channel_duty)
{
    STMR1_CMPAH = STMR_CMPA_VAL_H(((channel_duty) >> 8) & 0xFF); // 比较值
    STMR1_CMPAL = STMR_CMPA_VAL_L(((channel_duty) >> 0) & 0xFF); // 比较值
    STMR_LOADEN |= STMR_1_LOAD_EN(0x1);                          // 自动装载使能
}
